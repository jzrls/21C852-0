from typing import List

import numpy as np
import pyqtgraph as pg
from PySide6 import QtWidgets
from PySide6.QtCore import Qt, Signal, QThread, QTimer
from pyqtgraph import mkPen

from src.protocol.dual_redundancy_control import (
    TelemetryResponseInstruction,
    Dual,
)

colors = (
    "#B0C4DE",
    "#FF00FF",
    "#000000",
    "#FA8072",
    "#EEE8AA",
    "#FF1493",
    "#7B68EE",
    "#FFC0CB",
    "#00ff7f",
    "#556B2F",
    "#CD853F",
    "#000080",
    "#32CD32",
    "#7F007F",
    "#B03060",
    "#800000",
    "#483D8B",
    "#008000",
    "#3CB371",
    "#008B8B",
    "#FF0000",
    "#FF8C00",
    "#FFD700",
    "#00FF00",
    "#9400D3",
    "#00FA9A",
    "#DC143C",
    "#00FFFF",
    "#8A2BE2",
    "#0000FF",
    "#ADFF2F",
    "#DA70D6",
    "#F0F0F0",
    "#F0F0FF",
)


class PlotWidget(pg.PlotWidget):
    def __init__(self, items, parent=None):
        super().__init__(parent)

        pg.setConfigOptions(leftButtonPan=True, antialias=True)
        self.setBackground("w")
        self.showGrid(x=True, y=True, alpha=0.8)
        self.getPlotItem().hideButtons()

        # 图例设置
        self.legend = self.addLegend(colCount=3)
        self.legend.anchor((1, 0), (1, 0))
        self.legend.setVisible(False)

        # 待绘制的曲线集合
        self.curves = {}

        # 添加需要绘制的曲线
        self.items = items
        for item, color in zip(self.items, colors):
            self.add(item, color)

        # 自动化 x 轴坐标
        self.x_range = 0
        self.setXRange(0, 1000)

    def add(self, name: str, color: str) -> None:
        curve = pg.PlotCurveItem(pen=mkPen(color=color, width=2), name=name)
        curve.setVisible(False)
        self.addItem(curve)
        self.curves[name] = {"curve": curve, "data": np.array([])}

    def set_display(self) -> None:
        self.legend.setVisible(not self.legend.isVisible())

    def set_default_display(self, index: int = 0) -> None:
        # 默认显示第一个曲线
        self.curves.get(self.items[index])["curve"].setVisible(True)
        self.selection_changed(self.items[index])

    def selection_changed(self, button: str) -> None:
        for key, value in self.curves.items():
            if key == button:
                value["curve"].setVisible(True)
            else:
                value["curve"].setVisible(False)

    def clear_plot(self) -> None:
        for _, curve in self.curves.items():
            self.x_range = 0
            self.enableAutoRange()
            curve["data"] = np.array([])
            curve["curve"].clear()

    def plot(self, datas: np.ndarray | list | int | float) -> None:
        for data, curve in zip(datas, self.curves.values()):
            curve["data"] = np.append(curve["data"], data)

        for _, curve in self.curves.items():
            curve["curve"].setData(curve["data"])

            # X 轴显示范围设置
        if self.x_range >= 1000:
            self.setXRange(self.x_range - 1000, self.x_range + 1)

        self.x_range += 1


class UpdateThread(QThread):
    data_updated = Signal(object)

    def __init__(self):
        super().__init__()
        self.datas = None
        self.timer = QTimer()
        self.timer.timeout.connect(self.run)
        self.timer.start(10)

    def restart(self):
        self.timer.start(10)

    def stop(self):
        self.timer.stop()

    def run(self):
        if self.datas:
            datas = (np.array(self.datas[Dual.PARAMS_INDEX_MIN:Dual.PARAMS_INDEX_MAX]) * Dual.SCALING_FACTOR_MIN)
            datas[Dual.PUMP_SET_SPEED_INDEX] *= Dual.SCALING_FACTOR_MAX
            datas[Dual.PUMP_REAL_TIME_SPEED_INDEX] *= Dual.SCALING_FACTOR_MAX
            datas = np.append(datas, self.datas[Dual.FRAME_COUNTER_INDEX])

            self.data_updated.emit(datas)


class Waveform(QtWidgets.QWidget):
    def __init__(self, the_curve: int) -> None:
        super().__init__()

        self.layout = QtWidgets.QGridLayout(self)
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.setSpacing(3)

        # 绘图区
        self.figure = PlotWidget(Dual.TELEMETRY_INFO)
        self.figure.set_default_display(the_curve)

        # 功能区
        self.set_legend = QtWidgets.QPushButton("曲线设置")
        self.set_legend.clicked.connect(self.figure.set_display)
        self.reset = QtWidgets.QPushButton("重置绘图")
        self.reset.clicked.connect(lambda: self.figure.enableAutoRange())
        self.clear = QtWidgets.QPushButton("清除绘图")
        self.clear.clicked.connect(self.figure.clear_plot)
        self.stop_update = QtWidgets.QPushButton("停止更新")
        self.rerun_update = QtWidgets.QPushButton("重新更新")

        # 布局设置
        self.layout.addWidget(self.stop_update, 0, 5, 1, 1)
        self.layout.addWidget(self.rerun_update, 0, 6, 1, 1)
        self.layout.addWidget(self.reset, 0, 7, 1, 1)
        self.layout.addWidget(self.clear, 0, 9, 1, 1)
        self.layout.addWidget(self.set_legend, 0, 8, 1, 1)
        self.layout.addWidget(self.figure, 1, 0, 5, 10)

        self.update_thread = UpdateThread()
        self.update_thread.data_updated.connect(self.figure.plot)
        self.update_thread.start()

        self.stop_update.clicked.connect(self.update_thread.stop)
        self.rerun_update.clicked.connect(self.update_thread.restart)

    def update_plot(self, instruction: TelemetryResponseInstruction) -> None:
        if isinstance(instruction, TelemetryResponseInstruction):
            self.update_thread.datas = instruction.res


