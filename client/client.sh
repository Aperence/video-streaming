# Open the forwarded stream with vlc
vlc udp://@127.0.0.1:9000

# alternatively, open it with ffplay
# ffplay -fflags nobuffer -flags low_delay -probesize 20000 -analyzeduration 1 -strict experimental -framedrop -f mpegts udp://127.0.0.1:9000
