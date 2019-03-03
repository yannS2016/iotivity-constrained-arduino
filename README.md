# iotivity-constrained-arduino

## Porting iotivity constrained to arduino .

This port at present target the arduino mega board as the constrained device. The build takes about 18k of global memory on the device when executed statically thus depleting it before any stack allocation can start. the project thus deals only with dynamic allocation. The ipadapter for network requirement is based around the Wiznet w5500 shield,  the arduino c++ libraries were adapted by providing C wrappers. Though i plan to make the  build  C dependent.

### Patching arduino libraries 

I updated socket.cpp based on the mainline iotivity_1_2(arduino adapter) comment regarding the  fix required on the socket.cpp code for the udp receive method the patch is arduino_socket.patch. apply this patch to your socket.cpp file.

The iotivity constrained logging system is not directly compatible with arduino at it is. i move away from arduino Serial classes and adapted a rs232(c based) code from contiki( inspired by the fact that iotivity constrained stack uses is based on contiki). this is found under deps and as libarduino-rs232. using this, one can used the stdio.h printing and string formatting methods. still one need to patch the oc_log.h header in order to push constant text to arduino program memory (making use of PROGMEM attribute). 

the arduino Time library target C++ code, though adding attribute line extern C or _cplusplus the fact that it uses method overloading
is not acceptable from C perspective. i provide a basic tweak to make it usable. one cam write a C++ class around it a provide a cleaner C wrapper or  just make it plain C. 


## Build tools

- **Arduino Makefile** [Arduino mk](https://github.com/sudar/Arduino-Makefile) Used to compile and upload the hex file to the arduino board. this is added as a submodule to this repository, thus it will fetch automatically.

- **Arduino sdk** [Arduino home](https://github.com/arduino/ArduinoCore-avr.git) , one can just provide a path to the local arduino IDE

- **Arduino libraries**

   - **Time library** [Time](https://github.com/PaulStoffregen/Time) for implementing (oc_clock.c method)

   - **Ethernet2 library** [w5500 ethernet](https://github.com/adafruit/Ethernet2) for implementing (ipadapter.c methods)

   - **pseudo random number generator**[pRNG](https://github.com/leomil72/pRNG) for implementing (oc_random.c method)

## Build process: linux (mainly Debian) 

### Get the development sources

`git clone --recurse-submodules https://github.com/yannS2016/iotivity-constrained-arduino`

### Applying patch

all patch files are under iotivity-constrained-arduino/patches

 cd iotivity-constrained-arduino/patches

 - patch -p0 < arduino_socket.patch
 - patch -p0 < arduino_time.patch
 - patch -p0 <iotivity_constrained.patch
 
**Note:** The port was initially tested on commit(0840e0a41a95fcff725e498fc7245a828a1a65a3) based on this work [esp32 iotivity](https://github.com/espressif/esp32-iotivity).

cd ../../

**Note:** Arduino sdk depends on the building platform thus one will need to define the path to arduino-home(arduino core, libraries... path) in the setup.mk manually.

`make SERVER=1 IPV4=1  DYNAMIC=1 upload`

## Testing
Under test  see some log of what to expect from the arduino server when running the simpleclient app from linux. An arduino
client test is also provide under this path for operating as a resources requester. Additional tests show that one iotivity-constrained client/server can communicate with another client/server based on the main iotivity-code.some test were perfomed using the  iotivity node,  JS port(iotivity-node) as well yieldng expected results, i was even able to get good response from the iot-rest-api server. more on that will be documented.

## Issues
Arduino does not handle IPV6, thus client wont be able to send unicast requests/responses automatically. from what i got, the client actually sent two request, one over IPV6(default) and another via IPV4 to the server which  receive both and reject the latter, although the response from the server contains all endpoints enabled(IPV6 and IPV4). By default the iotivity client code uses the first endpoint which is the IPV6 one. thus making arduino client unicast response unreachable(w5500 does not support IPV6). one should use the next endpoint from the reply(which IPV4) as `light_server = endpoint->next;`

	
