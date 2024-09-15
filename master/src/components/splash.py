import time
from time import sleep

from PySide6 import QtWidgets, QtCore
from PySide6.QtCore import QCoreApplication, SIGNAL
from PySide6.QtCore import Qt, QTimer
from PySide6.QtGui import QPixmap, QFont
from PySide6.QtWidgets import QWidget, QProgressBar, QLabel, QFrame, QVBoxLayout


class SplashScreen(QWidget):
    def __init__(self, master):
        super().__init__()

        self.master = master

        self.setFixedSize(self.master.size())

        self.setWindowFlag(Qt.FramelessWindowHint)
        self.setAttribute(Qt.WA_TranslucentBackground)

        self.counter = 0
        self.n = 300  # total instance

        self.frame = QFrame()
        self.label_title = QLabel(self.frame)
        self.label_description = QLabel(self.frame)
        self.label_description.resize(self.width() - 10, 50)
        self.label_description.move(0, self.label_title.height())
        self.progress_bar = QProgressBar(self.frame)
        self.label_loading = QLabel(self.frame)
        self.init_ui()

        self.timer = QTimer()
        self.timer.connect(self.timer, SIGNAL('timeout()'), self.loading)
        self.timer.start(20)

    def init_ui(self):
        layout = QVBoxLayout()
        self.setLayout(layout)

        layout.addWidget(self.frame)

        self.label_title.setObjectName('LabelTitle')
        self.label_title.setStyleSheet(
            '''
            LabelTitle {
                font - size: 60px;
                color:  # 93deed;
            }
            '''
        )

        # center labels
        self.label_title.resize(self.width() - 10, 150)
        self.label_title.move(0, 40)  # x, y
        self.label_title.setText('21C852-0')
        self.label_title.setAlignment(Qt.AlignCenter)

        self.label_description.setObjectName('LabelDesc')
        self.label_description.setText('<strong>Motor Master</strong>')
        self.label_description.setAlignment(Qt.AlignCenter)

        self.progress_bar.resize(self.width() - 200 - 10, 50)
        self.progress_bar.move(100, self.label_description.y() + 130)
        self.progress_bar.setAlignment(Qt.AlignCenter)
        self.progress_bar.setFormat('%p%')
        self.progress_bar.setTextVisible(True)
        self.progress_bar.setRange(0, self.n)
        self.progress_bar.setValue(20)

        self.label_loading.resize(self.width() - 10, 50)
        self.label_loading.move(0, self.progress_bar.y() + 70)
        self.label_loading.setObjectName('LabelLoading')
        self.label_loading.setAlignment(Qt.AlignCenter)
        self.label_loading.setText('loading...')

    def loading(self):
        self.progress_bar.setValue(self.counter)

        if self.counter >= self.n:
            self.timer.stop()
            self.close()

            time.sleep(1)

            self.master.show()

        self.counter += 1


class MySplashScreen(QtWidgets.QSplashScreen):
    # 鼠标点击事件
    def mousePressEvent(self, event):
        pass


# 主界面
class MyWindow(QtWidgets.QMainWindow):
    # 初始化MenuDemo子类
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Demo")
        # 宽×高
        self.resize(800, 600)
        # 最小窗口尺寸
        # self.setMinimumSize(600, 500)
        self.btn = QtWidgets.QPushButton('关闭窗口')
        self.btn.clicked.connect(self.fun_Exit)
        self.setCentralWidget(self.btn)

    def load_data(self, sp):
        for i in range(1, 11):  # 模拟主程序加载过程
            sleep(1)  # 加载数据
            sp.showMessage("加载... {0}%".format(i * 10), QtCore.Qt.AlignHCenter | QtCore.Qt.AlignBottom,
                           QtCore.Qt.black)
            QCoreApplication.processEvents()  # 允许主进程处理事件

    # 退出菜单响应
    def fun_Exit(self):
        response_quit = QtWidgets.QApplication.instance()
        response_quit.quit()


if __name__ == "__main__":
    app = QtWidgets.QApplication([])
    # 设置启动界面
    splash = MySplashScreen()
    # 初始图片
    pixmap = QPixmap(r'C:\Users\lisheng\Desktop\CODE\pymaster\asset\preview1.jpg')
    pixmap = pixmap.scaled(800, 600)
    splash.setPixmap(pixmap)  # 设置背景图片
    splash.resize(800, 600)

    # 初始文本
    splash.showMessage("加载... 0%", QtCore.Qt.AlignHCenter | QtCore.Qt.AlignBottom, QtCore.Qt.black)
    # 设置字体
    splash.setFont(QFont('微软雅黑', 10))
    # 显示启动界面
    splash.show()
    app.processEvents()  # 处理主进程事件
    # 主窗口
    window = MyWindow()
    window.load_data(splash)  # 加载数据
    window.show()
    splash.finish(window)  # 隐藏启动界面
    splash.deleteLater()
    app.exec()
