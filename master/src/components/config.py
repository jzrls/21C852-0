from typing import Optional

from PySide6 import QtWidgets
from PySide6.QtCore import Signal, QObject, Qt
from PySide6.QtGui import (
    QIntValidator,
    QStandardItemModel,
    QStandardItem,
    QRegularExpressionValidator,
)

from src.components.LineEdit import LineEdit
from src.components.converter import Converter
from src.protocol.crc import Crc
from src.protocol.instruction import Instruction, Counter


class Byte:
    def __init__(
            self, name: str, bits: str, the_type: str, value: int | float, description: str,
            is_crc: bool = False, is_frame_count: bool = False
    ) -> None:
        self.name = name
        self.bits = bits
        self.type = the_type
        self.value = value
        self.description = description
        self.is_crc = is_crc
        self.is_frame_count = is_frame_count


class Frame:
    def __init__(self, name: str = None) -> None:
        self.name = name
        self.bytes = []


class Item(QtWidgets.QWidget):
    def __init__(self, frame: Frame, parent: QtWidgets.QListWidgetItem = None):
        super().__init__()

        self.setContentsMargins(0, 0, 0, 0)

        self.frame = frame

        # 创建按钮
        self.delete = QtWidgets.QPushButton("删除", self)
        parent.setSizeHint(self.delete.sizeHint())

        # 设置布局
        layout = QtWidgets.QHBoxLayout(self)
        spacer = QtWidgets.QSpacerItem(10, 10, QtWidgets.QSizePolicy.Policy.Expanding,
                                       QtWidgets.QSizePolicy.Policy.Minimum)
        layout.addItem(spacer)
        layout.addWidget(self.delete)
        layout.setContentsMargins(5, 0, 5, 0)
        layout.setAlignment(Qt.AlignTop)


class ConfigUi(QtWidgets.QWidget):
    def __init__(self, parent: Optional[QtWidgets.QWidget] = None) -> None:
        super().__init__(parent=parent)

        self.name = LineEdit("指令名称", "未命名", self)
        self.name.layout.addStretch(1)
        self.pattern = QtWidgets.QComboBox(self)
        self.pattern.addItems(["小端模式", "大端模式"])

        self.save = QtWidgets.QPushButton("保存", self)
        self.send = QtWidgets.QPushButton("发送", self)
        self.add = QtWidgets.QPushButton("增加", self)
        self.delete = QtWidgets.QPushButton("删除", self)

        self.model = QStandardItemModel(self)
        self.model.setHorizontalHeaderLabels(["名称", "位宽", "类型", "值", "描述"])

        self.table = QtWidgets.QTableView(self)
        self.table.setModel(self.model)
        self.header = self.table.horizontalHeader()
        self.header.setSectionResizeMode(QtWidgets.QHeaderView.ResizeMode.Stretch)

        self.commands = QtWidgets.QListWidget(self)
        # self.commands.setSelectionMode(QtWidgets.QListWidget.SelectionMode.NoSelection)

        self.layout = QtWidgets.QGridLayout(self)

        self.layout.addWidget(self.name, 0, 0, 1, 3)
        self.layout.addWidget(self.pattern, 0, 3, 1, 1)

        self.layout.addWidget(self.add, 1, 0, 1, 1)
        self.layout.addWidget(self.delete, 1, 1, 1, 1)
        self.layout.addWidget(self.save, 1, 2, 1, 1)
        self.layout.addWidget(self.send, 1, 3, 1, 1)

        self.layout.addWidget(self.table, 2, 0, 3, 4)
        self.layout.addWidget(self.commands, 6, 0, 3, 4)
        self.setLayout(self.layout)


