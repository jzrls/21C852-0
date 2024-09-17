from PySide6.QtGui import QIcon
from PySide6.QtWidgets import QApplication
from qt_material import apply_stylesheet

# noinspection PyUnresolvedReferences
import resource_rc
from src.master import Master

app = QApplication()

app.setWindowIcon(QIcon(rf":/asset/favicon.ico"))
master = Master(app)
apply_stylesheet(app, theme="light_blue.xml")
master.show()

app.exec()
