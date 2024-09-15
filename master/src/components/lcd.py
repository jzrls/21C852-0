from PySide6 import QtWidgets
from PySide6.QtCore import Qt


class Lcd(QtWidgets.QWidget):
    def __init__(self, label, value):
        super().__init__()

        layout = QtWidgets.QHBoxLayout(self)

        # 创建标签和LCD显示控件
        self.label = QtWidgets.QLabel()
        self.label.setTextFormat(Qt.TextFormat.MarkdownText)
        self.label.setAlignment(Qt.AlignmentFlag.AlignRight | Qt.AlignmentFlag.AlignVCenter)
        self.label.setText(label)

        self.lcd = QtWidgets.QLCDNumber()
        self.lcd.setFixedWidth(92)
        self.lcd.setSegmentStyle(QtWidgets.QLCDNumber.SegmentStyle.Flat)

        # 添加控件到布局
        layout.addWidget(self.label, 1)
        layout.addWidget(self.lcd, 2)

        # 更新LCD显示控件的数据
        self.refresh(value)

    def refresh(self, value):
        self.lcd.display(value)


class LabelLcd(object):
    def __init__(self):
        pass