class Config(QObject):
    command = Signal(bytes)

    SIZES = [
        "int8",
        "int16",
        "int32",
        "int64",
        "uint8",
        "uint16",
        "uint32",
        "uint64",
        "float",
        "double",
        "fc",
        "crc16",
        "crc32",
        "CRC-16/CCITT",
        "CRC-16/IBM",
        "CRC-16/MODBUS",
        "CRC-16/MAXIM",
        "CRC-16/USB",
        "CRC-16/XMODEM",
    ]

    TYPES = ["BIN", "OCT", "DEC", "HEX"]

    def __init__(self, parent: Optional[QtWidgets.QWidget] = None):
        super().__init__()

        self.counter: Counter = Counter()
        self.crc = Crc.check_table

        self.previous = []

        self.ui = ConfigUi(parent)

        self._default()
        self._connect()

    def _default(self):
        items = ["帧头1", "帧头2", "帧头3", "帧计数", "帧类型", "数据长度", "帧尾1", "帧尾2", "帧尾3"]

        for item in items:
            self.add_row(item)

    def _connect(self) -> None:
        self.ui.save.clicked.connect(self.save)
        self.ui.send.clicked.connect(self.send)
        self.ui.add.clicked.connect(self.add_row)
        self.ui.delete.clicked.connect(self.delete_row)

        self.ui.commands.clicked.connect(self.edit)
        # self.ui.commands.doubleClicked.connect(self.ui.commands.editItem)
        self.ui.commands.itemDoubleClicked.connect(self.ui.commands.editItem)

    def rename(self):
        pass

    def set_validator(self, text):
        reg = None
        if text == self.TYPES[0]:
            reg = QRegularExpressionValidator("[0-1]+")
        elif text == self.TYPES[1]:
            reg = QRegularExpressionValidator("[0-7]+")
        elif text == self.TYPES[2]:
            reg = QRegularExpressionValidator("[0-9]+")
        elif text == self.TYPES[3]:
            reg = QRegularExpressionValidator("[0-9a-fA-F]+")
        else:
            pass

        validator = QRegularExpressionValidator(reg)
        row = self.ui.table.selectionModel().currentIndex().row()
        value = self.ui.table.indexWidget(self.ui.model.index(row, 3))
        if isinstance(value, QtWidgets.QLineEdit):
            value.setValidator(validator)

    def add_row(self, item: str = "", bits_text: str = 'uint8', type_text: str = 'HEX', value: str = "",
                description: str = "") -> None:
        index = self.ui.table.selectionModel().currentIndex()
        if index.isValid():
            row = index.row() + 1
        else:
            row = self.ui.model.rowCount()

        self.ui.model.insertRow(row)

        # 设置第一列，名称
        self.ui.model.setItem(row, 0, QStandardItem(item))

        # 设置第二列，位宽
        _bits = QtWidgets.QComboBox()
        _bits.addItems(self.SIZES)
        _bits.setCurrentText(bits_text)
        self.ui.table.setIndexWidget(self.ui.model.index(row, 1), _bits)

        # 设置第三列，类型
        _type = QtWidgets.QComboBox()
        _type.addItems(self.TYPES)
        _type.setCurrentText(type_text)
        self.ui.table.setIndexWidget(self.ui.model.index(row, 2), _type)

        #  设置第四列，值
        _value = QtWidgets.QLineEdit()
        _value.setValidator(QIntValidator())
        _value.setText(value)
        self.ui.table.setIndexWidget(self.ui.model.index(row, 3), _value)

        # 设置第5列，描述
        self.ui.model.setItem(row, 4, QStandardItem(description))

        # 根据数据表示方法更新值
        _type.currentTextChanged.connect(self.set_validator)
        self.previous.insert(row, _type.currentText())
        _type.currentTextChanged.connect(self.update_value)

    def delete_row(self):
        index = self.ui.table.selectionModel().currentIndex()
        if index.isValid():
            row = index.row()
        else:
            row = self.ui.model.rowCount() - 1
        self.previous.pop(row)
        self.ui.model.removeRow(row)

    def update_value(self, destination) -> None:
        row = self.ui.table.selectionModel().currentIndex().row()

        source = self.previous[row]

        value = self.ui.table.indexWidget(self.ui.model.index(row, 3))
        if isinstance(value, QtWidgets.QLineEdit):
            converter = Converter.get_converter_function(source, destination)
            value.setText(
                converter(value.text())
            ) if converter and value.text() else value.text()
            self.previous[row] = destination

    def _frame(self) -> Frame:
        frame = Frame()
        name = self.ui.name.get_text()
        if name:
            frame.name = name
        else:
            frame.name = r"未命名"

        for row in range(self.ui.model.rowCount()):
            name = self.ui.model.item(row, 0).text()

            bits = self.ui.table.indexWidget(self.ui.model.index(row, 1))
            if isinstance(bits, QtWidgets.QComboBox):
                bits = bits.currentText()

            _type = self.ui.table.indexWidget(self.ui.model.index(row, 2))
            if isinstance(_type, QtWidgets.QComboBox):
                _type = _type.currentText()

            value = self.ui.table.indexWidget(self.ui.model.index(row, 3))
            if isinstance(value, QtWidgets.QLineEdit):
                if value.text():
                    if _type == self.SIZES[8] or _type == self.SIZES[9]:
                        value = float(value.text())
                    else:
                        value = int(value.text())

            description = self.ui.model.item(row, 4).text()

            byte = Byte(name, bits, _type, value, description, bits == 'fc', bits == 'crc16')
            frame.bytes.append(byte)

        return frame

    def save(self) -> None:
        frame = self._frame()
        items = self.ui.commands.findItems(frame.name, Qt.MatchExactly)
        if not items:
            widget_item = QtWidgets.QListWidgetItem(self.ui.commands)
            widget_item.setText(frame.name)

            # 动态绑定功能
            item = Item(frame, widget_item)
            item.delete.clicked.connect(self.delete)

            self.ui.commands.setItemWidget(widget_item, item)

    def delete(self, item):
        item = self.ui.commands.currentItem()
        # 如果没有选中的项，就返回
        if not item:
            return
        # 从列表中删除项
        self.ui.commands.takeItem(self.ui.commands.row(item))

    def send(self) -> None:
        cmd = Instruction(self.counter, self.crc, model='little' if self.ui.model == '小端模式' else 'big')
        frame = self._frame()
        for byte in frame.bytes:
            cmd.add(byte.name, byte.value, byte.bits, byte.is_frame_count, byte.is_crc)
        self.command.emit(cmd.pack())

    def edit(self, index):
        self.ui.model.removeRows(0, self.ui.model.rowCount())
        item = self.ui.commands.indexWidget(index)

        if not isinstance(item, Item):
            return

        self.ui.name.set_text(item.frame.name)

        for byte in item.frame.bytes:
            self.add_row(byte.name, byte.bits, byte.type, str(byte.value), byte.description)
