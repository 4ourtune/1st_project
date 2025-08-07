from PySide2.QtWidgets import *
from PySide2.QtCore import QThread
from PySide2 import QtGui
from myAppUI import Ui_MainWindow
from controller import Controller


class MyApp(QMainWindow, Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.img = QtGui.QPixmap()
        self.main()

    def main(self):
        self.ctrl_worker = Controller()
        self.ctrl_thread = QThread()

        self.ctrl_worker.moveToThread(self.ctrl_thread)
        self.ctrl_worker.signal.connect(self.on_ctrl_message)

        self.ctrl_thread.started.connect(self.ctrl_worker.run)
        self.ctrl_thread.start()

    def on_ctrl_message(self, sndRcv: int, message: str):
        if sndRcv == 1:
            msg = "send : " + message
        else:
            msg = "recv : " + message
        print(msg)
        self.label_2.setText(msg)

    def closeEvent(self, event):
        self.ctrl_worker.stop()
        self.ctrl_thread.quit()
        self.ctrl_thread.wait()
        event.accept()

    def btnClicked(self):
        print("clicked")
