from PySide6.QtCore import QByteArray, Signal, QObject

from src.protocol.crc import Crc
from src.protocol.dual_redundancy_control import SelfTestResponseInstruction, ControlResponseInstruction, \
    TelemetryResponseInstruction, CommonInstruction, Response
from src.protocol.instruction import Counter


class Resolver(QObject):
    received_self = Signal(SelfTestResponseInstruction)
    received_control = Signal(ControlResponseInstruction)
    received_telemetry = Signal(TelemetryResponseInstruction)
    buffer = Signal(TelemetryResponseInstruction)

    def __init__(self, counter: Counter):
        super().__init__()

        self.counter = counter

        self._buffer = bytes()
        self.data_length = 0
        self.crc = bytes()
        self.obj = None
        self.signal = None
        self.received = 0

        self.state = "idle"
        self.states = {
            "idle": self.handle_idle,
            "header1": self.handle_header1,
            "header2": self.handle_header2,
            "self_test": self.handle_self_test,
            "ctrl_response": self.handle_ctrl_response,
            "tele_response": self.handle_tele_response,
            "receiving": self.handle_receiving,
            "crc": self.handle_crc,
            "connect_controller": self.handle_connect_controller,
            "clear": self.handle_clear,
            "open_control": self.handle_open_control,
            "write_motor_params": self.handle_write_motor_params,
            "read_motor_params": self.handle_read_motor_params,
            "control": self.handle_control,
            "read_motor_sensors": self.handle_read_motor_sensors,
            "stop_motor": self.handle_stop_motor,
            "write_control_params": self.handle_write_control_params,
            "read_control_params": self.handle_read_control_params,
            "lock": self.handle_lock,
            "unlock": self.handle_unlock,
            "write_analog_params": self.handle_write_analog_params,
            "read_analog_params": self.handle_read_analog_params,
            "write_other_params": self.handle_write_other_params,
            "read_other_params": self.handle_read_other_params,
            "write_eeprom": self.handle_write_eeprom
        }

    def parse(self, data: QByteArray) -> None:
        for byte in data:
            self.states[self.state](byte)

    def initialize(self) -> None:
        self._buffer = bytes()
        self.crc = bytes()
        self.received = 0
        self.data_length = 0
        self.obj = None
        self.signal = None

    def handle_idle(self, byte) -> None:
        self.initialize()
        if byte == Response.HEADER1 or byte == Response.COM_HEADER1:
            self._buffer += byte
            self.state = "header1"

    def handle_header1(self, byte) -> None:
        if byte == Response.HEADER2 or byte == Response.COM_HEADER2:
            self._buffer += byte
            self.state = "header2"
        else:
            self.state = "idle"

    def _handle_header2(self, byte: bytes, state: str) -> None:
        self._buffer += byte
        self.state = state

    def handle_header2(self, byte) -> None:
        if byte == Response.SELF_TEST:
            self._handle_header2(byte, "self_test")
        elif byte == Response.CTRL_RESPONSE:
            self._handle_header2(byte, "ctrl_response")
        elif byte == Response.TELE_RESPONSE:
            self._handle_header2(byte, "tele_response")
        elif byte == Response.INITIALIZE:
            self._handle_header2(byte, "connect_controller")
        elif byte == Response.CLEAR:
            self._handle_header2(byte, "clear")
        elif byte == Response.OPEN_CONTROL:
            self._handle_header2(byte, "open_control")
        elif byte == Response.WRITE_MOTOR_PARAMS:
            self._handle_header2(byte, "write_motor_params")
        elif byte == Response.READ_MOTOR_PARAMS:
            self._handle_header2(byte, "read_motor_params")
        elif byte == Response.CONTROL:
            self._handle_header2(byte, "control")
        elif byte == Response.READ_MOTOR_SENSOR:
            self._handle_header2(byte, "read_motor_sensors")
        elif byte == Response.STOP_MOTOR:
            self._handle_header2(byte, "stop_motor")
        elif byte == Response.WRITE_CONTROL_PARAMS:
            self._handle_header2(byte, "write_control_params")
        elif byte == Response.READ_CONTROL_PARAMS:
            self._handle_header2(byte, "read_control_params")
        elif byte == Response.LOCK:
            self._handle_header2(byte, "lock")
        elif byte == Response.STOP_LOCK_OR_OPEN_CONTROL:
            self._handle_header2(byte, "unlock")
        elif byte == Response.WRITE_ANALOG_PARAMS:
            self._handle_header2(byte, "write_analog_params")
        elif byte == Response.READ_ANALOG_PARAMS:
            self._handle_header2(byte, "read_analog_params")
        elif byte == Response.WRITE_OTHER_PARAMS:
            self._handle_header2(byte, "read_other_params")
        elif byte == Response.READ_OTHER_PARAMS:
            self._handle_header2(byte, "read_other_params")
        elif byte == Response.WRITE_EEPROM:
            self._handle_header2(byte, "write_eeprom")
        else:
            self.state = "idle"

    def handle_self_test(self, byte) -> None:
        self._buffer += byte  # 帧计数
        self.obj = SelfTestResponseInstruction(self.counter, Crc.check_table)
        self.signal = self.received_self
        self.state = "receiving"

    def handle_ctrl_response(self, byte) -> None:
        self._buffer += byte  # 帧计数
        self.obj = ControlResponseInstruction(self.counter, Crc.check_table)
        self.signal = self.received_control
        self.state = "receiving"

    def handle_tele_response(self, byte) -> None:
        self._buffer += byte  # 帧计数
        self.obj = TelemetryResponseInstruction(self.counter, Crc.check_table)
        self.signal = self.received_telemetry
        self.state = "receiving"

    def handle_receiving(self, byte) -> None:
        if self.data_length == 0:
            self.data_length = int.from_bytes(byte, byteorder="little", signed=False)
        self._buffer += byte
        if self.received == self.data_length:
            self.state = "crc"
        self.received += 1

    def handle_crc(self, byte) -> None:
        self.crc += byte
        if len(self.crc) == 2:
            self.state = "idle"
            crc = Crc.check_table(self._buffer)
            if self.crc == crc:
                self._buffer += self.crc
                self.obj.unpack(self._buffer)
                self.signal.emit(self.obj)
                # self.buffer.emit(f"Rx: {self._buffer.hex(' ').upper()}")
                self.buffer.emit(self.obj)

    def _instruction(self, byte, response: Response) -> None:
        self._buffer += byte  # 帧计数
        self.obj = CommonInstruction(self.counter, Crc.check_table, response)
        self.signal = self.received_control
        self.state = "receiving"

    def handle_connect_controller(self, byte) -> None:
        self._instruction(byte, Response.INITIALIZE)

    def handle_clear(self, byte) -> None:
        self._instruction(byte, Response.CLEAR)

    def handle_open_control(self, byte) -> None:
        self._instruction(byte, Response.OPEN_CONTROL)

    def handle_write_motor_params(self, byte) -> None:
        self._instruction(byte, Response.WRITE_MOTOR_PARAMS)

    def handle_read_motor_params(self, byte) -> None:
        self._instruction(byte, Response.READ_MOTOR_PARAMS)

    def handle_control(self, byte) -> None:
        self._instruction(byte, Response.CONTROL)

    def handle_read_motor_sensors(self, byte) -> None:
        self._instruction(byte, Response.READ_MOTOR_SENSOR)

    def handle_stop_motor(self, byte) -> None:
        self._instruction(byte, Response.STOP_MOTOR)

    def handle_write_control_params(self, byte):
        self._instruction(byte, Response.WRITE_CONTROL_PARAMS)

    def handle_read_control_params(self, byte) -> None:
        self._instruction(byte, Response.READ_CONTROL_PARAMS)

    def handle_lock(self, byte) -> None:
        self._instruction(byte, Response.LOCK)

    def handle_unlock(self, byte) -> None:
        self._instruction(byte, Response.STOP_LOCK_OR_OPEN_CONTROL)

    def handle_write_analog_params(self, byte) -> None:
        self._instruction(byte, Response.WRITE_ANALOG_PARAMS)

    def handle_read_analog_params(self, byte) -> None:
        self._instruction(byte, Response.READ_ANALOG_PARAMS)

    def handle_write_other_params(self, byte) -> None:
        self._instruction(byte, Response.WRITE_OTHER_PARAMS)

    def handle_read_other_params(self, byte) -> None:
        self._instruction(byte, Response.READ_OTHER_PARAMS)

    def handle_write_eeprom(self, byte) -> None:
        self._instruction(byte, Response.WRITE_EEPROM)


class Resolver2:
    def __init__(self):
        pass

    def parse(self, data: QByteArray):
        pass

    def check_crc(self):
        pass
