# iotivity-constrained-arduino

immature port of iotivity constrained to arduino .

this port at present target the arduino mega board as the constrained device. the build takes about 18k of global memory 
on the device when executed statically thus depleting it before any stack allocation can start. this project thus deals only
with dynamic allocation. the ipadapter for network requirement is based around the wiznet w5500 shield, i had to adapt the c++ 
libraries by providing C wrappers. though i plan to make the all build  C dependant.


Build tools:

1. Arduino Makefile from: https://github.com/sudar/Arduino-Makefile
this is used to compile and upload the hex file to the arduino board.

2. arduino sdk from: when can just provide a path to the arduino ide location or gt the sources form
https://github.com/arduino/ArduinoCore-avr.git

build process: linux (mainly Debian)

get the development sources

git clone https://github.com/yannS2016/iotivity-constrained-arduino

cd iotivity-constrained-arduino/ adapter

git clone --recursive https://github.com/iotivity/iotivity-constrained.git

cd ../../

review the setup.mk file to define the correct location of arduino-home(arduino core, libraries... path), arduino-mk (path to the arduino makefile 
folder. when happy proceed as below

make SERVER=1 IPV4=1  DYNAMIC=1 upload

look under test to see some log of what to expect from the arduino server when running the simpleclient app from linux
other an arduino client test is also provide under this path for operating as a resources requester. 
additional test show that one iotivity-constrained on arduino works even from the main iotivity code(client/server)
the node js port(iotivity-node) as well, i was even able to get good response from the iot-rest-api server. more on that will be documented.