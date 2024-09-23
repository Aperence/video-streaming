# https://superuser.com/questions/1776901/streaming-video-over-udp-with-ffmpeg-h264-low-latency
ffmpeg -s 1920x1080 -f x11grab  -i :0.0+0,0  -preset ultrafast -tune zero_latency -f mpegts udp://127.0.0.1:8000
