import time
from PySide2.QtCore import *
from joystick import Joystick
from bluetooth import BluetoothHandler


class Controller(QObject):
    signal = Signal(int, str)  # int : 0 -> send, 1 -> recv / str : message

    def __init__(self):
        super().__init__()
        self.js = Joystick()
        self.bt = BluetoothHandler()
        self.bt.register_callback(self.on_bt_message)
        self.running = True

    def on_bt_message(self, message: str):
        self.signal.emit(2, message)

    def run(self):
        self.bt.start()

        pre_swt_msg = ""
        pre_vrx_vry_msg = ""

        while self.running:
            cnt = 0

            swt_msg = f"P{self.js.get_converted_swt():02}" + '\0'
            if swt_msg != pre_swt_msg:
                self.bt.send(swt_msg)
                self.signal.emit(1, swt_msg)
                pre_swt_msg = swt_msg
                cnt += 1

            vrx_vry_msg = f"M{self.js.get_converted_vrx():02}{self.js.get_converted_vry():02}" + '\0'
            if vrx_vry_msg != pre_vrx_vry_msg:
                self.bt.send(vrx_vry_msg)
                self.signal.emit(1, vrx_vry_msg)
                pre_vrx_vry_msg = vrx_vry_msg
                cnt += 1

            time.sleep(0.01 * (3 - cnt))

    def stop(self):
        self.running = False
        self.bt.stop()
        self.bt.wait_until_stopped()