class CommonWaveform(QtWidgets.QWidget):
    def __init__(self) -> None:
        super().__init__()

        self.layout = QtWidgets.QGridLayout(self)

        # 绘图区
        self.items = (
            "设定转速",
            "转速",
            "设定Udc",
            "Udc",
            "设定Id",
            "Id",
            "设定Iq",
            "Iq",
            "Id_PI",
            "Iq_PI",
            "Ud",
            "Uq",
            "电机温度1",
            "电机温度2",
            "驱动板温度",
            "Idc",
        )
        self.figure = PlotWidget(self.items)

        # 布局设置
        self.layout.addWidget(self.figure, 1, 0)

        # 自动化 x 轴坐标
        self.x_range = 0

    def update_plot(self, instruction: TelemetryResponseInstruction) -> None:
        pass

    def clear_plot(self):
        self.figure.clear()


class TimingWaveform(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()

        self.layout = QtWidgets.QGridLayout(self)

        # 绘图区
        self.items = ["启停标志", "反馈启停标志", "控制设定值", "控制回传值", "遥测设定值", "遥测实际值"]
        self.figure = PlotWidget(self.items)
        self.figure.legend.setColumnCount(1)
        self.figure.legend.setVisible(True)

        # 功能区
        self.combox = QtWidgets.QComboBox()
        self.combox.addItems(["蝶阀1", "蝶阀2", "电动泵"])
        self.combox.setCurrentText("电动泵")
        self.combox.currentTextChanged.connect(self.on_combox_changed)

        self.file_name_label = QtWidgets.QLabel("文件名：")
        self.file_name_label.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.file_name = QtWidgets.QLineEdit()
        self.file_name.setPlaceholderText("Test")
        self.speed_line = QtWidgets.QLineEdit()
        self.speed_line.setPlaceholderText("2000")
        self.low_angle_line = QtWidgets.QLineEdit()
        self.low_angle_line.setPlaceholderText("5")
        self.high_angle_line = QtWidgets.QLineEdit()
        self.high_angle_line.setPlaceholderText("355")

        self.start = QtWidgets.QPushButton("开始")
        self.stop = QtWidgets.QPushButton("停止")
        self.clear = QtWidgets.QPushButton("清除绘图")
        self.clear.clicked.connect(self.figure.clear_plot)

        # 布局设置
        self.angle_set = QtWidgets.QLabel("角度范围设置:")
        self.angle_set.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.layout.addWidget(self.angle_set, 0, 3, 1, 1)
        self.layout.addWidget(self.low_angle_line, 0, 4, 1, 1)
        self.sp = QtWidgets.QLabel("---")
        self.sp.setAlignment(Qt.AlignCenter)
        self.layout.addWidget(self.sp, 0, 5, 1, 1)
        self.layout.addWidget(self.high_angle_line, 0, 6, 1, 1)

        self.speed_set = QtWidgets.QLabel("转速设定：")
        self.speed_set.setAlignment(Qt.AlignRight | Qt.AlignVCenter)
        self.layout.addWidget(self.speed_set, 0, 3, 1, 1)
        self.layout.addWidget(self.speed_line, 0, 4, 1, 1)
        self.rpm = QtWidgets.QLabel("rpm")
        self.layout.addWidget(self.rpm, 0, 5, 1, 1)

        self.angle_set.setVisible(False)
        self.low_angle_line.setVisible(False)
        self.sp.setVisible(False)
        self.high_angle_line.setVisible(False)
        self.speed_set.setVisible(True)
        self.speed_line.setVisible(True)
        self.rpm.setVisible(True)

        self.layout.addWidget(self.combox, 0, 0, 1, 1)
        self.layout.addWidget(self.file_name_label, 0, 1, 1, 1)
        self.layout.addWidget(self.file_name, 0, 2, 1, 1)
        self.layout.addWidget(self.start, 0, 7, 1, 1)
        self.layout.addWidget(self.stop, 0, 8, 1, 1)
        self.layout.addWidget(self.clear, 0, 9, 1, 1)
        self.layout.addWidget(self.figure, 1, 0, 5, 10)

        for i in range(10):
            self.layout.setColumnStretch(i, 1)

    def on_combox_changed(self, text):
        if text == "蝶阀1" or text == "蝶阀2":
            self.speed_set.setVisible(False)
            self.speed_line.setVisible(False)
            self.rpm.setVisible(False)
            self.angle_set.setVisible(True)
            self.low_angle_line.setVisible(True)
            self.sp.setVisible(True)
            self.high_angle_line.setVisible(True)
        else:
            self.angle_set.setVisible(False)
            self.low_angle_line.setVisible(False)
            self.sp.setVisible(False)
            self.high_angle_line.setVisible(False)
            self.speed_set.setVisible(True)
            self.speed_line.setVisible(True)
            self.rpm.setVisible(True)

    def update_plot(self, instruction: List[float]) -> None:
        self.figure.plot(instruction)
