sudo python http_server.py wlp2s0 &
sleep 0.5
python zmq/relais.py &
./../bin/streamRetro /usr/lib/libretro/gambatte_libretro.so ../roms/gbai.gb
