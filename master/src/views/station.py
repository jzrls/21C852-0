from PySide6.QtWidgets import QWidget, QFrame, QHBoxLayout, QVBoxLayout, QSizePolicy

from src.components.GroupBox import GroupBox
from src.components.console import DualControlConsole
from src.components.monitor import Monitor, SelfTestResponseStatus
from src.components.watcher import Watcher
from src.components.waveform import Waveform
from src.protocol.instruction import Counter
from src.protocol.resolver import Resolver
from src.views.serial import SerialHelper


class WorkStation(QWidget):
    def __init__(self, serial_helper: SerialHelper):
        super().__init__()

        self.serial_helper = serial_helper

        self.counter = Counter()  # 全局帧计数
        self.resolver = Resolver(self.counter)

        self.watcher = GroupBox(r"数据监测", Watcher())  # 数据检测
        self.serial = GroupBox(r"串口设置", self.serial_helper.serial)  # 串口设置
        self.console = GroupBox(r"控制台", DualControlConsole(self.counter, self.serial_helper.transformer))  # 控制台
        self.waveform1 = GroupBox(r"数据曲线", Waveform(5))  # 波形 1, 默认：泵实时转速
        # self.waveform2 = GroupBox(r"数据曲线2", Waveform(3))  # 波形 2，默认：泵实时转速
        # self.waveform3 = GroupBox(r"数据曲线3", Waveform(5))  # 波形 3，默认：蝶阀2实时角度
        self.monitor = GroupBox(r"遥测信号", Monitor())  # 故障检测

        # 自检响应
        # self_test_group = QGroupBox('自检响应')
        self.self_test_group = QFrame()
        self.self_test = SelfTestResponseStatus()
        self.self_test_group.setLayout(self.self_test.layout)

        self.set_ui()
        self.binding()

    def set_ui(self):
        layout = QHBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)
        layout.setSpacing(3)

        left_layout = QVBoxLayout()
        left_layout.setContentsMargins(0, 0, 0, 0)
        left_layout.setSpacing(3)
        left_layout.addWidget(self.serial)
        left_layout.addWidget(self.console)
        left_layout.addWidget(self.monitor)

        right_layout = QVBoxLayout()
        right_layout.setContentsMargins(0, 0, 0, 0)
        right_layout.setSpacing(3)
        right_layout.addWidget(self.watcher)
        right_layout.addWidget(self.waveform1)

        layout.addLayout(left_layout, 1)
        layout.addLayout(right_layout, 3)
        layout.addWidget(self.self_test_group)

        self.monitor.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)

        self.watcher.setFixedHeight(350)
        self.serial.setFixedHeight(120)
        self.console.setFixedHeight(280)
        self.waveform1.setMinimumHeight(400)

        self.setLayout(layout)

    def binding(self):
        # 开串口自检
        self.serial_helper.serial.is_serial_open.connect(self.console.target.start_self_test)

        # 开串口遥测
        # self.serial_helper.serial.is_serial_open.connect(self.console.start_telemetry)

        # 数据接收解析
        self.serial_helper.serial.received.connect(self.resolver.parse)

        # 数据接收更新
        self.resolver.received_self.connect(self.self_test.update_light_state)
        self.resolver.received_telemetry.connect(self.watcher.target.refresh)
        self.resolver.received_telemetry.connect(self.monitor.target.refresh)
        self.resolver.received_telemetry.connect(self.waveform1.target.update_plot)
        # self.resolver.received_telemetry.connect(self.waveform2.target.update_plot)
        # self.resolver.received_telemetry.connect(self.waveform3.target.update_plot)

        # 日志
        self.resolver.buffer.connect(self.serial_helper.recorder.record)
