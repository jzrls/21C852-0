from typing import Callable

from PySide6 import QtWidgets
from PySide6.QtCore import Signal, QObject, QTimer, SIGNAL, Qt
from PySide6.QtGui import QDoubleValidator, QIntValidator, QValidator

from src.components.LineEdit import LineEdit
from src.components.lcd import Lcd
from src.components.switch import Switch
from src.components.transformer import Transformer
from src.protocol.crc import Crc
from src.protocol.dual_redundancy_control import ControlInstruction, TelemetryRequestInstruction, SelfTestInstruction, \
    CommonInstruction, Operator
from src.protocol.instruction import Counter


class Console(QObject):
    on_run = Signal(bytes)
    on_stop = Signal()

    def __init__(
        self,
        prompt: str,
        set_data: Callable[[int], None],
        pack: Callable[[], bytes],
        is_int: bool = True,
        validator: QValidator = None,
        has_run: bool = True,
        has_stop: bool = True
    ) -> None:
        super().__init__()

        self.layout = QtWidgets.QHBoxLayout()
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.setSpacing(3)

        self._input = LineEdit(prompt, '')
        self.layout.addWidget(self._input, 2)

        if validator:
            self._input.set_validator(validator)

        if has_run:
            self._run = QtWidgets.QPushButton("运行")
            self._run.clicked.connect(self.run)
            self.layout.addWidget(self._run, 1)

        if has_stop:
            self._stop = QtWidgets.QPushButton('停止')
            self._stop.clicked.connect(self.stop)
            self.layout.addWidget(self._stop, 1)

        self.set_data = set_data
        self.pack = pack
        self.is_int = is_int

    def set_text(self, text: str):
        self._input.set_text(text)

    def get_text(self):
        return self._input.get_text()

    def run(self):
        info = self._input.get_text()

        try:
            if self.is_int:
                data = int(info)
            else:
                data = float(info)
        except ValueError:
            return

        self.set_data(data)
        self.on_run.emit(self.pack())

    def stop(self):
        self.on_stop.emit()


class OpenConsole(QObject):
    on_run = Signal(list)
    on_stop = Signal()

    def __init__(self):
        super().__init__()

        self._voltage = LineEdit('电压幅值', '0')
        self._freq = LineEdit('频率', '0')
        self._dc_voltage = LineEdit('直流电压', '0')
        self.open_run = QtWidgets.QPushButton('开环运行')
        self.release_rotor = QtWidgets.QPushButton('释放转子')

        self.layout = QtWidgets.QGridLayout()
        self.set_ui()
        self.binding()

    def set_ui(self):
        self.layout.addWidget(self._voltage, 1, 0, 1, 2)
        self.layout.addWidget(self._freq, 2, 0, 1, 2)
        self.layout.addWidget(self._dc_voltage, 3, 0, 1, 2)
        self.layout.addWidget(self.open_run, 4, 0, 1, 1)
        self.layout.addWidget(self.release_rotor, 4, 1, 1, 1)

    def binding(self):
        self.open_run.clicked.connect(self.on_open_run_clicked)
        self.release_rotor.clicked.connect(self.on_release_rotor_clicked)

    def on_open_run_clicked(self):
        _voltage = float(self._voltage.get_text()) if self._voltage.get_text() else 0
        _freq = float(self._freq.get_text()) if self._freq.get_text() else 0
        _dc_voltage = float(self._dc_voltage.get_text()) if self._dc_voltage.get_text() else 0
        self.on_run.emit([_voltage, _freq, _dc_voltage])

    def on_release_rotor_clicked(self):
        self.on_stop.emit()


