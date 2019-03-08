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


#ifdef OC_DYNAMIC_ALLOCATION
ip_context_t *dev;
#else /* OC_DYNAMIC_ALLOCATION */
static ip_context_t device ;
static ip_context_t *dev;
#endif /* !OC_DYNAMIC_ALLOCATION */

static ard_eth_udp_callback ard_eth_packet_recvcb = NULL;

/**
 * @brief Flag to check if multicast server is started
 */
bool arduino_mcast_serv_started = false;

void init_ip_context() {
#ifdef OC_DYNAMIC_ALLOCATION
  dev = (ip_context_t *)calloc(1, sizeof(ip_context_t));
  if (!dev) {
    oc_abort("Insufficient memory");
  }
#else  /* OC_DYNAMIC_ALLOCATION */
  dev = &device;
#endif /* !OC_DYNAMIC_ALLOCATION */
}

OCResult_t start_arduino_ucast_server(uint16_t *local_port)
{
	if(!local_port) {
	  OC_DBG(("server port null!"));
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
		OC_DBG("ucast sock init failed");
		return STATUS_FAILED;
	}

	dev->arduino_ucast_socket = serverFD;
	dev-> arduino_ucast_port = *local_port;
	dev->num_active_sockets++;
	dev->active_sockets[dev->num_active_sockets - 1] = serverFD;

    OC_DBG("Unicast server started! socket info: socket: %d: port: %u", dev->arduino_ucast_socket, dev->arduino_ucast_port);
	return STATUS_OK;
}
OCResult_t start_arduino_mcast_server(const char *mcast_addr, uint16_t *mcast_port, uint16_t *local_port)
{
	if (arduino_mcast_serv_started == true)
	{
	  return SERVER_STARTED_ALREADY;
	}
	uint8_t serverFD = 1; 
	if (arduino_init_mcast_udp_socket(mcast_addr, mcast_port, local_port, &serverFD)!= STATUS_OK)
	{
		OC_DBG("mcast serv init failed");
		return STATUS_FAILED;
	}
	dev->arduino_mcast_socket = serverFD;
	dev->arduino_mcast_port = *local_port;
	dev->num_active_sockets++;
	dev->active_sockets[dev->num_active_sockets - 1] = serverFD;
    OC_DBG("multicast server started! socket info: socket: %d: port: %u", dev->arduino_mcast_socket, dev->arduino_mcast_port);
	return STATUS_OK;	
}
#ifdef OC_SERVER
OCResult_t start_arduino_servers(){
	
	uint16_t ucast_port = OCF_SERVER_PORT_UNSECURED;
	uint16_t mcast_port = OCF_MCAST_PORT_UNSECURED;
	const char *mcast_ipaddr = (const char *)OCF_IPv4_MULTICAST;
	OCResult_t ret = start_arduino_ucast_server(&ucast_port);
	if (STATUS_OK != ret)
	{
		OC_DBG(("Start ucast serv failed[%d]"), ret);
		return ret;
	}
	ret = start_arduino_mcast_server(mcast_ipaddr, &mcast_port, &mcast_port);
	if (STATUS_OK != ret)
	{
		OC_DBG(("Start mcast failed[%d]"), ret);
	}
	return ret;	
}
#endif
OCResult_t ard_ucast_server_shutdown()
{
	close(dev->arduino_ucast_socket);
	dev->arduino_ucast_socket = 0;
	return STATUS_OK;
}

OCResult_t ard_mcast_server_shutdown()
{
	//OC_DBG(("Stop mcast serv!"));
	close(dev->arduino_mcast_socket);
	dev->arduino_mcast_socket = 0;
	return STATUS_OK;
}

void ard_servers_shutdown()
{
	OCResult_t result = ard_ucast_server_shutdown();
	if (STATUS_OK != result)
	{
		OC_ERR(("stop ucast srv fail:%d"), result);
		return;
	}
  result = ard_mcast_server_shutdown();
	if (STATUS_OK != result)
	{
		OC_ERR(("stop mcast srv fail:%d"), result);
		return;
	}
}

void set_ard_packet_recvcb(ard_eth_udp_callback cb){
	ard_eth_packet_recvcb = cb;
}

void ard_sock_poll_data()
{
	for(int i = 0; i < dev->num_active_sockets ; i++){
		if (ard_sock_get_data(&dev->active_sockets[i])!= STATUS_OK){
			OC_ERR(("rcv fail"));
		}
	}
}

OCResult_t ard_sock_get_data(uint8_t *socketID)
{
	/**Bug : When there are multiple UDP packets in Wiznet buffer, w5500.getRXReceivedSize
	* will not return correct length of the first packet.
	* Fix : Use the patch provided for arduino/libraries/Ethernet/utility/socket.cpp
	*/
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
	OC_WRN("Completed reception!!!");
	return STATUS_OK;
}

uint8_t get_mcast_socket() {
	if(!dev->arduino_mcast_socket){
	 return STATUS_FAILED;
	}
  	return dev->arduino_mcast_socket;	
}
 
uint8_t get_ucast_socket() {
	if(!dev->arduino_ucast_socket){
	 	return STATUS_FAILED;
	}
	return dev->arduino_ucast_socket;	
}
 
#ifdef OC_SERVER
OCResult_t ard_send_data(uint8_t *dest_addr, uint16_t *dest_port, 
																 uint8_t *data, uint16_t len)
{	
	uint8_t socketID = get_ucast_socket(); // default client socket
	OC_WRN("send server resp to: %d.%d.%d.%d:%u",dest_addr[0],dest_addr[1], dest_addr[2],dest_addr[3], *dest_port);
	uint32_t ret = sendto(socketID, data, len, dest_addr, *dest_port);
	if (ret <= 0)
	{
		OC_ERR("SendData failed: %u", ret);
	}
	return STATUS_OK;
} 
#endif

#ifdef OC_CLIENT
OCResult_t 
ard_send_data(uint8_t *dest_addr, uint16_t *dest_port, 
						uint8_t *data, uint16_t len, uint8_t isMulticast)
{	
  uint8_t socketID = get_ucast_socket();
	uint16_t port = *dest_port; // port of listening servers
	if(isMulticast){
		OC_WRN("Init Discovery request %d", isMulticast);
		port = (uint16_t)OCF_MCAST_PORT_UNSECURED;	
		socketID = get_mcast_socket();
		OC_DBG("send multicast resquest to %u:", port);
	}
	else {
		OC_DBG("send unicast request to %u:", port);
	}
	uint32_t ret = sendto(socketID, data, len, dest_addr, port);
	if (ret <= 0)
	{
		OC_ERR("Data send failed: %u", ret);
	}
	return STATUS_OK;
} 
#endif									