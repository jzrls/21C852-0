class Converter:
    @staticmethod
    def binary_to_octal(binary_number: str) -> str:
        return ' '.join([oct(int(b, 2)).replace('0o', '') for b in binary_number.split(' ')])

    @staticmethod
    def binary_to_decimal(binary_number: str) -> str:
        return str(int(binary_number, 2))

    @staticmethod
    def binary_to_hex(binary_number: str) -> str:
        return ' '.join([hex(int(b, 2)).replace('0x', '') for b in binary_number.split(' ')]).upper()

    @staticmethod
    def octal_to_binary(octal_number: str) -> str:
        return ''.join([bin(int(b, 8)).replace('0b', '') for b in octal_number.split(' ')])

    @staticmethod
    def octal_to_decimal(octal_number: str) -> str:
        return str(int(octal_number, 8))

    @staticmethod
    def octal_to_hex(octal_number: str) -> str:
        return ' '.join([hex(int(b, 8)).replace('0x', '') for b in octal_number.split(' ')]).upper()

    @staticmethod
    def decimal_to_binary(decimal_number: str) -> str:
        return bin(int(decimal_number)).replace('0b', '')

    @staticmethod
    def decimal_to_octal(decimal_number: str) -> str:
        return oct(int(decimal_number)).replace('0o', '')

    @staticmethod
    def decimal_to_hex(decimal_number: str) -> str:
        return hex(int(decimal_number)).replace('0x', '').upper()

    @staticmethod
    def hex_to_binary(hex_number: str) -> str:
        return ''.join([bin(int(b, 16)).replace('0b', '') for b in hex_number.split(' ')])

    @staticmethod
    def hex_to_octal(hex_number: str) -> str:
        return ' '.join([oct(int(b, 16)).replace('0o', '') for b in hex_number.split(' ')])

    @staticmethod
    def hex_to_decimal(hex_number: str) -> str:
        return str(int(hex_number, 16))

    @staticmethod
    def get_converter_function(source, destination):
        conversion_functions = {
            ("DEC", "BIN"): Converter.decimal_to_binary,
            ("DEC", "HEX"): Converter.decimal_to_hex,
            ("DEC", "OCT"): Converter.decimal_to_octal,
            ("BIN", "DEC"): Converter.binary_to_decimal,
            ("BIN", "HEX"): Converter.binary_to_hex,
            ("BIN", "OCT"): Converter.binary_to_octal,
            ("HEX", "DEC"): Converter.hex_to_decimal,
            ("HEX", "BIN"): Converter.hex_to_binary,
            ("HEX", "OCT"): Converter.hex_to_octal,
            ("OCT", "DEC"): Converter.octal_to_decimal,
            ("OCT", "BIN"): Converter.octal_to_binary,
            ("OCT", "HEX"): Converter.octal_to_hex,
            ("十进制", "二进制"): Converter.decimal_to_binary,
            ("十进制", "十六进制"): Converter.decimal_to_hex,
            ("十进制", "八进制"): Converter.decimal_to_octal,
            ("二进制", "十进制"): Converter.binary_to_decimal,
            ("二进制", "十六进制"): Converter.binary_to_hex,
            ("二进制", "八进制"): Converter.binary_to_octal,
            ("十六进制", "十进制"): Converter.hex_to_decimal,
            ("十六进制", "二进制"): Converter.hex_to_binary,
            ("十六进制", "八进制"): Converter.hex_to_octal,
            ("八进制", "十进制"): Converter.octal_to_decimal,
            ("八进制", "二进制"): Converter.octal_to_binary,
            ("八进制", "十六进制"): Converter.octal_to_hex,
        }

        return conversion_functions.get((source, destination), None)