class LockZero(QObject):
    def __init__(self, counter: Counter, transformer: Transformer) -> None:
        super().__init__()

        self.counter = counter
        self.transformer = transformer

        self._set = QtWidgets.QLabel('设定值')
        self._set.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self._read_value = QtWidgets.QLabel('读取值')
        self._read_value.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)

        self._A_phase_voltage = LineEdit('A相电压', '0')
        self._B_phase_voltage = LineEdit('B相电压', '0')
        self._C_phase_voltage = LineEdit('C相电压', '0')
        self._dc_voltage = LineEdit('直流电压', '0')

        self._A_phase_voltage_display = Lcd('A相电流', '0')
        self._B_phase_voltage_display = Lcd('B相电流', '0')
        self._C_phase_voltage_display = Lcd('C相电流', '0')
        self._dc_voltage_display = Lcd('旋变读数', '0')

        self._locked_to_zero = QtWidgets.QPushButton('锁定对零')
        self._read = QtWidgets.QPushButton('读取旋变值')
        self._locked_to_zero.clicked.connect(self.on_lock_clicked)
        self._read.clicked.connect(self.on_read_clicked)

        self.set_cmd = CommonInstruction(self.counter, Crc.check_table, Operator.LOCK)
        self.read_cmd = CommonInstruction(self.counter, Crc.check_table, Operator.READ_CURRENT_ROTATOR)

        self.layout = QtWidgets.QGridLayout()
        self.set_ui()

    def set_ui(self) -> None:
        self.layout.addWidget(self._set, 0, 0, 1, 1)
        self.layout.addWidget(self._read_value, 0, 1, 1, 1)
        self.layout.addWidget(self._A_phase_voltage, 1, 0, 1, 1)
        self.layout.addWidget(self._B_phase_voltage, 2, 0, 1, 1)
        self.layout.addWidget(self._C_phase_voltage, 3, 0, 1, 1)
        self.layout.addWidget(self._dc_voltage, 4, 0, 1, 1)

        self.layout.addWidget(self._A_phase_voltage_display, 1, 1, 1, 1)
        self.layout.addWidget(self._B_phase_voltage_display, 2, 1, 1, 1)
        self.layout.addWidget(self._C_phase_voltage_display, 3, 1, 1, 1)
        self.layout.addWidget(self._dc_voltage_display, 4, 1, 1, 1)

        self.layout.addWidget(self._locked_to_zero, 5, 0, 1, 1)
        self.layout.addWidget(self._read, 5, 1, 1, 1)

        self.layout.setColumnStretch(0, 1)
        self.layout.setColumnStretch(1, 1)

    def on_lock_clicked(self) -> None:
        _A_phase_voltage = float(self._A_phase_voltage.get_text()) if self._A_phase_voltage.get_text() else 0
        _B_phase_voltage = float(self._B_phase_voltage.get_text()) if self._B_phase_voltage.get_text() else 0
        _C_phase_voltage = float(self._C_phase_voltage.get_text()) if self._C_phase_voltage.get_text() else 0
        dc_voltage = float(self._dc_voltage.get_text()) if self._dc_voltage.get_text() else 0
        self.set_cmd.lock_zero(_A_phase_voltage, _B_phase_voltage, _C_phase_voltage, dc_voltage)
        self.transformer.write_port(self.set_cmd.pack())

    def on_read_clicked(self):
        self.read_cmd.read_current_rotator()
        self.transformer.write_port(self.read_cmd.pack())


class CommonControlConsole(QObject):

    def __init__(self, counter: Counter, transformer: Transformer):
        super().__init__()

        self.counter = counter
        self.transformer = transformer

        self.layout = QtWidgets.QVBoxLayout()

        # 需要使用到的指令
        self.control = CommonInstruction(self.counter, Crc.check_table, Operator.CONTROL)

        # 控制台
        self.speed_console = Console('速度控制', self.control.set_speed, self.control.pack, is_int=True,
                                     validator=QIntValidator(0, 8000)
                                     )
        self.position_console = Console('位置控制', self.control.set_position, self.control.pack, is_int=False,
                                        validator=QDoubleValidator(0, 360, 3)
                                        )
        self.torque_console = Console('力矩控制', self.control.set_torque, self.control.pack, is_int=False,
                                      validator=QDoubleValidator(0, 300, 3)
                                      )
        self.open_console = OpenConsole()

        self.set_ui()
        self.binding()

    def set_ui(self):
        controls = QtWidgets.QTabWidget()
        controls.addTab(self._console(self.speed_console), '速度控制')
        controls.addTab(self._console(self.position_console), '位置控制')
        controls.addTab(self._console(self.torque_console), '力矩控制')
        controls.addTab(self._console(self.open_console), '开环控制')
        self.layout.addWidget(controls)

    def binding(self):
        # 转速/位置/力矩控制
        self.speed_console.on_run.connect(self.transformer.write_port)
        self.position_console.on_run.connect(self.transformer.write_port)
        self.torque_console.on_run.connect(self.transformer.write_port)

        # 停止电机运行指令
        self.speed_console.on_stop.connect(self.on_stop_motor)
        self.position_console.on_stop.connect(self.on_stop_motor)
        self.torque_console.on_stop.connect(self.on_stop_motor)

        # 开环控制
        self.open_console.on_run.connect(self.on_open_run)
        self.open_console.on_stop.connect(self.on_open_stop)

    @staticmethod
    def _console(console: Console | OpenConsole):
        widget = QtWidgets.QWidget()
        widget.setLayout(console.layout)
        return widget

    def on_stop_motor(self) -> None:
        cmd = CommonInstruction(self.counter, Crc.check_table, Operator.STOP_MOTOR)
        cmd.stop_motor()
        self.transformer.write_port(cmd.pack())

    def on_open_run(self, datas: list) -> None:
        cmd = CommonInstruction(self.counter, Crc.check_table, Operator.OPEN_CONTROL)
        cmd.open_control(datas)
        self.transformer.write_port(cmd.pack())

    def on_open_stop(self) -> None:
        cmd = CommonInstruction(self.counter, Crc.check_table, Operator.STOP_LOCK_OR_OPEN_CONTROL)
        cmd.stop_lock_or_open_control()
        self.transformer.write_port(cmd.pack())


