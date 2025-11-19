./setup_virtual_can.sh
cd can2udp

echo "starting can2udp ..."
./can2udp -b vcan0 -a 224.0.0.3 -p 11111

