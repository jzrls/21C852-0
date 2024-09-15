import logging
import os
from datetime import datetime
from logging.handlers import TimedRotatingFileHandler


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
        formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s', datefmt='%Y-%m-%d %H:%M:%S')
        self.handler.setFormatter(formatter)
        self.logger.addHandler(self.handler)

    def record(self, message: str) -> None:
        time = datetime.now()
        time = time.strftime("%d/%m/%Y %H:%M:%S")
        self.logger.info(f'[{time}]-{message}')
