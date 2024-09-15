from typing import Optional

from PySide6 import QtWidgets
from PySide6.QtCore import Qt
from PySide6.QtGui import QValidator
from PySide6.QtWidgets import QSizePolicy


class LineEdit(QtWidgets.QWidget):
    def __init__(self, prompt: str, param: str, parent: Optional[QtWidgets.QWidget] = None) -> None:
        super().__init__(parent=parent)

        self.prompt = QtWidgets.QLabel(prompt, self)
        self.prompt.setAlignment(Qt.AlignmentFlag.AlignRight | Qt.AlignmentFlag.AlignVCenter)

        self.param = QtWidgets.QLineEdit(self)
        self.param.setText(param)
        self.param.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Preferred)

        self.layout = QtWidgets.QHBoxLayout()
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.setSpacing(6)
        self.layout.addWidget(self.prompt)
        self.layout.addWidget(self.param)
        self.setLayout(self.layout)

    def set_validator(self, validator: QValidator):
        self.param.setValidator(validator)

    def set_text(self, value):
        self.param.setText(value)

    def get_text(self):
        return self.param.text()
