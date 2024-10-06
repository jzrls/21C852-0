from PySide6 import QtWidgets
from PySide6.QtCore import Signal, QIODeviceBase, QByteArray, QThread
from PySide6.QtSerialPort import QSerialPortInfo, QSerialPort

from src.components.GroupBox import GroupBox_1
from src.components.config import Config
from src.components.receiver import Receiver
from src.components.transformer import Transformer
from src.tools.log import Recorder


class SerialSettings(QtWidgets.QDialog):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("串口设置")
        self.setFixedSize(300, 200)

        self.form_layout = QtWidgets.QFormLayout()

        self.baud_rate_combo = QtWidgets.QComboBox()
        self.baud_rate_combo.setEditable(True)
        self.baud_rate_combo.addItems(
            ["2400", "4800", "9600", "19200", "38400", "57600", "115200"]
        )
        self.baud_rate_combo.setCurrentIndex(6)
        self.form_layout.addRow("波特率：", self.baud_rate_combo)

        self.data_bits_combo = QtWidgets.QComboBox()
        self.data_bits_combo.addItems(["5", "6", "7", "8"])
        self.data_bits_combo.setCurrentIndex(3)
        self.form_layout.addRow("数据位：", self.data_bits_combo)

        self.parity_combo = QtWidgets.QComboBox()
        self.parity_combo.addItems(["None", "Even", "Odd", "Space", "Mask"])
        self.parity_combo.setCurrentIndex(2)
        self.form_layout.addRow("校验位：", self.parity_combo)

        self.stop_bits_combo = QtWidgets.QComboBox()
        self.stop_bits_combo.addItems(["1", "1.5", "2"])
        self.form_layout.addRow("停止位：", self.stop_bits_combo)

        self.button_box = QtWidgets.QDialogButtonBox(
            QtWidgets.QDialogButtonBox.StandardButton.Ok
            | QtWidgets.QDialogButtonBox.StandardButton.Cancel
        )
        self.button_box.button(QtWidgets.QDialogButtonBox.StandardButton.Ok).setText(
            "确定"
        )
        self.button_box.button(
            QtWidgets.QDialogButtonBox.StandardButton.Cancel
        ).setText("取消")
        self.button_box.accepted.connect(self.accept)
        self.button_box.rejected.connect(self.reject)

        layout = QtWidgets.QVBoxLayout()
        layout.addLayout(self.form_layout)
        layout.addWidget(self.button_box)

        self.setLayout(layout)

    def get_settings(self):
        baud_rate = int(self.baud_rate_combo.currentText())
        data_bits = int(self.data_bits_combo.currentText())
        parity = self.parity_combo.currentText()
        stop_bits = float(self.stop_bits_combo.currentText())
        return baud_rate, data_bits, parity, stop_bits


class SerialThread(QThread):
    data_received = Signal(QByteArray)

    def __init__(self, serial_port):
        super().__init__()
        self.serial_port = serial_port
        self.serial_port.readyRead.connect(self.run)

    def run(self):
        data = self.serial_port.readAll()
        self.data_received.emit(data)


