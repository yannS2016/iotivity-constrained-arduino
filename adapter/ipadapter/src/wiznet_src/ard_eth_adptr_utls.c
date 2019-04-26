/******************************************************************
*
* Copyright 2018 iThemba LABS All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at

*    http://www.apache.org/licenses/LICENSE-2.0

* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************/
#include <socket.h>
#include <ctype.h>
#include "config.h"
// c adapter
#include "Wiz5500.h"
#include "oc_log.h"
#include "ard_eth_adptr_utls.h"

OCResult_t arduino_get_free_socket(uint8_t *sockID){
	
	uint8_t state;
		/*
		* Check if we dont already have a created object
		* Otherwise create it and test fro success
		*/
		if(!wiznet5500) {
			wiznet5500 = wiz5500_create();
			if(!wiznet5500)
			return STATUS_FAILED;
		}	
	//Is any socket available to work with ?
	*sockID = 0;
	for (uint8_t i = 1; i < OC_MAX_NUM_ENDPOINTS; i++)
	{
		/*
		* First read the state of all W5500 socket return 
		* for usage the socket that is closed ( not being used)or the socket which is closing
		*/
		state = wiz5500_readSnSR(wiznet5500, &i) ;
		if (state == SnSR_CLOSED || state == SnSR_FIN_WAIT)
		{
			*sockID = i;
			break;
		}
	}
	/* 
	if no socket is availlable for new operation
	return a socket fail status in this case socketID = 0
	first socket or socket as far as w5500 is concern as the address 1(0001)
	*/
	if (*sockID == 0)
	{
		OC_ERR("No socket sockID 0");
		return SOCKET_OPERATION_FAILED;
	}
	return STATUS_OK;	
}

OCResult_t arduino_init_udp_socket(uint16_t *local_port, uint8_t *socketID){
   
	if(!socketID) {
		OC_ERR("Socket ID not provided!");
		return SOCKET_OPERATION_FAILED;
	} 
	/*Get an availlable socket(closing or closed)*/
	OCResult_t ret = arduino_get_free_socket(socketID);
	if (ret != STATUS_OK)
	{
		OC_ERR("Get sock failed!");
		return ret;
	}
	//Create a datagram socket on which to recv/send.
	if (!socket(*socketID, SnMR_UDP, *local_port, 0))
	{
		OC_ERR("socket create failed!");
		return STATUS_FAILED;
	}
	return STATUS_OK;	
}
OCResult_t 
arduino_init_mcast_udp_socket(const char *mcast_addr, uint16_t *mcast_port, uint16_t *local_port, uint8_t *socketID)
{
	//OC_DBG(("Init mcast udp socket!"));
	if(!socketID || !mcast_addr) {
		OC_ERR("Socket ID or mcast addr null!");
		return SOCKET_OPERATION_FAILED;
	} 
	uint8_t mcast_mac_addr[] = { 0x01, 0x00, 0x5E, 0x00, 0x00, 0x00};
	uint8_t ip_addr[4] = { 0 };
	// in case user supply mcastip:mcastport
	uint16_t parsed_port = 0;
	if (arduino_parse_IPv4_addr(mcast_addr, ip_addr, sizeof(ip_addr), &parsed_port) != STATUS_OK)
	{
		OC_ERR("mcast ip parse fail!");
		return STATUS_FAILED;
	}
	*socketID = 0;
	OCResult_t ret = arduino_get_free_socket(socketID);
	if (ret != STATUS_OK)
	{
		OC_ERR("Get sock fail!");
		return ret;
	}
	//Calculate Multicast MAC address
	mcast_mac_addr[3] = ip_addr[1] & 0x7F;
	mcast_mac_addr[4] = ip_addr[2];
	mcast_mac_addr[5] = ip_addr[3];
	//OC_WRN("mcast address: %d.%d.%d.%d", ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
	wiz5500_writeSnDIPR(wiznet5500, socketID, (uint8_t *)ip_addr);
	wiz5500_writeSnDHAR(wiznet5500, socketID, (uint8_t *)mcast_mac_addr);
	wiz5500_writeSnDPORT(wiznet5500, socketID, mcast_port);
	if (!socket(*socketID, SnMR_UDP, *local_port, SnMR_MULTI))
	{
		OC_ERR("sock create fail!");
		return SOCKET_OPERATION_FAILED;
	}
	return STATUS_OK;																						 
}
/// Retrieves the IP address assigned to Arduino Ethernet shield
OCResult_t oc_ard_get_iface_addr(uint8_t *address)
{
	//TODO : Fix this for scenarios when this API is invoked when device is not connected
	if(!wiznet5500) {
		wiznet5500 = wiz5500_create();
		if(!wiznet5500)
			return STATUS_FAILED;
	}
	wiz5500_getIPAddress(wiznet5500, (uint8_t *)address);
	return STATUS_OK;
}
///
OCResult_t arduino_parse_IPv4_addr(const char *ip_addrStr, uint8_t *ip_addr,
                                      uint8_t ip_addrLen, uint16_t *port)
{
	if (!ip_addr || !isdigit(ip_addrStr[0]) || !port)
	{
		OC_ERR("invalid param!");
		return STATUS_INVALID_PARAM;
	}
	uint8_t index = 0;
	uint8_t dotCount = 0;
	ip_addr[index] = 0;
	*port = 0;
	while (*ip_addrStr)
	{
		if (isdigit(*ip_addrStr))
		{
			if(index >= ip_addrLen)
			{
				OC_ERR(("invalid mcast addr!"));
				return STATUS_INVALID_PARAM;
			}
			ip_addr[index] *= 10; //20+2=22 --> 220+7
			ip_addr[index] += *ip_addrStr - '0';
		}
		else if (*ip_addrStr == '.')
		{
			index++;
			dotCount++;
			ip_addr[index] = 0;
		}
		else
		{
			break;
		}
		ip_addrStr++;
	}
  // incase user supply ip:port fetch the port number as below
	if (*ip_addrStr == ':')
	{
		ip_addrStr++;
		while (*ip_addrStr)
		{
			if (isdigit(*ip_addrStr))
			{
				*port *= 10;
				*port += *ip_addrStr - '0';
			}
			else
			{
				break;
			}
			ip_addrStr++;
		}
	}
	if (dotCount == 3)
	{
		return STATUS_OK;
	}
	return STATUS_FAILED;
}

