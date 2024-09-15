from PySide6.QtCore import Qt, QRect, Signal
from PySide6.QtGui import QPainter, QColor, QFont, QPen, QBrush
from PySide6.QtWidgets import QApplication, QWidget


class Switch(QWidget):
    """自定义Switch按钮"""
    is_open = Signal(bool)

    def __init__(self, prompt: str, margin=3, height=32, parent=None):
        super().__init__(parent)

        self.prompt = prompt
        self.margin = margin

        self.primary_color = '#2979FF'
        self.inactive_color = '#FFFFFF'

        # 提示文字长度
        # self.setMinimumWidth(self.length + 60)
        self.setFixedHeight(height)
        self.setMinimumWidth(4 * self.height() + 4 * margin)

        # 设置无边框和背景透明
        self.setAttribute(Qt.WA_TranslucentBackground)

        self.state = False  # 按钮状态：True表示开，False表示关

    def mousePressEvent(self, event):
        """鼠标点击事件：用于切换按钮状态"""
        super().mousePressEvent(event)

        self.state = False if self.state else True
        self.update()
        self.is_open.emit(self.state)

    def _background(self, painter, brush, color):
        painter.setPen(Qt.NoPen)
        brush.setColor(color)
        painter.setBrush(brush)
        painter.drawRoundedRect(0, 0, self.width(), self.height(), 5, 5)

    def _block(self, painter, length, position, pen=Qt.NoPen, radius=5):
        painter.setPen(pen)

        painter.drawRoundedRect(position, self.margin, length, length, radius, radius)

    def _text(self, painter, color, length, status='开启'):
        # 绘制文本
        painter.setPen(QPen(QColor(color)))
        painter.setBrush(Qt.NoBrush)
        x = length + self.margin
        _length = self.width() - 2 * (length + self.margin)
        text_rect = QRect(x, 0, _length, self.height())
        painter.drawText(text_rect, Qt.AlignCenter, f'{self.prompt}{status}')

    def paintEvent(self, event):
        """绘制按钮"""
        super().paintEvent(event)

        # 创建绘制器并设置抗锯齿和图片流畅转换
        painter = QPainter(self)
        painter.setRenderHints(QPainter.Antialiasing | QPainter.SmoothPixmapTransform)

        # 定义字体样式
        font = QFont('Microsoft YaHei')
        font.setPixelSize(14)
        painter.setFont(font)

        length = self.height() - 2 * self.margin
        # brush = QBrush()

        # 开关为开的状态
        if self.state:
            # 绘制背景
            painter.setPen(Qt.NoPen)
            brush = QBrush(QColor('#2979ff'))
            painter.setBrush(brush)
            painter.drawRoundedRect(0, 0, self.width(), self.height(), 5, 5)

            # 绘制滑块
            brush.setColor(QColor('#FFFFFF'))
            painter.setBrush(brush)
            position = self.width() - self.margin - length
            self._block(painter, length, position)

            # 绘制文本
            self._text(painter, self.inactive_color, length, '开启')
        else:  # 开关为关的状态
            # 绘制背景
            painter.setPen(Qt.NoPen)
            brush = QBrush(QColor('#FFFFFF'))
            painter.setBrush(brush)
            painter.drawRoundedRect(0, 0, self.width(), self.height(), 5, 5)

            # 绘制滑块
            pen = QPen(QColor('#999999'))
            pen.setWidth(1)
            self._block(painter, length, position=self.margin, pen=pen)

            # 绘制文本
            self._text(painter, '#ACACAC', length, '关闭')


if __name__ == "__main__":
    app = QApplication([])
    window = Switch('开关')
    window.show()
    app.exec()
