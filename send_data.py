#!/usr/bin/env python3
import signal
from pynput import keyboard, mouse  # https://github.com/moses-palmer/pynput
from pynput.mouse import Controller
import serial
import time
from os import stat
import sys
try:
    GUI = True
    import easygui
except ImportError:
    GUI = False

test_list = ["T", "e", "s", "t", "\n"]
DELAY = 5

class SerialWrapper:
    def __init__(self, device):
        self.ser = serial.Serial(device, 115200)
        self.video_lost = False

    def sendData(self, data):
        data = "$" + str(data)
        data += "#\n"
        if not self.video_lost:
            if not video_exists():
                self.video_lost = True
                if GUI:
                    easygui.msgbox("Video stream lost", title="ERROR")
                print("Video stream lost")
        else:
            return
        # print(data)
        self.ser.write(data.encode())

def video_exists():
    return exists('/dev/videoKVM')

def exists(path):
    """Test whether a path exists.  Returns False for broken symbolic links"""
    try:
        stat(path)
    except OSError:
        return False
    return True


class SupressInput:
    def __init__(self):
        self.key_combination = '<ctrl>+q'
        self.hotkey = keyboard.HotKey(keyboard.HotKey.parse(self.key_combination), self.enable_all)
        self.enabled = False
        self.run_video = True
        self.blocking = True
        self.testing = False
        self.count = 0
        cont = True
        try:
            self.serial = SerialWrapper('/dev/ttyKVM')
        except Exception as ex:
            self.serial = None
            print("Trouble with serial: ", ex)
            if GUI:
                easygui.msgbox("Trouble with serial: " + str (ex), title="ERROR")
            cont = False
        if not video_exists():
            if GUI:
                cont = easygui.ynbox('No video device /dev/videoKVM. Shall I continue?', 'WARNING', ('Yes', 'No'))
            else:
                cont = input("No video device. Shall I continue? [y/n]")
        if not cont:
            exit(-1)
        self.controller = Controller()
        self.mouse_pose = 60
        self.controller.position = (self.mouse_pose, self.mouse_pose)

        # self.serial = SerialWrapper('/dev/ttyUSB0')
        # signal.signal(signal.SIGINT, self.signal_handler)

    def disable_all(self):
        """Disable mouse and keyboard events"""

        self.mouse_listener =  mouse.Listener(suppress=self.blocking,
                            on_move=self.on_move,
                            on_click=self.on_click,
                            on_scroll=self.on_scroll)
        self.hotkey_listener =  keyboard.Listener(on_press=self.for_canonical(self.hotkey.press),
                                                  on_release=self.for_canonical(self.hotkey.release))
        self.keyboard_listener = keyboard.Listener(suppress=self.blocking,
                                  on_press=self.on_press,
                                  on_release=self.on_release)
        self.mouse_listener.start()
        self.hotkey_listener.start()
        self.keyboard_listener.start()
        self.mouse_listener.join()
        self.hotkey_listener.join()
        self.keyboard_listener.join()

    def show_unblock_message(self):
        print("You now have " + str(DELAY) + "seconds to switch to the remote desktop before loosing control - guvcwiev")
        print(f'Push {self.key_combination} to unblock mouse and keyboard input...')
        for i in range(DELAY, 0, -1):
            print(str(i) + "  ", end='', flush=True)
            time.sleep(1)
        print("Remote control enabled")


    def enable_all(self):
        """Enable mouse and keyboard events"""
        print("Disabling remote control")
        if not self.enabled:
            self.mouse_listener.stop()
            self.keyboard_listener.stop()
            self.enabled = True
        sys.exit()

    def for_canonical(self, f):
        # print(f)
        return lambda k: f(self.hotkey_listener.canonical(k))

    def on_move(self, x, y):
        movex = x - self.mouse_pose
        movey = y - self.mouse_pose
        if not self.blocking:
            return
        if movex != 0 or movey != 0:
            self.controller.position = (self.mouse_pose, self.mouse_pose)
        else:
            return

        if self.serial is not None:
            self.serial.sendData([0, 0, movex, movey])

    def on_click(self, x, y, button, pressed):
        # print('{0} at {1}'.format(
        # 'Pressed' if pressed else 'Released',
        # (x, y)))
        # print(button)
        if self.serial is not None:
            self.serial.sendData([button.value, int(pressed), x, y])
        # print(pressed)

    def on_scroll(self, x, y, dx, dy):
        # print('Scrolled {0} at {1}'.format(
        # 'down' if dy < 0 else 'up',
        # (x, y)))
        if self.serial is not None:
            self.serial.sendData([4, 0, dx, dy])

    def on_press(self, key):
        try:
            # print("ascii: ", ord(key.char))
            send = ord(key.char)
        except AttributeError:
            send = key.value.vk
            # print(key)
        except TypeError:
            print("Type error - not working key")
            # print(key)
            return
        if self.serial is not None:
            self.serial.sendData((1, send))

    def on_release(self, key):
        try:
            # print("ascii: ", ord(key.char))
            send = ord(key.char)
        except AttributeError:
            send = key.value.vk
            # print(key)
        except TypeError:
            # print("Type error")
            print(key)
            return
        if self.serial is not None:
            self.serial.sendData((0, send))

    def test(self):
        try:
            while True:
                self.serial.sendData([0, 0, 12, 34])
                time.sleep(1)
                # self.serial.sendData((1, ord(test_list[ self.count])))
                # time.sleep(0.1)
                # self.serial.sendData((0, ord(test_list[ self.count])))
                # self.count = self.count + 1
                if self.count >= len(test_list):
                    self.count = 0
        except KeyboardInterrupt:
            print('interrupted!')


def main():
    supress_input = SupressInput()
    supress_input.show_unblock_message()
    if supress_input.testing:
        supress_input.test()
    else:
        supress_input.disable_all()


if __name__ == '__main__':
    main()
