themes = (
    'dark_amber.xml',
    'dark_blue.xml',
    'dark_cyan.xml',
    'dark_lightgreen.xml',
    'dark_pink.xml',
    'dark_purple.xml',
    'dark_red.xml',
    'dark_teal.xml',
    'dark_yellow.xml',
    'light_amber.xml',
    'light_blue.xml',
    'light_cyan.xml',
    'light_cyan_500.xml',
    'light_lightgreen.xml',
    'light_pink.xml',
    'light_purple.xml',
    'light_red.xml',
    'light_teal.xml',
    'light_yellow.xml'
)


def get_bit(num: int, bit_position: int) -> int:
    return (num >> bit_position) & 1


def set_bit(num, bit_position):
    return num | (1 << bit_position)


def clear_bit(num, bit_position):
    return num & ~(1 << bit_position)


def update_bit(num, bit_position, bit_value):
    if bit_value == 0:
        return clear_bit(num, bit_position)
    else:
        return set_bit(num, bit_position)


def get_bits(num, start_bit, end_bit):
    mask = (2 ** (end_bit - start_bit + 1)) - 1
    return (num >> start_bit) & mask


def set_bits(num, start_bit, end_bit, value):
    mask = (2 ** (end_bit - start_bit + 1)) - 1
    return num | ((value & mask) << start_bit)


def clear_bits(num, start_bit, end_bit):
    mask = (2 ** (end_bit - start_bit + 1)) - 1
    return num & ~(mask << start_bit)


def update_bits(num, start_bit, end_bit, value):
    mask = (2 ** (end_bit - start_bit + 1)) - 1
    return num | ((value & mask) << start_bit)


def get_format(bits: str = "uint8") -> str:
    _map = {
        "uint8": "B",
        "uint16": "H",
        "uint32": "I",
        "uint64": "Q",
        "int8": "b",
        "int16": "h",
        "int32": "i",
        "int64": "q",
        "float": "f",
        "double": "d",
    }
    return _map.get(bits, None)
