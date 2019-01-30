# iotivity-constrained-arduino

## Porting iotivity constrained to arduino .

This port at present target the arduino mega board as the constrained device. The build takes about 18k of global memory on the device when executed statically thus depleting it before any stack allocation can start. the project thus deals only with dynamic allocation. The ipadapter for network requirement is based around the Wiznet w5500 shield,  the arduino c++ libraries were adapted by providing C wrappers. Though i plan to make the  build  C dependent.

### Patching arduino libraries 

i updated socket.cpp based on the mainline iotivity_1_2(arduino adapter) comment regarding the  fix required on the socket.cpp code for the udp receive method the patch is arduino_socket.patch. apply this patch to your socket.cpp file.

The iotivity constrained logging system is not directly compatible with arduino at it is. i move away from arduino Serial classes and adapted a rs232(c based) code from contiki( inspired by the fact that iotivity constrained stack uses is based on contiki). this is found under deps and as libarduino-rs232. using this, one can used the stdio.h printing and string formatting methods. still one need to patch the oc_log.h header in order to push constant text to arduino program memory (making use of PROGMEM attribute). 

the arduino Time library target C++ code, though adding attribute line extern C or _cplusplus the fact that it uses method overloading
is not acceptable from C perspective. i provide a basic tweak to make it usable. one cam write a C++ class around it a provide a cleaner C wrapper or  just make it plain C. 

Aplying patches

under ./pathces  apply:

arduino_socket.patch to your /path/to/arduino/home/libraries/Ethernet2
arduino_time.patch to /path/to/arduino/home/libraries/Time
iotivity_constrained.patch to your iotivity-constrained-arduino 

## Build tools

1. **Arduino Makefile** [Arduino mk](https://github.com/sudar/Arduino-Makefile) Used to compile and upload the hex file to the arduino board.

2. **Arduino sdk** [Arduino home](https://github.com/arduino/ArduinoCore-avr.git) , one can just provide a path to the arduino ide

3. ***Arduino libraries**

- **Time library** [Time] (https://github.com/PaulStoffregen/Time) for implementing (oc_clock.c method)

- **Ethernet2 library** [w5500 ethernet](https://github.com/adafruit/Ethernet2) for implementing (ipadapter.c methods)

- **pseudo random number generator** [pRNG](https://github.com/leomil72/pRNG) for implementing (oc_random.c method)

## Build process: linux (mainly Debian) 

### Get the development sources

`git clone https://github.com/yannS2016/iotivity-constrained-arduino`

`cd iotivity-constrained-arduino/ adapter`

**Note:** Initially tested on commit(0840e0a41a95fcff725e498fc7245a828a1a65a3) based on this work [https://github.com/espressif/esp32-iotivity] . the port was lately tested on this commit(4f045e4a0f6d05a564877a6413cf33a51173fe25).

`git clone --recursive https://github.com/iotivity/iotivity-constrained.git`

cd ../../

Review the setup.mk file to define the correct location of arduino-home(arduino core, libraries... path), arduino-mk (path to the arduino makefile  folder. When happy proceed as below

`make SERVER=1 IPV4=1  DYNAMIC=1 upload`

## Testing
look under test to see some log of what to expect from the arduino server when running the simpleclient app from linux other an arduino client test is also provide under this path for operating as a resources requester. additional test show that one iotivity-constrained on arduino works even from the main iotivity code(client/server)the node js port(iotivity-node) as well, i was even able to get good response from the iot-rest-api server. more on that will be documented.

## Issues
Arduino does not handle IPV6, thus client wont be able to send unicast requests/responses automatically. from what i got, the client actually sent two request, one over IPV6(default) and another via IPV4 the server, receive both and reject the latter, although the response from the server contains all endpoints enable(IPV6 and IPV4). by default the iotivity client code uses the first endpoint which is the IPV6 one. thus making arduino client unicast response unreachable(w5500 does not support IPV6). one should use the next endpoint from the reply(which IPV4) as `light_server = endpoint->next;`

	
