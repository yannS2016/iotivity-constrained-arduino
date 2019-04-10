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
#include "port/oc_assert.h"
#include "port/oc_connectivity.h"
#include "port/oc_log.h"
#include "Wiz5500.h"
#include "ard_eth_adptr_srv.h"

/**
 * @brief Flag to check if multicast server is started
 */
bool arduino_mcast_serv_started = false;

uint8_t start_udp_server(uint16_t *local_port)
{
	if(!local_port) {
	  OC_DBG("server port null!");
		return STATUS_FAILED;
	}
	uint8_t raw_ip_addr[4];
	// check if we have a working ref of w5500
	if(!wiznet5500) {
		wiznet5500 = wiz5500_create();
		if(!wiznet5500)
			return STATUS_FAILED;
	}
	wiz5500_getIPAddress(wiznet5500, raw_ip_addr);
	uint8_t serverFD = 1; // try this socket
	if (arduino_init_udp_socket(local_port, &serverFD) != STATUS_OK)
	{
		return STATUS_FAILED;
	}
	return serverFD;
}
uint8_t start_udp_mcast_server(const char *mcast_addr, 
                              uint16_t *mcast_port,
                              uint16_t *local_port)
{
	if (arduino_mcast_serv_started == true)
	{
	  return SERVER_STARTED_ALREADY;
	}
	uint8_t serverFD = 1; 
	if (arduino_init_mcast_udp_socket(mcast_addr, mcast_port, local_port, &serverFD)!= STATUS_OK)
		return STATUS_FAILED;
  return serverFD;	
}

/*Utility method to monitor ready socket*/
static uint16_t socket_ready(uint8_t *socketID){
  
  if(!wiznet5500) {
    wiznet5500 = wiz5500_create();
    if(!wiznet5500)
      return STATUS_FAILED;
  }
  uint16_t recvLen = wiz5500_getRXReceivedSize(wiznet5500, socketID);
  if(recvLen == 0) {
    return 0;
  } else {
    //OC_ERR("DATA READY.....");
    return recvLen;
  }
}

uint8_t select(uint8_t nsds, sdset_t *setsds){
  uint8_t n = 0;
  for(int i = 1; i < nsds; i++){
      uint16_t ret = socket_ready(&setsds->sds[i]);
      // Good: data has been receive on this socket: clear it and increase socket ready count
      if(ret != 0) {
				SD_CLR(setsds->sds[i], setsds);
        n++;
				setsds->rcv_size = ret;
      }
  }
  return n;
}

int16_t recv_msg(uint8_t *socketID, uint8_t *sender_addr, uint16_t *sender_port,
                 uint8_t *data, uint16_t packets_size)
{
  packets_size = packets_size > OC_MAX_APP_DATA_SIZE ? OC_MAX_APP_DATA_SIZE : packets_size;
  return recvfrom(*socketID, (uint8_t *)data, packets_size + 1, sender_addr, sender_port);
}

OCResult_t ard_send_data(uint8_t socketID, uint8_t *dest_addr, 
                          uint16_t *dest_port, uint8_t *data, 
                          const uint16_t len)
{	
	uint8_t _socketID = socketID; // default client socket
	uint32_t ret = sendto(_socketID, data, len, dest_addr, *dest_port);
	if (ret <= 0)
		OC_ERR("SendData failed: %u", ret);
	return STATUS_OK;
} 