class SerialManager(QtWidgets.QWidget):
    serial_changed = Signal()
    is_serial_open = Signal()
    received = Signal(QByteArray)

    def __init__(self):
        super().__init__()

        self.port = QSerialPort()
        self.port.errorOccurred.connect(self.handle_error)

        self.label = QtWidgets.QLabel("串口：")
        self.port_combo = QtWidgets.QComboBox()
        self.open = QtWidgets.QPushButton("打开")
        self.open.clicked.connect(self.toggle_serial_port)
        self.refresh_button = QtWidgets.QPushButton("刷新")
        self.refresh_button.clicked.connect(self.update_serial_ports)
        self.settings_button = QtWidgets.QPushButton("设置")
        self.settings_button.clicked.connect(self.open_settings)
        self.status = QtWidgets.QLabel(r"串口状态：无连接")

        # 设置串口默认值
        self.port.setBaudRate(115200)
        self.port.setDataBits(QSerialPort.DataBits.Data8)
        self.port.setParity(QSerialPort.Parity.OddParity)
        self.port.setStopBits(QSerialPort.StopBits.OneStop)

        self.layout = QtWidgets.QGridLayout()
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.setSpacing(3)
        self.layout.addWidget(self.label, 0, 0)
        self.layout.addWidget(self.port_combo, 0, 1, 1, 2)
        self.layout.addWidget(self.open, 0, 3)
        self.layout.addWidget(self.refresh_button, 0, 4)
        self.layout.addWidget(self.settings_button, 0, 5)
        self.layout.addWidget(self.status, 1, 0, 1, 4)

        self.update_serial_ports()

        # self.port.readyRead.connect(self.read)

        # self.timer = QTimer(self)
        # self.timer.connect(self.timer, SIGNAL("timeout()"), self.read_serial_data)
        # self.timer.start(100)

        self.serial_thread = SerialThread(self.port)
        self.serial_thread.data_received.connect(lambda data: self.received.emit(data))

    # def read_serial_data(self):
    #     if self.port.isOpen():
    #         data = self.port.readAll()
    #         if data:
    #             self.received.emit(data)
    #
    # def read(self) -> None:
    #     data = self.port.readAll()
    #     self.received.emit(data)

    def write(self, data: QByteArray | bytes) -> None:
        self.port.write(data)

    def update_serial_ports(self):
        self.port_combo.clear()
        ports = QSerialPortInfo.availablePorts()
        for port in ports:
            self.port_combo.addItem(port.portName())

    def toggle_serial_port(self):
        if self.port.isOpen():
            self.close_serial_port()
        else:
            self.open_serial_port()

    def open_serial_port(self):
        if self.port.isOpen():
            return

        port_name = self.port_combo.currentText()
        self.port.setPortName(port_name)

        if self.port.open(QIODeviceBase.OpenModeFlag.ReadWrite):
            self.update_status_label(f"串口状态：{port_name}已连接")
            self.update_open_button_text("关闭")
            self.serial_thread.start()
            self.serial_changed.emit()
            self.is_serial_open.emit()
        else:
            self.update_status_label(f"串口状态：{port_name}连接失败")

    def close_serial_port(self):
        if self.port.isOpen():
            self.port.close()
            self.update_status_label("串口状态：无连接")
            self.update_open_button_text("打开")
            self.serial_thread.terminate()
            self.serial_thread.wait()

    def update_status_label(self, text):
        self.status.setText(text)

    def update_open_button_text(self, text):
        self.open.setText(text)

    def handle_error(self, error):
        if error == QSerialPort.SerialPortError.NoError:
            return
        error_message = self.port.errorString()
        self.status.setText(f"串口错误: {error_message}")

    def open_settings(self):
        dialog = SerialSettings()
        data_bits_map = {
            5: QSerialPort.DataBits.Data5,
            6: QSerialPort.DataBits.Data6,
            7: QSerialPort.DataBits.Data7,
            8: QSerialPort.DataBits.Data8,
        }

        stop_bits_map = {
            1: QSerialPort.StopBits.OneStop,
            1.5: QSerialPort.StopBits.OneAndHalfStop,
            2: QSerialPort.StopBits.TwoStop,
        }

        if dialog.exec_() == QtWidgets.QDialog.DialogCode.Accepted:
            baud_rate, data_bits, parity, stop_bits = dialog.get_settings()
            self.port.setBaudRate(baud_rate)
            self.port.setDataBits(data_bits_map[data_bits])
            if parity == "None":
                self.port.setParity(QSerialPort.Parity.NoParity)
            elif parity == "Even":
                self.port.setParity(QSerialPort.Parity.EvenParity)
            elif parity == "Odd":
                self.port.setParity(QSerialPort.Parity.OddParity)
            self.port.setStopBits(stop_bits_map[stop_bits])

            self.serial_changed.emit()


class SerialHelper(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()

        self.serial = SerialManager()
        self.recorder = Recorder()
        self.receiver = Receiver()
        self.configer = Config(self)

        self.layout = QtWidgets.QGridLayout()

        # 数据格式化
        self.config = GroupBox_1(r"指令配置区", self.configer.ui, self)
        self.layout.addWidget(self.config, 0, 0, 4, 1)

        # 数据收发显示区
        self.display = GroupBox_1(r"数据收发显示区", self.receiver.ui, self)
        self.layout.addWidget(self.display, 0, 1, 3, 2)

        # 数据发送
        self.transformer = Transformer()
        self.transform = GroupBox_1(r"数据发送", self.transformer, self)
        self.layout.addWidget(self.transform, 3, 1, 1, 2)

        # 信号处理
        self.serial.received.connect(self.receiver.recv_display)
        self.receiver.received_content.connect(self.recorder.record)
        self.transformer.transformed.connect(self.serial.write)
        self.transformer.transformed_content.connect(self.receiver.display)
        self.transformer.transformed_content.connect(self.recorder.record)
        self.configer.command.connect(self.serial.write)
        self.configer.command.connect(self.receiver.display)

        self.layout.setColumnStretch(0, 1)
        self.layout.setColumnStretch(1, 1)
        self.layout.setColumnStretch(2, 1)
        # self.layout.setRowStretch(0, 1)
        # self.layout.setRowStretch(1, 1)

        self.setLayout(self.layout)
        
    def save(self):
        self.recorder.save()
