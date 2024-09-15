from typing import Optional

from PySide6 import QtWidgets


class GroupBox(QtWidgets.QGroupBox):
    def __init__(self, title: str, target) -> None:
        super().__init__()
        self.setTitle(title)
        self.target = target
        self.setLayout(self.target.layout)


class GroupBox_1(QtWidgets.QGroupBox):
    def __init__(
            self, title: str, target: QtWidgets.QWidget, parent: Optional[QtWidgets.QWidget] = None
    ) -> None:
        super().__init__(parent=parent)
        self.setTitle(title)

        self.target = target
        self.layout = QtWidgets.QVBoxLayout()
        self.layout.addWidget(self.target)

        self.setLayout(self.layout)
