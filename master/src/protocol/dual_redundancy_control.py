from src.protocol.instruction import Instruction
from src.tools.utils import get_bit, get_bits


# TELEMETRY_INFO = (
#     '阀1-设定角度', '阀1-实际角度', '阀2-设定角度', '阀2-实际角度',
#     '泵-设定转速', '泵-实际转速',
#     '阀1-温度', '阀2-温度', '泵-温度',
#     '阀1-母线电压', '阀2-母线电压', '泵-母线电压',
#     '阀1-母线电流', '阀2-母线电流', '泵-母线电流',
#     '阀1-A相电流', '阀1-B相电流', '阀1-C相电流',
#     '阀2-A相电流', '阀2-B相电流', '阀2-C相电流',
#     '泵-A相电流', '泵-B相电流', '泵-C相电流', '帧计数'
# )


def set_angle(angle: float) -> float:
    angle = 0 if angle <= 0 else angle
    angle = 360 if angle >= 360 else angle
    return angle


class Dual:
    TELEMETRY_INFO = (
        '阀1-设定角度', '阀1-实际角度', '阀2-设定角度', '阀2-实际角度',
        '泵-设定转速', '泵-实际转速',
        '阀1-温度', '阀2-温度', '泵-温度',
        '阀1-母线电压', '阀2-母线电压', '泵-母线电压',
        '阀1-母线电流', '阀2-母线电流', '泵-母线电流',
        '阀1-A相电流', '阀1-B相电流', '阀1-C相电流',
        '阀2-A相电流', '阀2-B相电流', '阀2-C相电流',
        '泵-A相电流', '泵-B相电流', '泵-C相电流', '帧计数'
    )

    # 泵设定/实时转速索引, 针对 TELEMETRY_INFO
    PUMP_SET_SPEED_INDEX = 4
    PUMP_REAL_TIME_SPEED_INDEX = 5

    # 定标系数
    SCALING_FACTOR_MIN = 0.01
    SCALING_FACTOR_MAX = 100

    # 帧计数索引，针对解包数据
    FRAME_COUNTER_INDEX = 3

    # 驱动状态位索引，针对解包数据
    DRIVER_STATUS_INDEX = 5

    # 运行参数索引范围，针对解包数据
    PARAMS_INDEX_MIN = 6
    PARAMS_INDEX_MAX = -2  # 数组原则左开右闭，实际索引位置+1 = -2

    # 故障状态字索引，针对解包数据
    FAULT_STATUS_WORD_INDEX = -2


class CPU:
    """
    CPU 编号
    """
    VALVE1_MAIN_CPU = 0
    VALVE1_AUX_CPU = 1
    PUMP_MAIN_CPU = 2
    PUMP_AUX_CPU = 3
    VALVE2_MAIN_CPU = 4
    VALVE2_AUX_CPU = 5
    UNKNOWN_CPU = -1


class Operator:
    """
    操作指令，单板控制指令类型
    """
    INITIALIZE = 0x00  # 连接CPU{0， 1， 2， 3， 4， 5}
    CLEAR = 0x01  # 清除故障信息
    READ_CURRENT_ROTATOR = 0x02
    OPEN_CONTROL = 0x03
    WRITE_MOTOR_PARAMS = 0x04
    READ_MOTOR_PARAMS = 0x05
    CONTROL = 0x08
    READ_MOTOR_SENSOR = 0x09
    STOP_MOTOR = 0x0A
    WRITE_CONTROL_PARAMS = 0x0B
    READ_CONTROL_PARAMS = 0x0C
    LOCK = 0x0D
    STOP_LOCK_OR_OPEN_CONTROL = 0x0E
    WRITE_ANALOG_PARAMS = 0x10
    READ_ANALOG_PARAMS = 0x11
    WRITE_OTHER_PARAMS = 0x12
    READ_OTHER_PARAMS = 0x13
    WRITE_EEPROM = 0x60


