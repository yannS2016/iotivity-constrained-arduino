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

#include "Wiz5500.h"
#include "oc_log.h"
#include "oc_connectivity.h"
#include "ard_eth_adptr_srv.h"
#include "port/oc_assert.h"

static ard_eth_udp_callback ard_eth_packet_recvcb = NULL;

/**
 * @brief Flag to check if multicast server is started
 */
bool arduino_mcast_serv_started = false;
static uint8_t active_sockets[MAX_SOCK_NUM];
static uint8_t num_active_sockets;


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
	num_active_sockets++;
	active_sockets[num_active_sockets - 1] = serverFD;
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
	{
		return STATUS_FAILED;
	}
	num_active_sockets++;
	active_sockets[num_active_sockets - 1] = serverFD;
  return serverFD;	
}

void set_ard_packet_recvcb(ard_eth_udp_callback cb){
	ard_eth_packet_recvcb = cb;
}

void ard_sock_poll_data()
{
	for(int i = 0; i < num_active_sockets ; i++){
		if (ard_sock_get_data(&active_sockets[i])!= STATUS_OK){
			OC_ERR(("rcv fail"));
		}
	}
}
OCResult_t ard_sock_get_data(uint8_t *socketID)
{

	uint8_t sender_addr[4] = { 0 };
	uint16_t sender_port = 0;
	// check if we have a working ref of w5500
	if(!wiznet5500) {
		wiznet5500 = wiz5500_create();
		if(!wiznet5500)
			return STATUS_FAILED;
	}
	uint16_t recvLen = wiz5500_getRXReceivedSize(wiznet5500, socketID);
	if (recvLen == 0)
	{
		return STATUS_OK;
	}
  recvLen = recvLen > OC_MAX_APP_DATA_SIZE ? OC_MAX_APP_DATA_SIZE : recvLen;
	uint8_t data[recvLen + 1];
  	// Read available data.
	int16_t ret = recvfrom(*socketID, (uint8_t *)data, recvLen + 1, sender_addr, &sender_port);
	if (ret < 0)
	{
		OC_ERR(("rcv fail"));
		return STATUS_FAILED;
	}
	else if (ret > 0)
	{
		if(ard_eth_packet_recvcb){
			ard_eth_packet_recvcb((uint8_t *)sender_addr, &sender_port, data, ret);
		}
	}
	return STATUS_OK;
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
    //OC_WRN("NO Reception on socket %d", socketID);
    return 0;
  } else {
    //OC_WRN("Reception on socket %d:%d", socketID, recvLen);
    return recvLen;
  }
}

uint8_t select(uint8_t nsds, sdset_t *setsds){
  uint8_t n = 0;
  //OC_WRN("active descriptors %d", nsds);
  //OC_WRN("Active socket in list: %d:%d",setsds->sds[1], setsds->sds[2]);
  for(int i = 1; i < nsds; i++){
      uint16_t ret = socket_ready(&setsds->sds[i]);
      // Good: data has been receive on this socket: clear it and increase socket ready count
      if(ret != 0) {
          OC_WRN("Reception on socket %d", setsds->sds[i]); 
          SD_CLR(setsds->sds[i], setsds);
          OC_WRN("Clearing socket after reception %d", setsds->sdsset);
          setsds->ready_sds++;
          setsds->rcv_size = ret;
      }
  }
  n = setsds->ready_sds;
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
	{
		OC_ERR("SendData failed: %u", ret);
	}
	return STATUS_OK;
} 
