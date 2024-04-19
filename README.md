#Software for USB KVM. 
## Instalation
Run 
```
./install.sh
```

Unplug and plug it the device.
## Running
Reconnect your master pc using USB-C and the slave using the other USB-C and HDMI.

Than run the `./start` script which should display your screen and after few seconds locks your mouse and keyboard and sends it to the slave. 
The mouse is kept in top left corner while capturing.


To end the streaming (and a lock) press "CTRL q" (controll + q)


## Dependencies for runing pure script
Runnable compiled script is available. 
To modify and run the .py file 
`
pip install pynput pyserial easyguils

`

### Manual instalation
Make sure the udev rules are installed 
```
sudo cp 99-usb-usb.rules /etc/udev/rules.d/
```

### Manual run
Run 'guvcview' or 'cheese' (or any other software that shows webcams) and prepare your view of the slave computer. 
Possible  commands to run different webcam programs
```
cheese -f -d "UVC Camera (534d:2109): USB Vid" 
ffmpeg -f v4l2 -input_format mjpeg -video_size 1920x1080 -i /dev/videoKVM -vf "format=yuv420p" -f sdl "Webcam Feed"
guvcview -d /dev/video4 -f MJPG -x 1920x1080 -a none -r sdl 
```

Than run the `./send_data` script which locks your mouse and keyboard and sends it to the slave. 

To end the streaming (and a lock) press "CTRL q" (controll + q)

### Manual install 
run 
```
pyinstaller -F send_data.py
cp dist/send_data .

```
