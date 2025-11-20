# MotorDriver Can Client Library
Interface with the MotorDriver from linux via the can bus. 
Here also the can protocol is defined.


## Usage
First connect a usb-to-CAN adapter of your choice to the computer and setup the can interface:
```bash
sudo ip link set can0 up type can bitrate 500000

# check interface
sudo ifconfig

# test send something
cansend can0 123#42ff4545
```

For `canable` adapters there is a helper script for this in `tools/setup_slcand_can.sh`.

### Configure the Board and Move the Motor
You can configure the motor driver with the `configuration_tool`:
```bash
# in the build folder
# set gains:
./configuration_tool --can-interface can0 --client-id 1 --set-gain-p 1.0 --set-gain-d 0.05
```

Move the actuator in a sinusodial motion:
```bash
# here we also can update the gains:
./example_app -i can0 --client-id 1 \
  --loop-delay-ms 2 `--pd-sin-lambda 0.4` \
  --throttle-limit 0.9 --pd-sin-amplitude 10000 \
  --gain-p -1.5 --gain-d -0.00 
```


## Compile
### Compile the example and configuration execuables
```bash
# install deps
apt-get install libasio-dev libboost-coroutine-dev libboost-dev

mkdir build
cd build
cmake ..
make genCanSotHeaders   # gen protocol headers first
make example_app 
make configuration_tool 
```