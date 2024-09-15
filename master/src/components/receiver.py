from datetime import datetime
from typing import Optional

from PySide6 import QtWidgets
from PySide6.QtCore import QObject, Signal, QByteArray


class ReceiverUi(QtWidgets.QWidget):
    def __init__(self, parent: Optional[QtWidgets.QWidget] = None) -> None:
        super().__init__(parent=parent)

        self.prompt = QtWidgets.QLabel(r"文本格式：", self)
        self.type = QtWidgets.QComboBox(self)
        self.type.addItems(["HEX", "ASCII"])
        self.clear = QtWidgets.QPushButton("清空", self)
        self.text = QtWidgets.QTextEdit(self)
        self.text.setReadOnly(True)

        horizontal = QtWidgets.QHBoxLayout()
        horizontal.addWidget(self.prompt)
        horizontal.addWidget(self.type, 1)
        horizontal.addStretch(1)
        horizontal.addWidget(self.clear)

        vertical = QtWidgets.QVBoxLayout()
        vertical.addLayout(horizontal)
        vertical.addWidget(self.text)

        self.setLayout(vertical)


class Receiver(QObject):
    received_content = Signal(str)

    def __init__(self, parent: Optional[QtWidgets.QWidget] = None) -> None:
        super().__init__()

        self.ui = ReceiverUi(parent)
        self.ui.clear.clicked.connect(self.clear_data)

    def clear_data(self) -> None:
        self.ui.text.clear()

    def display(self, data: str | bytes) -> None:
        time = datetime.now()
        time = time.strftime("%d/%m/%Y %H:%M:%S")
        if isinstance(data, bytes):
            data = data.hex(" ").upper()
        self.ui.text.append(f"[{time}]-{data}")

    def recv_display(self, recv: QByteArray) -> None:
        if recv is None:
            return
        if self.ui.type.currentText() == "HEX":
            recv = recv.data().hex(" ").upper()
        elif self.ui.type.currentText() == "ASCII":
            recv = recv.data().decode()

        self.display(f"Rx: {recv}")
        self.received_content.emit(f"Rx: {recv}")
