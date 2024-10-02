# https://superuser.com/questions/1776901/streaming-video-over-udp-with-ffmpeg-h264-low-latency
# replace 57 with your output audio, can be found using
# pactl list sources
# Here: my audio source "Monitor of Quantum 400 Analog Stereo" is source 57

ffmpeg -s 1920x1080 -f x11grab  -i :0.0+0,0 -f pulse -i 57 -preset ultrafast -tune zero_latency -f mpegts udp://127.0.0.1:8000
