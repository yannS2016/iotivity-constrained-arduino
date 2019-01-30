# iotivity-constrained-arduino

immature port of iotivity constrained to arduino .

This port at present target the arduino mega board as the constrained device. the build takes about 18k of global memory 
on the device when executed statically thus depleting it before any stack allocation can start. this project thus deals only
with dynamic allocation. the ipadapter for network requirement is based around the Wiznet w5500 shield, i had to adapt the c++ 
libraries by providing C wrappers. though i plan to make the all build  C dependent.

***** Patching arduino libraries ******

i updated socket.cpp based on mainline iotivity_1_2(arduino adapter) comment regarding the a fix required on the socket.cpp code for the udp receive method
the patch is arduino_socket.patch. apply this patch to your socket.cpp file.

The iotivity constrained logging system is not directly compatible with arduino at it is. i move away from arduino Serial classes and adapted
a rs232(c based) code from contiki( inspired by the fact that iotivity constrained stack uses is based on contiki). this is found under deps 
and as libarduino-rs232 with this, one can used the stdio.h printing and string formatting methods. still one need to patch the oc_log.h header
to push constant text to arduino program memory(making use of PROGMEM attribute). the patch can be found under ./patches as oc_log.patch.
the arduino Time library target C++ code, though adding attribute line extern C or _cplusplus the fact that it uses method overloading
is not acceptable from C perspective. i provide a basic tweak to make it usable. one cam write a C++ class around it a provide a cleaner C wrapper or 
just make it plain C. 

under the patch directory apply:

arduino_socket.patch to your /path/to/arduino/home/libraries/Ethernet2
iotivity_constrained.patch to your iotivity-constrained-arduino 
arduino_time.patch to /path/to/arduino/home/libraries/Time

****** Build tools *********

1. Arduino Makefile from: https://github.com/sudar/Arduino-Makefile
this is used to compile and upload the hex file to the arduino board.

2. arduino sdk from: when can just provide a path to the arduino ide location or gt the sources form
https://github.com/arduino/ArduinoCore-avr.git

3. Arduino libraries 

Time library: https://github.com/PaulStoffregen/Time for implementing (oc_clock.c method)

Ethernet2 library: https://github.com/adafruit/Ethernet2 for implementing (ipadapter.c methods)

pseudo random number generator(pRNG): https://github.com/leomil72/pRNG for implementing (oc_random.c method)

********** Build process: linux (mainly Debian) *********

get the development sources

git clone https://github.com/yannS2016/iotivity-constrained-arduino

cd iotivity-constrained-arduino/ adapter

Note: initially tested on commit(0840e0a41a95fcff725e498fc7245a828a1a65a3) based on kishen work for esp32.
Though  i also tested to work on this commit(4f045e4a0f6d05a564877a6413cf33a51173fe25).

git clone --recursive https://github.com/iotivity/iotivity-constrained.git

cd ../../


review the setup.mk file to define the correct location of arduino-home(arduino core, libraries... path), arduino-mk (path to the arduino makefile 
folder. when happy proceed as below

make SERVER=1 IPV4=1  DYNAMIC=1 upload

******** Testing ******************
look under test to see some log of what to expect from the arduino server when running the simpleclient app from linux
other an arduino client test is also provide under this path for operating as a resources requester. 
additional test show that one iotivity-constrained on arduino works even from the main iotivity code(client/server)
the node js port(iotivity-node) as well, i was even able to get good response from the iot-rest-api server. more on that will be documented.

Issues: Arduino does not handle IPV6, thus client wont be able to send unicast requests/responses automatically. from what i got, the client actually sent two request, one over IPV6(default) and another via IPV4
the server, receive both and reject the latter, although the response from the server contains all endpoints enable(IPV6 and IPV4). by default the iotivity client code uses the first endpoint which is the IPV6 one. thus making arduino client unicast response 
unreachable(w5500 does not support IPV6). one should use the next endpoint from the reply(which IPV4) as "light_server = endpoint->next;"

	
