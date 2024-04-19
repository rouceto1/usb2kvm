#!/bin/bash
#cheese -f -d "UVC Camera (534d:2109): USB Vid" 
#ffmpeg -f v4l2 -input_format mjpeg -video_size 1920x1080 -i /dev/videoKVM -vf "format=yuv420p" -f sdl "Webcam Feed"
guvcview -d /dev/videoKVM -f MJPG -x 1920x1080 -a none -r sdl 