class Response:
    """
    响应指令常数、响应指令类型
    """
    # Master 总体控制常量
    HEADER1 = b'\xEB'
    HEADER2 = b'\x90'
    SELF_TEST = b'\x50'
    CTRL_RESPONSE = b'\x51'
    TELE_RESPONSE = b'\x52'

    # 单板控制常量
    COM_HEADER1 = b'\xAA'
    COM_HEADER2 = b'\x55'

    # 单板控制，响应指令类型
    INITIALIZE = 0x70
    CLEAR = 0x71
    READ_CURRENT_ROTATOR = 0x72
    OPEN_CONTROL = 0x73
    WRITE_MOTOR_PARAMS = 0x74
    READ_MOTOR_PARAMS = 0x75
    CONTROL = 0x78
    READ_MOTOR_SENSOR = 0x79
    STOP_MOTOR = 0x7A
    WRITE_CONTROL_PARAMS = 0x7B
    READ_CONTROL_PARAMS = 0x7C
    LOCK = 0x7D
    STOP_LOCK_OR_OPEN_CONTROL = 0x7E
    WRITE_ANALOG_PARAMS = 0x80
    READ_ANALOG_PARAMS = 0x81
    WRITE_OTHER_PARAMS = 0x82
    READ_OTHER_PARAMS = 0x33
    WRITE_EEPROM = 0xD0


