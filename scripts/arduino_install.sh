#!/bin/bash

#######################################################################
#User interaction functions 
#######################################################################
Print_inst_HEADINGs() {

	printf "%bInstallation script for porting iotivity constrained to arduino%b\n" "$HEADING" "$DEF_OUT"
	printf "%bInstallation is for now limited via linux(debian)%b\n" "$HEADING" "$DEF_OUT"
	echo 
	sleep 1
}
download_sources() {
################# Install Git ##########
	#Get the Iotivity constrained sources
	cd ${ARDUINO_IOTIVITY_CONSTRAINED_HOME}/components
	git clone --recursive ${IOTIVITY_CONSTRAINED}
  cd ../
	#Get the arduino sources
	wget  ${ARDUINO_SRC}
	tar -xvf arduino-1.8.8-linuxarm.tar.xz 
	mv arduino-1.8.8 arduino_home
	cd arduino_home/libraries
	#Get the Ethernet2, time and the pRNG libraries
	git clone ${ETHERNET2_LIB}
	git clone ${PRNG_LIB}
	git clone ${TIME_LIB}
	cd ../../
	#Get the Arduino makefile
	git clone ${ARDUINO_MAKEFILE} arduino_mk
	rm arduino-1.8.8-linuxarm.tar.xz
	echo
	echo
}
patch_sources()
{
	echo "*********************************************************************************************************"
	printf "%bApplying patches to arduino libraries and iotivity constrained sources %b\n" "$HEADING" "$DEF_OUT"
	echo "**********************************************************************************************************"
	cd ../patches
	sudo patch  -d/ -p0 < arduino_time.patch
	sudo patch  -d/ -p0 < arduino_socket.patch
	sudo patch  -d/ -p0 < arduino_iotivity_constrained_api.patch
	sudo patch  -d/ -p0 < arduino_iotivity_constrained_messaging.patch
	sudo patch  -d/ -p0 < arduino_iotivity_constrained_include.patch
	echo
}

building_sources()
{
	echo "*****************************************************************************"
	printf "%b            Building iotivity constrained archive and arduino app %b\n" "$HEADING" "$DEF_OUT"
	echo "******************************************************************************"
	cd ${ARDUINO_IOTIVITY_CONSTRAINED_HOME}
	#build the dependencies and archives
	make  DYNAMIC=1 IPV4=1 CLIENT=0 SERVER=1 clean
  make  DYNAMIC=1 IPV4=1 CLIENT=0 SERVER=1
	echo
}

################################################################################
# Main function -  execution function
###############################################################################
	#set -e   # exit if any command return with status different than 0

	#Define installation environment
	export ARDUINO_IOTIVITY_CONSTRAINED_USER=$USER
	export ARDUINO_IOTIVITY_CONSTRAINED_GROUP=$(groups "$ARDUINO_IOTIVITY_CONSTRAINED_USER" | sed 's/^.*\s:\s\(\S*\)\s.*$/\1/')	
	export ARDUINO_IOTIVITY_CONSTRAINED_HOME=${HOME}/iotivity-constrained-arduino
	echo $ARDUINO_IOTIVITY_CONSTRAINED_BUILD_PATH

	export IOTIVITY_CONSTRAINED="https://github.com/iotivity/iotivity-constrained.git"
	export ARDUINO_MAKEFILE="https://github.com/sudar/Arduino-Makefile"
	export ARDUINO_SRC="https://downloads.arduino.cc/arduino-1.8.8-linuxarm.tar.xz"
	export ETHERNET2_LIB="https://github.com/adafruit/Ethernet2"
	export PRNG_LIB="https://github.com/leomil72/pRNG.git"
	export TIME_LIB="https://github.com/PaulStoffregen/Time.git"
	export ERROR="\033[31;1m"  
	export HEADING="\033[32;1m" 
	export MESSAGE="\033[33;1m" 
	export DEF_OUT="\033[0m"  
  export server
  export client
	export dynamic
	export ipv4
	echo
	#Process the different installation functions
	Print_inst_HEADINGs 
	#patch_sources
	#download_sources
	#usage
  building_sources  
	exit 0
