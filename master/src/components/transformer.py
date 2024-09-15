from PySide6.QtCore import Signal, QTimer, SIGNAL
from PySide6.QtWidgets import QWidget, QComboBox, QLabel, QCheckBox, QSpinBox, QPushButton, QTextEdit, QGridLayout, \
    QMessageBox


class Transformer(QWidget):
    transformed = Signal(bytes)
    transformed_content = Signal(str)

    def __init__(self) -> None:
        super().__init__()

        self.label = QLabel(r'文本格式：')
        self.format_combo = QComboBox(self)
        self.format_combo.addItem("HEX")
        self.format_combo.addItem("ASCII")

        self.auto_send = QCheckBox("自动发送", self)
        self.auto_send.stateChanged.connect(self.toggle_auto_send)
        self.auto_send_state = False
        self.auto_send_timer = QTimer(self)
        self.interval = None

        self.interval_spinbox = QSpinBox(self)
        self.interval_spinbox.setEnabled(False)
        self.interval_spinbox.setMinimum(0)
        self.interval_spinbox.setMaximum(10000)
        self.interval_spinbox.setValue(1000)

        self.send_button = QPushButton("发送", self)
        self.send_button.clicked.connect(self.send_text)

        self.text = QTextEdit(self)

        self.layout = QGridLayout()
        self.layout.addWidget(self.label, 0, 0)
        self.layout.addWidget(self.format_combo, 0, 1, 1, 1)
        self.layout.addWidget(self.auto_send, 0, 2)
        self.layout.addWidget(self.interval_spinbox, 0, 3)
        self.layout.addWidget(self.send_button, 0, 4)
        self.layout.addWidget(self.text, 1, 0, 5, 5)

        self.setLayout(self.layout)

    def toggle_auto_send(self, state) -> None:
        if state == 2:
            self.auto_send_state = True
            self.interval_spinbox.setEnabled(True)
        else:
            self.auto_send_state = False
            self.interval_spinbox.setEnabled(False)

    def send_text(self) -> None:
        text = self.get_text()

        if self.auto_send_state:
            self.interval = self.interval_spinbox.value()
            self.auto_send_timer.start(self.interval)
            self.auto_send_timer.connect(self.auto_send_timer, SIGNAL("timeout()"), self.auto_send_timeout)
            self.send_button.setText("停止")
            self.send_button.clicked.disconnect()
            self.send_button.clicked.connect(self.stop_send)
        else:
            self.write_port(text)

    def write_port(self, text: bytes) -> None:
        try:
            self.transformed_content.emit(f'Tx: {text.hex(" ").upper()}')
            self.transformed.emit(text)
        except Exception as e:
            print(f"发送数据时出现异常：{str(e)}")
            QMessageBox.critical(self, '异常', '十六进制发送错误')
            self.stop_send()
            return

    def auto_send_timeout(self):
        text = self.get_text()
        self.write_port(text)

    def get_text(self) -> bytes | None:
        text = self.text.toPlainText()
        if not text:
            return

        text_type = self.format_combo.currentText()
        if text_type == r'HEX':
            text = bytes.fromhex(text.replace(" ", ""))
        elif text_type == r'ASCII':
            text = text.encode()

        return text

    def stop_send(self):
        self.send_button.setText("发送")
        self.send_button.clicked.disconnect()
        self.send_button.clicked.connect(self.send_text)
        self.auto_send_timer.stop()