class SelfTestInstruction(Instruction):
    """
    帧头1: 0xEB
    帧头2: 0x90
    帧类型: 0xA0
    帧计数: 0
    数据长度: 1
    数据: 0x01
    CRC: 0x0000
    """

    def __init__(self, frame_counter, crc):
        super().__init__(frame_counter, crc)

        common = (
            {"name": "frame_header1", "data": 0xEB, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_header2", "data": 0x90, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_type", "data": 0xA0, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_count", "data": 0, "bits": "uint8", "is_frame_count": True, "is_crc": False, },
            {"name": "data_length", "data": 1, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "data", "data": 0, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "crc", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": True, }
        )

        for item in common:
            self.add(item["name"], item["data"], item["bits"], item["is_frame_count"], item["is_crc"], )


class SelfTestResponseInstruction(Instruction):
    """
    帧头1: 0xEB
    帧头2: 0x90
    帧类型: 0x50
    帧计数: 0
    数据长度: 1
    数据: 0x01
    CRC: 0x0000
    """

    def __init__(self, frame_counter, crc):
        super().__init__(frame_counter, crc)

        common = (
            {"name": "frame_header1", "data": 0xEB, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_header2", "data": 0x90, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_type", "data": 0x50, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_count", "data": 0, "bits": "uint8", "is_frame_count": True, "is_crc": False, },
            {"name": "data_length", "data": 8, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "cpu_state", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve1_state", "data": 0, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "valve2_state", "data": 0, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "power_state", "data": 0, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "soft_version", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "copyright", "data": 0, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "crc", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": True, }
        )

        for item in common:
            self.add(item["name"], item["data"], item["bits"], item["is_frame_count"], item["is_crc"], )

        # print(common)

    def is_valve1_main_cpu_normal(self) -> bool:
        cpu = get_bits(self.res[5], 0, 1)
        return True if cpu == 1 else False

    def is_valve1_aux_cpu_normal(self) -> bool:
        cpu = get_bits(self.res[5], 2, 3)
        return True if cpu == 1 else False

    def is_valve2_main_cpu_normal(self) -> bool:
        cpu = get_bits(self.res[5], 4, 5)
        return True if cpu == 1 else False

    def is_valve2_aux_cpu_normal(self) -> bool:
        cpu = get_bits(self.res[5], 6, 7)
        return True if cpu == 1 else False

    def is_pump_main_cpu_normal(self) -> bool:
        cpu = get_bits(self.res[5], 8, 9)
        return True if cpu == 1 else False

    def is_pump_aux_cpu_normal(self) -> bool:
        cpu = get_bits(self.res[5], 10, 11)
        return True if cpu == 1 else False

    def is_valve1_normal(self) -> bool:
        status = get_bits(self.res[6], 0, 1)
        return True if status == 1 else False

    def is_valve2_normal(self) -> bool:
        status = get_bits(self.res[7], 0, 1)
        return True if status == 1 else False

    def is_supply_28v1_normal(self) -> bool:
        power = get_bits(self.res[8], 0, 1)
        return True if power == 1 else False

    def is_supply_28v2_normal(self) -> bool:
        power = get_bits(self.res[8], 2, 3)
        return True if power == 1 else False

    def is_supply_270v1_normal(self) -> bool:
        power = get_bits(self.res[8], 4, 5)
        return True if power == 1 else False

    def is_supply_270v2_normal(self) -> bool:
        power = get_bits(self.res[8], 6, 7)
        return True if power == 1 else False


class ControlInstruction(Instruction):
    LAUNCH = 0x10
    STOP = 0x11
    SET_VALVE1_ANGLE = 0x12
    SET_VALVE2_ANGLE = 0x13
    SET_PUMP_SPEED = 0x14

    RUN_VALVE1 = 2.312142466135948e-43
    RUN_VALVE2 = 2.382207389352189e-43
    RUN_PUMP = 1.1911036946760945e-43

    STOP_VALVE1 = 1.2611686178923354e-43
    STOP_VALVE2 = RUN_PUMP
    STOP_PUMP = RUN_VALVE1

    def __init__(self, frame_counter, crc):
        super().__init__(frame_counter, crc)

        common = (
            {"name": "frame_header1", "data": 0xEB, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_header2", "data": 0x90, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_type", "data": 0xA1, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_count", "data": 0, "bits": "uint8", "is_frame_count": True, "is_crc": False, },
            {"name": "data_length", "data": 5, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "operator", "data": 0x00, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "op_code", "data": 0x00, "bits": "float", "is_frame_count": False, "is_crc": False, },
            {"name": "crc", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": True, }
        )

        for item in common:
            self.add(item["name"], item["data"], item["bits"], item["is_frame_count"], item["is_crc"], )

    def _set_op(self, operator, op_code):
        self.items["operator"]["data"] = operator
        self.items["op_code"]["data"] = op_code

    def start_valve1(self) -> None:
        self._set_op(ControlInstruction.LAUNCH, ControlInstruction.RUN_VALVE1)

    def start_valve2(self) -> None:
        self._set_op(ControlInstruction.LAUNCH, ControlInstruction.RUN_VALVE2)

    def start_pump(self) -> None:
        self._set_op(ControlInstruction.LAUNCH, ControlInstruction.RUN_PUMP)

    def stop_valve1(self) -> None:
        self._set_op(ControlInstruction.STOP, ControlInstruction.STOP_VALVE1)

    def stop_valve2(self) -> None:
        self._set_op(ControlInstruction.STOP, ControlInstruction.STOP_VALVE2)

    def stop_pump(self) -> None:
        self._set_op(ControlInstruction.STOP, ControlInstruction.STOP_PUMP)

    def set_valve1_angle(self, angle: float) -> None:
        self._set_op(ControlInstruction.SET_VALVE1_ANGLE, set_angle(angle))

    def set_valve2_angle(self, angle: float) -> None:
        self._set_op(ControlInstruction.SET_VALVE2_ANGLE, set_angle(angle))

    def set_pump_speed(self, speed: float) -> None:
        speed = 0 if speed <= 0 else speed
        speed = 8000 if speed >= 8000 else speed
        self._set_op(ControlInstruction.SET_PUMP_SPEED, speed)


class ControlResponseInstruction(ControlInstruction):
    def __init__(self, frame_counter, crc):
        super().__init__(frame_counter, crc)
        self.items.update({'frame_type': {'data': 0x51, 'bits': 'uint8', 'is_frame_count': False, 'is_crc': False}})


class TelemetryRequestInstruction(SelfTestInstruction):
    def __init__(self, frame_counter, crc) -> None:
        super().__init__(frame_counter, crc)
        self.items.update({'frame_type': {'data': 0xA2, 'bits': 'uint8', 'is_frame_count': False, 'is_crc': False}})


class TelemetryResponseInstruction(Instruction):
    def __init__(self, frame_counter, crc) -> None:
        super().__init__(frame_counter, crc)

        common = (
            {"name": "frame_header1", "data": 0xEB, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_header2", "data": 0x90, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_type", "data": 0x52, "bits": "uint8", "is_frame_count": False, "is_crc": False, },
            {"name": "frame_count", "data": 0, "bits": "uint8", "is_frame_count": True, "is_crc": False, },
            {"name": "data_length", "data": 48, "bits": "uint8", "is_frame_count": True, "is_crc": False, },
            {"name": "driver_status", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve1_setting_angle", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve1_real_angle", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve2_setting_angle", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve2_real_angle", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "pump_setting_speed", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "pump_real_speed", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve1_temp", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve2_temp", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "pump_temp", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve1_voltage", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve2_voltage", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "pump_voltage", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve1_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve2_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "pump_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve1_a_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve1_b_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve1_c_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve2_a_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve2_b_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "valve2_c_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "pump_a_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "pump_b_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "pump_c_current", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": False, },
            {"name": "fault_status_word", "data": 0, "bits": "uint32", "is_frame_count": False, "is_crc": False, },
            {"name": "crc", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": True, }
        )

        for item in common:
            self.add(item["name"], item["data"], item["bits"], item["is_frame_count"], item["is_crc"], )

    def is_normal(self, bit_position: int) -> bool:
        # 故障状态字处理
        fault_status_word = self.res[-2]
        bit = get_bit(fault_status_word, bit_position)
        return True if bit == 1 else False


class CommonInstruction(Instruction):
    TORQUE_CONTROL = 0
    SPEED_CONTROL = 1
    POSITION_CONTROL = 2

    def __init__(self, frame_counter, crc, operator: Operator | Response) -> None:
        super().__init__(frame_counter, crc, model='big')  # 大端模式

        # 公共部分
        common = [
            {"name": "header1", "data": 0xAA, "bits": "uint8", "is_frame_count": False, "is_crc": False},
            {"name": "header2", "data": 0x55, "bits": "uint8", "is_frame_count": False, "is_crc": False}
        ]

        # 设置指令类型， 1字节
        self.operator = operator
        common.append(
            {"name": "frame_type", "data": self.operator, "bits": "uint8", "is_frame_count": False, "is_crc": False}
        )

        # 设置数据长度， 2字节
        data_length = self._get_data_length()
        common.append(
            {"name": "data_length", "data": data_length, "bits": "uint16", "is_frame_count": False, "is_crc": False}
        )

        # 判断该指令是否有校验码
        if self._has_no_crc():
            # 若无校验码，最后一位设置为指令码
            common.append(
                {"name": f"status_code", "data": 0, "bits": "uint8", "is_frame_count": False, "is_crc": False}
            )
        else:
            # 设置参数个数
            for i in range(1, data_length // 4 + 1):
                common.append(
                    {"name": f"param{i}", "data": 0, "bits": "uint32", "is_frame_count": False, "is_crc": False}
                )

            # 添加校验码位
            common.append(
                {"name": "crc", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": True}
            )

        # if has_crc and self.operator == Operator.INITIALIZE:
        #     common.append(
        #         {"name": "crc", "data": 0, "bits": "uint16", "is_frame_count": False, "is_crc": True}
        #     )
        # elif has_crc:
        #     pass
        # else:
        #     common.append(
        #         {"name": "crc", "data": 0, "bits": "uint8", "is_frame_count": False, "is_crc": True}
        #     )

        for item in common:
            self.add(item["name"], item["data"], item["bits"], item["is_frame_count"], item["is_crc"])

    def _get_data_length(self):
        _map = {
            Operator.CLEAR: 0x01,
            Operator.READ_CURRENT_ROTATOR: 0x01,
            Operator.READ_MOTOR_PARAMS: 0x01,
            Operator.READ_CONTROL_PARAMS: 0x01,
            Operator.READ_ANALOG_PARAMS: 0x01,
            Operator.READ_OTHER_PARAMS: 0x01,
            Operator.WRITE_EEPROM: 0x01,
            Operator.INITIALIZE: 0x04,
            Operator.READ_MOTOR_SENSOR: 0x05,
            Operator.STOP_MOTOR: 0x05,
            Operator.STOP_LOCK_OR_OPEN_CONTROL: 0x05,
            Operator.OPEN_CONTROL: 0x11,
            Operator.LOCK: 0x15,
            Operator.CONTROL: 0x21,
            Operator.WRITE_ANALOG_PARAMS: 0x6D,
            Operator.WRITE_MOTOR_PARAMS: 0x79,
            Operator.WRITE_CONTROL_PARAMS: 0x79,
            Operator.WRITE_OTHER_PARAMS: 0x79,
        }

        return _map.get(self.operator, 0)

    def _has_no_crc(self):
        _map = {
            Operator.CLEAR: True,
            Response.CLEAR: True,
            Operator.READ_CURRENT_ROTATOR: True,
            Response.OPEN_CONTROL: True,
            Response.WRITE_MOTOR_PARAMS: True,
            Operator.READ_MOTOR_PARAMS: True,
            Operator.READ_MOTOR_SENSOR: True,
            Operator.STOP_MOTOR: True,
            Response.STOP_MOTOR: True,
            Response.WRITE_CONTROL_PARAMS: True,
            Operator.READ_CONTROL_PARAMS: True,
            Response.LOCK: True,
            Operator.STOP_LOCK_OR_OPEN_CONTROL: True,
            Response.STOP_LOCK_OR_OPEN_CONTROL: True,
            Response.WRITE_ANALOG_PARAMS: True,
            Operator.READ_ANALOG_PARAMS: True,
            Response.READ_OTHER_PARAMS: True,
            Operator.READ_OTHER_PARAMS: True,
            Operator.WRITE_EEPROM: True,
            Response.WRITE_EEPROM: True
        }

        return _map.get(self.operator, False)

    def _set(self, param: str, data: int | float | CPU) -> None:
        self.items[param]['data'] = data

    def _set_bits(self, param: str, bits: str):
        self.items[param]['bits'] = bits

    def connect_controller(self, cpu: CPU):
        self._set('header1', 0xEB)
        self._set('header2', 0x90)
        self._set('param1', cpu)

    def set_speed(self, speed: int) -> None:
        self._set('param2', self.SPEED_CONTROL)
        self._set('param4', speed)

    def set_position(self, position: float) -> None:
        self._set('param2', self.POSITION_CONTROL)
        self._set_bits('param5', 'float')
        self._set('param5', set_angle(position))

    def set_torque(self, torque: float) -> None:
        self._set('param2', self.TORQUE_CONTROL)
        self._set_bits('param3', 'float')
        self._set('param3', torque)

    def stop_motor(self) -> None:
        self._set('param1', 0)

    def open_control(self, datas: list[int]) -> None:
        voltage = datas[0]
        freq = datas[1]
        dc_voltage = datas[2]
        self._set('param1', voltage)
        self._set('param2', freq)
        self._set('param3', dc_voltage)

    def stop_lock_or_open_control(self):
        self._set('param1', 0)

    def lock_zero(self, a_phase_voltage, b_phase_voltage, c_phase_voltage, dc_voltage):
        self._set('param1', a_phase_voltage)
        self._set('param2', b_phase_voltage)
        self._set('param3', c_phase_voltage)
        self._set('param4', dc_voltage)

    def read_current_rotator(self):
        self._set('status_code', 0x02)

    def read_params(self, param):
        self._set('status_code', param)

    def write_params(self, params: list[float]):
        for i, param in enumerate(params):
            self._set(f'param{i + 1}', param)

    def write_eeprom(self):
        self._set('status_code', 0x60)
