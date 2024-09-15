import numpy as np
from PySide6.QtWidgets import QGridLayout

from src.components.lcd import Lcd
from src.protocol.dual_redundancy_control import TelemetryResponseInstruction, SelfTestInstruction, \
    SelfTestResponseInstruction, ControlInstruction, ControlResponseInstruction, TelemetryRequestInstruction, Dual
from src.protocol.instruction import Instruction


class Watcher:
    """
    Watcher：数据观测器
    """

    def __init__(self) -> None:
        super().__init__()

        self.prompts = Dual.TELEMETRY_INFO[:-1]

        self.orders = [0, 2, 4, 1, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 18, 21, 16, 19, 22, 17, 20, 23]

        self.layout = QGridLayout()
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.setSpacing(1)

        self.items = []
        for i, index in enumerate(self.orders):
            lcd = Lcd(self.prompts[index], 0)
            self.items.append(lcd)
            self.layout.addWidget(self.items[i], i // 4, i % 4)

    def refresh(self, instruction: TelemetryResponseInstruction) -> None:
        if isinstance(instruction, TelemetryResponseInstruction):
            # 对解析的数据按需要的顺序进行重排
            datas = np.array(instruction.res[Dual.PARAMS_INDEX_MIN:Dual.PARAMS_INDEX_MAX])[self.orders]

            for item, value in zip(self.items, datas):
                if item == self.items[2] or item == self.items[5]:
                    # 泵电机转速不需要乘定标系数
                    item.refresh(value)
                else:
                    # 其他参数需要乘定标系数 0.01
                    item.refresh(value * Dual.SCALING_FACTOR_MIN)


class SingleBoardWatcher:
    """
    Watcher：数据观测器
    """

    def __init__(self) -> None:
        super().__init__()

        self.prompts = (
            '设定转速', '转速', '设定Udc', 'Udc',
            '设定Id', 'Id', '设定Iq', 'Iq',
            'Id_PI', 'Iq_PI', 'Ud', 'Uq',
            '电机温度1', '电机温度2', '驱动板温度', 'Idc'
        )

        self.layout = QGridLayout()
        # self.layout.setHorizontalSpacing(1)

        self.items = []
        for i, prompt in enumerate(self.prompts):
            self.items.append(Lcd(prompt, 0))
            self.layout.addWidget(self.items[i], i // 4, i % 4)

    def refresh(self, instruction: TelemetryResponseInstruction) -> None:
        if isinstance(instruction, TelemetryResponseInstruction):
            # 对解析的数据按需要的顺序进行重排
            pass


class InstructionWatcher:
    """
    InstructionWatcher：指令观测器
    """

    def __init__(self) -> None:
        super().__init__()

        self.prompts = [
            '自检指令', '控制指令', '遥测指令',
            '自检响应', '控制响应', '遥测响应'
        ]

        self.layout = QGridLayout()

        self.items = []
        for i, prompt in enumerate(self.prompts):
            self.items.append(Lcd(prompt, 0))
            self.layout.addWidget(self.items[i], i // 3, i % 3)

        # 计数器
        self.self_test_count = 0
        self.self_response_count = 0
        self.control_count = 0
        self.control_response_count = 0
        self.telemetry_count = 0
        self.telemetry_response_count = 0

    def refresh(self, instruction: Instruction) -> None:
        if isinstance(instruction, SelfTestInstruction):
            self.self_test_count += 1
            self.items[0].refresh(self.self_test_count)
        elif isinstance(instruction, SelfTestResponseInstruction):
            self.self_response_count += 1
            self.items[1].refresh(self.self_response_count)
        elif isinstance(instruction, ControlInstruction):
            self.control_count += 1
            self.items[2].refresh(self.control_count)
        elif isinstance(instruction, ControlResponseInstruction):
            self.control_response_count += 1
            self.items[3].refresh(self.control_response_count)
        elif isinstance(instruction, TelemetryRequestInstruction):
            self.telemetry_count += 1
            self.items[4].refresh(self.telemetry_count)
        elif isinstance(instruction, TelemetryResponseInstruction):
            self.telemetry_response_count += 1
            self.items[5].refresh(self.telemetry_response_count)
