import logging
import os
from datetime import datetime
from logging.handlers import TimedRotatingFileHandler

from openpyxl.workbook import Workbook


class Recorder:
    def __init__(self):
        # 创建一个handler，用于写入日志文件

        # 创建一个logger
        self.logger = logging.getLogger('my_logger')
        self.logger.setLevel(logging.INFO)

        # 创建一个按日期切割的文件处理程序
        directory = 'logs'  # 存放日志文件的目录
        if not os.path.exists(directory):
            os.makedirs(directory)

        #  创建一个按日期切割的文件处理程序
        self.name = 'logs/' + datetime.now().strftime('%Y-%m-%d') + '.log'
        self.handler = TimedRotatingFileHandler(self.name, when='D', interval=1, backupCount=30)
        self.handler.suffix = '%Y-%m-%d.log'

        # 创建一个格式器并将其添加到处理程序
        fmt = '%(asctime)s - %(message)s'
        date_fmt = '%Y-%m-%d %H:%M:%S'
        formatter = logging.Formatter(fmt, datefmt=date_fmt)
        self.handler.setFormatter(formatter)
        self.logger.addHandler(self.handler)

        self.header = [
            "包头1", "包头2", "包类型", "包计数", "数据长度", "驱动状态",
            '阀1-设定角度', '阀1-实际角度', '阀2-设定角度', '阀2-实际角度',
            '泵-设定转速', '泵-实际转速',
            '阀1-温度', '阀2-温度', '泵-温度',
            '阀1-母线电压', '阀2-母线电压', '泵-母线电压',
            '阀1-母线电流', '阀2-母线电流', '泵-母线电流',
            '阀1-A相电流', '阀1-B相电流', '阀1-C相电流',
            '阀2-A相电流', '阀2-B相电流', '阀2-C相电流',
            '泵-A相电流', '泵-B相电流', '泵-C相电流',
            "故障状态字", "校验码"
        ]

        self.__wb = Workbook()
        self.__ws = self.__wb.active
        self.__ws.append(self.header)
        self.__ws_name = 'logs/' + datetime.now().strftime('%Y-%m-%d') + '.xlsx'

    def save(self):
        self.__wb.save(self.__ws_name)

    def record(self, message) -> None:
        if isinstance(message, str):
            self.logger.info(message)
        else:
            self.__ws.append(message.res)
