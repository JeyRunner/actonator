# MotorDriver Can Client Library
Interface with the MotorDriver from linux via the can bus.

<br/>

## Compile
Install for cross compile for raspberry pi:
```bash
apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf
```

### Compile the example app
```bash
# install deps
apt-get install libasio-dev libboost-coroutine-dev libboost-dev

mkdir build
cd build
cmake ..
make genCanSotHeaders   # gen protocol headers first
make test_app
```

## Can bus setup on Raspberry Pi
edit /boot/config.txt:
```
# add
dtoverlay=mcp2515-can0,oscillator=8000000,interrupt=25
dtoverlay=spi-bcm2835-overlay
```

For testing installation:
```bash
sudo apt-get install can-utils

# check -> should show can0
ls /sys/bus/spi/devices/spi0.0/net
```


Bring up can interface with 500kHz:
```bash
sudo ip link set can0 up type can bitrate 500000

# check interface
sudo ifconfig

# send something
cansend can0 123#42ff4545
```

Refrerence: https://forums.raspberrypi.com//viewtopic.php?t=141052



## CAN Tools
### Forward CAN via UDP
Setup CAN forwarding via upd from pi to other linux computer.

On local dev computer and on pi (see https://github.com/tvesalainen/can2udp):
```bash
git clone https://github.com/tvesalainen/can2udp.git
cd can2udp
make
sudo make install
```

Setup forwarding:
```bash
# on dev computer, create virtual can interface
./tools/setup_virtual_can.sh
can2udp -b vcan0 -a 224.0.0.3 -p 11111

# on other linux computer
can2udp -b can0 -a 224.0.0.3 -p 11111
```