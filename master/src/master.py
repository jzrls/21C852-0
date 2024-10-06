from PySide6.QtWidgets import QMainWindow, QWidget, QGridLayout

from src.views.serial import SerialHelper
from src.views.station import WorkStation


class Master(QMainWindow):
    """
    Master Window
    主窗口设置
    """

    def __init__(self, app):
        super().__init__()

        self.setWindowTitle("MOTORMASTER-21C852-0")

        self.resize(1450, 860)

        self.content = QWidget(self)
        self.layout = QGridLayout(self.content)

        self.serial_helper = SerialHelper()
        self.master = WorkStation(self.serial_helper)

        self.layout.addWidget(self.master)

        self.setCentralWidget(self.content)

    def save(self):
        self.serial_helper.save()