class DualControlConsole(QObject):
    def __init__(self, counter: Counter, transformer: Transformer) -> None:
        super().__init__()

        self.transformer = transformer

        # 需要操作的指令
        self.self_test = SelfTestInstruction(counter, Crc.check_table)
        self.control = ControlInstruction(counter, Crc.check_table)
        self.telemetry = TelemetryRequestInstruction(counter, Crc.check_table)

        # 启停按钮
        self.valve1_switch = Switch("蝶阀1 ")
        self.valve2_switch = Switch("蝶阀2 ")
        self.pump_switch = Switch("电动泵")

        # 控制台
        self.valve1_console = Console('蝶阀 1 角度设置', self.control.set_valve1_angle, self.control.pack, is_int=False,
                                      validator=QDoubleValidator(0, 360, 2), has_stop=False
                                      )
        self.valve2_console = Console('蝶阀 2 角度设置', self.control.set_valve2_angle, self.control.pack, is_int=False,
                                      validator=QDoubleValidator(0, 360, 2), has_stop=False
                                      )
        self.pump_console = Console('电动泵转速设置', self.control.set_pump_speed, self.control.pack,
                                    validator=QIntValidator(0, 8000), has_stop=False
                                    )

        self._single_telemetry = QtWidgets.QPushButton('单次遥测')
        self._loop_telemetry = QtWidgets.QPushButton('循环遥测')
        self._stop_telemetry = QtWidgets.QPushButton('遥测停止')
        self.check = QtWidgets.QPushButton('自检指令')

        # 遥测指令
        self.timer = QTimer()
        self.timer.connect(self.timer, SIGNAL("timeout()"), self.single_telemetry)

        self.layout = QtWidgets.QVBoxLayout()
        self.set_ui()
        self.binding()

    def set_ui(self):
        # 布局
        switches = QtWidgets.QHBoxLayout()
        switches.addWidget(self.valve1_switch)
        switches.addWidget(self.valve2_switch)
        switches.addWidget(self.pump_switch)

        funcs = QtWidgets.QHBoxLayout()
        funcs.addWidget(self._single_telemetry)
        funcs.addWidget(self._loop_telemetry)
        funcs.addWidget(self._stop_telemetry)
        funcs.addWidget(self.check)

        self.layout.addLayout(switches)
        self.layout.addLayout(self.valve1_console.layout)
        self.layout.addLayout(self.valve2_console.layout)
        self.layout.addLayout(self.pump_console.layout)
        self.layout.addLayout(funcs)

    def binding(self):
        # 启动/停止指令
        self.valve1_switch.is_open.connect(self.start_valve1)
        self.valve2_switch.is_open.connect(self.start_valve2)
        self.pump_switch.is_open.connect(self.start_pump)

        # 角度/转速指令
        self.valve1_console.on_run.connect(self.transformer.write_port)
        self.valve2_console.on_run.connect(self.transformer.write_port)
        self.pump_console.on_run.connect(self.transformer.write_port)

        # 自检/控制/遥测指令
        self._single_telemetry.clicked.connect(self.single_telemetry)
        self._loop_telemetry.clicked.connect(self.start_telemetry)
        self._stop_telemetry.clicked.connect(self.stop_telemetry)
        self.check.clicked.connect(self.start_self_test)

    def start_self_test(self):
        cmd = self.self_test.pack()
        self.transformer.write_port(cmd)

    def start_telemetry(self):
        self.timer.start(100)

    def single_telemetry(self):
        cmd = self.telemetry.pack()
        self.transformer.write_port(cmd)

    def stop_telemetry(self):
        self.timer.stop()

    def start_valve1(self) -> None:
        self.control.start_valve1()
        self.transformer.write_port(self.control.pack())
        try:
            self.valve1_switch.is_open.disconnect(self.start_valve1)
            self.valve1_switch.is_open.connect(self.stop_valve1)
        except RuntimeError:
            pass

    def stop_valve1(self) -> None:
        self.control.stop_valve1()
        self.transformer.write_port(self.control.pack())
        try:
            self.valve1_switch.is_open.disconnect(self.stop_valve1)
            self.valve1_switch.is_open.connect(self.start_valve1)
        except RuntimeError:
            pass

    def start_valve2(self) -> None:
        self.control.start_valve2()
        self.transformer.write_port(self.control.pack())
        try:
            self.valve2_switch.is_open.disconnect(self.start_valve2)
            self.valve2_switch.is_open.connect(self.stop_valve2)
        except RuntimeError:
            pass

    def stop_valve2(self) -> None:
        self.control.stop_valve2()
        self.transformer.write_port(self.control.pack())
        try:
            self.valve2_switch.is_open.disconnect(self.stop_valve2)
            self.valve2_switch.is_open.connect(self.start_valve2)
        except RuntimeError:
            pass

    def start_pump(self) -> None:
        self.control.start_pump()
        self.transformer.write_port(self.control.pack())
        try:
            self.pump_switch.is_open.disconnect(self.start_pump)
            self.pump_switch.is_open.connect(self.stop_pump)
        except RuntimeError:
            pass

    def stop_pump(self) -> None:
        self.control.stop_pump()
        self.transformer.write_port(self.control.pack())
        try:
            self.pump_switch.is_open.disconnect(self.stop_pump)
            self.pump_switch.is_open.connect(self.start_pump)
        except RuntimeError:
            pass
