from PySide6 import QtWidgets
from PySide6.QtCore import Qt, QObject
from PySide6.QtGui import QPainter, QColor
from PySide6.QtWidgets import QWidget, QHBoxLayout, QGridLayout, QVBoxLayout

from src.protocol.dual_redundancy_control import SelfTestResponseInstruction, TelemetryResponseInstruction, Dual
from src.tools.utils import get_bits


class Light(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setFixedSize(20, 20)
        self._status = "Inactive"

    def set_normal(self):
        self._status = r'Normal'
        self.update()

    def set_abnormal(self):
        self._status = r'Abnormal'
        self.update()

    def set_inactive(self):
        self._status = r'Inactive'
        self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)

        colors = {
            "Normal": QColor('#1DCC00'),  # Green
            "Inactive": QColor('#CECECE'),  # Orange
            "Abnormal": QColor('#CC0022'),  # Red
        }

        painter.setBrush(colors[self._status])
        painter.drawRoundedRect(0, 0, self.width(), self.height(), 5, 5)


class IndicatorLight(QWidget):
    def __init__(self, prompt: str) -> None:
        super().__init__()

        # 创建Qt主窗口部件
        layout = QHBoxLayout(self)

        # 创建指示灯和标签
        self.light = Light()
        self.prompt = QtWidgets.QLabel(prompt)
        self.prompt.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)

        # 添加控件到布局
        layout.addWidget(self.light)
        layout.addWidget(self.prompt)

    def set_normal(self):
        self.light.set_normal()

    def set_abnormal(self):
        self.light.set_abnormal()

    def set_inactive(self):
        self.light.set_inactive()


class Monitor(QObject):

    def __init__(self):
        super().__init__()

        self.prompts = [
            '阀1主驱动', '阀2主驱动', '泵主驱动',
            '阀1副驱动', '阀2副驱动', '泵副驱动',
            '阀1过压', '阀2过压', '泵 过 压',
            '阀1欠压', '阀2欠压', '泵 欠 压',
            '阀1过流', '阀2过流', '泵 过 流',
            '阀1硬件保护', '阀2硬件保护', '泵硬件保护',
            '阀1软件限位', '阀2软件限位', '阀1旋变故障',
            '阀2旋变故障', '阀旋变故障', '泵旋变故障',
            '阀1使能', '阀2使能', '泵使能'
        ]

        self.layout = QGridLayout()
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.setSpacing(1)

        self.items = []
        for i, prompt in enumerate(self.prompts):
            item = IndicatorLight(prompt)
            self.layout.addWidget(item, i // 3, i % 3)
            self.items.append(item)

    def refresh(self, instruction: TelemetryResponseInstruction):
        if isinstance(instruction, TelemetryResponseInstruction):
            driver_status = instruction.res[Dual.DRIVER_STATUS_INDEX]

            # 驱动状态监控
            for i in range(3):
                bits = get_bits(driver_status, 2 * i, 2 * i + 1)
                if bits == 1:
                    self.items[i].set_normal()
                    self.items[i + 3].set_inactive()
                elif bits == 2:
                    self.items[i].set_inactive()
                    self.items[i + 3].set_normal()
                else:
                    self.items[i].set_inactive()
                    self.items[i + 3].set_inactive()

            # 故障状态字处理
            for i, item in enumerate(self.items[6:]):
                if instruction.is_normal(i):
                    item.set_normal()
                else:
                    item.set_abnormal()


class FaultStatusMonitor(QObject):

    def __init__(self):
        super().__init__()

        self.prompts = [
            '电流超限', 'd轴电流环积分限幅', '泵主驱动',
            '速度超限', '硬件保护', 'q轴电流环积分限幅',
            '电压总限幅', '直流过压', '旋变故障(8位)',
            '电机过温', '模块过温', '直流欠压',
            '通信故障'
        ]

        self.layout = QGridLayout()

        self.items = []
        for i, prompt in enumerate(self.prompts):
            item = IndicatorLight(prompt)
            self.layout.addWidget(item, i // 7, i % 7)
            self.items.append(item)

    def refresh(self, instruction: TelemetryResponseInstruction):
        if isinstance(instruction, TelemetryResponseInstruction):
            pass


class SelfTestResponseStatus(QWidget):

    def __init__(self) -> None:
        super().__init__()

        self.valve1_main_cpu = IndicatorLight('阀1主CPU')
        self.valve1_aux_cpu = IndicatorLight('阀1辅CPU')
        self.valve2_main_cpu = IndicatorLight('阀2主CPU')
        self.valve2_aux_cpu = IndicatorLight('阀2辅CPU')
        self.pump_main_cpu = IndicatorLight('泵主CPU')
        self.pump_aux_cpu = IndicatorLight('泵辅CPU')

        self.power_supply_28V = IndicatorLight('28V供电')
        self.power_supply_270V = IndicatorLight('270V供电')

        self.layout = QVBoxLayout()
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.setSpacing(1)
        self.layout.addWidget(self.valve1_main_cpu, )
        self.layout.addWidget(self.valve1_aux_cpu, )
        self.layout.addWidget(self.valve2_main_cpu, )
        self.layout.addWidget(self.valve2_aux_cpu, )
        self.layout.addWidget(self.pump_main_cpu, )
        self.layout.addWidget(self.pump_aux_cpu, )

        self.layout.addWidget(self.power_supply_28V, )
        self.layout.addWidget(self.power_supply_270V, )

        self.layout.setContentsMargins(10, 0, 10, 0)
        # self.setLayout(layout)

    def update_light_state(self, instruction: SelfTestResponseInstruction):
        if isinstance(instruction, SelfTestResponseInstruction):
            if not instruction.is_valve1_main_cpu_normal():
                self.valve1_main_cpu.set_abnormal()
            else:
                self.valve1_main_cpu.set_normal()

            if not instruction.is_valve1_aux_cpu_normal():
                self.valve1_aux_cpu.set_abnormal()
            else:
                self.valve1_aux_cpu.set_normal()

            if not instruction.is_valve2_main_cpu_normal():
                self.valve2_main_cpu.set_abnormal()
            else:
                self.valve2_main_cpu.set_normal()

            if not instruction.is_valve2_aux_cpu_normal():
                self.valve2_aux_cpu.set_abnormal()
            else:
                self.valve2_aux_cpu.set_normal()

            if not instruction.is_pump_main_cpu_normal():
                self.pump_main_cpu.set_abnormal()
            else:
                self.pump_main_cpu.set_normal()

            if not instruction.is_pump_aux_cpu_normal():
                self.pump_aux_cpu.set_abnormal()
            else:
                self.pump_aux_cpu.set_normal()

            if not instruction.is_supply_28v1_normal() and instruction.is_supply_28v2_normal():
                self.power_supply_28V.set_abnormal()
            else:
                self.power_supply_28V.set_normal()

            if not instruction.is_supply_270v1_normal() and instruction.is_supply_270v2_normal():
                self.power_supply_270V.set_abnormal()
            else:
                self.power_supply_270V.set_normal()
