from PySide2.QtWidgets import *
from myApp import MyApp

if __name__ == '__main__':
    app = QApplication()
    win = MyApp()
    win.show()
    app.exec_()
