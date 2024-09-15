import struct
from typing import Callable

from PySide6.QtCore import QObject

from src.tools.utils import get_format


class Counter:
    """
    帧计数器
    """

    def __init__(self) -> None:
        self._count: int = -1

    def count(self) -> int:
        self._count = self._count + 1 if self._count < 255 else 0
        return self._count


class Instruction(QObject):
    def __init__(self, counter: Counter = None, crc: Callable[[bytes], bytes] = None,
                 model: str = "little") -> None:
        super().__init__()

        self.items: dict = {}
        self.pack_fmt: str = "<" if model == "little" else ">"
        self.unpack_fmt: str = ''

        self.counter: Counter = counter
        self.crc: Callable[[bytes], bytes] = crc

        self.res: tuple = ()

    def add(self, name: str, data: int = 0, bits: str = "uint8", is_frame_count=False, is_crc=False) -> None:
        self.items[name] = {"data": data, "bits": bits, "is_frame_count": is_frame_count, "is_crc": is_crc}

        if is_crc:
            self.unpack_fmt = self.pack_fmt + get_format(bits)
        else:
            self.pack_fmt += get_format(bits)

    def pack(self) -> bytes:
        data: list = [value['data'] if not value['is_frame_count'] else self.counter.count() for value in
                      self.items.values() if not value["is_crc"]]

        no_crc_packed: bytes = struct.pack(self.pack_fmt, *data)
        _crc: bytes = self.crc(no_crc_packed) if callable(self.crc) else bytes()

        return no_crc_packed + _crc

    def unpack(self, data) -> None:
        self.res = struct.unpack(self.unpack_fmt, data)


if __name__ == "__main__":
    from fastcrc import crc16

    print(crc16.xmodem(b"\x01\x02\x03\x04"))
