from PySide6.QtWidgets import QApplication

# noinspection PyUnresolvedReferences
from resource_rc import *

from qt_material import apply_stylesheet

from src.master import Master

app = QApplication()

master = Master(app)

# 主题设置
apply_stylesheet(app, theme="light_blue.xml")

master.show()

app.exec()
