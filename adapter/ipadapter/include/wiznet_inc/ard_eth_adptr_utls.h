/* ****************************************************************
 *
 * Copyright 2014 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/
/**
 * @file
 * This file provides APIs ethernet client/server/network monitor modules.
 */

#ifndef OC_ARD_ETH_ADPTR_UTILS_
#define OC_ARD_ETH_ADPTR_UTILS_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
//#include "utils.h"
typedef enum
{
	/** Success status code - START HERE.*/
	STATUS_OK = 0,
	STATUS_FAILED = -1,
	SOCKET_OPERATION_FAILED = -2,
	STATUS_INVALID_PARAM = 1,
	SERVER_STARTED_ALREADY = 2,
} OCResult_t;


/**
 * Get available UDP socket.
 * @param[out]   sockID         Available UDP socket ID.
 * @return  ::OC_STATUS_OK or Appropriate error code.
 */
extern OCResult_t arduino_get_free_socket(uint8_t *sockID);

/**
 * Initialize Unicast UDP socket.
 * @param[in/out]   port        Port to start the unicast server.
 * @param[out]      socketID    Unicast socket ID.
 * @return  ::OC_STATUS_OK or Appropriate error code.
 */
extern OCResult_t arduino_init_udp_socket(uint16_t *local_port, uint8_t *socketID);

/**
 * Initialize Multicast UDP socket.
 * @param[in]   mcastAddress     Port to start the unicast server.
 * @param[in]   mport            Multicast port.
 * @param[in]   lport            Local port on which the server is started.
 * @param[out]  socketID         Multicast socket ID.
 * @return  ::OC_STATUS_OK or Appropriate error code.
 */
																					
extern OCResult_t arduino_init_mcast_udp_socket(const char *mcast_addr, uint16_t *mcast_port, 
																						     uint16_t *local_port, uint8_t *socketID);																				
/**
 * To parse the IP address and port from "ipaddress:port".
 * @param[in]   ipAddrStr       IP address to be parsed.
 * @param[out]  ipAddr          Parsed IP address.
 * @param[in]   ipAddr          Buffer length for parsed IP address.
 * @param[out]  port            Parsed Port number.
 * @return ::CA_STATUS_OK or Appropriate error code.
 */
extern OCResult_t arduino_parse_IPv4_addr(const char *ipAddrStr, uint8_t *ipAddr,
                                      uint8_t ipAddrLen, uint16_t *port);
/**
 * set the socket ID for for unicast client mode.
 * @param[in]  socketID         Unicast socket ID.
 * @return  ::OC_STATUS_OK or Appropriate error code.
 */																	
extern OCResult_t set_ucast_socket(uint8_t *ucast_socketID);
/**
 * return the allocated unicast socket for client operation.
 * @param[in]  none.
 * @return  ::allocated unicast socket ID
 */	
 extern uint8_t get_ucast_socket();
 extern bool discovery_enable(bool discovery);
/**
 * set the socket ID for for multiicast client mode.
 * @param[in]  socketID         Multicast socket ID.
 * @return  ::OC_STATUS_OK or Appropriate error code.
 */	
extern OCResult_t set_mcast_socket(uint8_t *mcast_socketID);
/**
 * return the allocated multicast socket for discovery operation.
 * @param[in]  none.
 * @return  ::allocated nulticast socket ID.
 */	
 extern uint8_t get_mcast_socket();
/**
 * set the unicast send port for client mode.
 * @param[in]  ucast_port     unicast port.
 * @return  ::OC_STATUS_OK or Appropriate error code.
 */	
extern OCResult_t set_ucast_port(uint16_t *ucast_port);
/**
 * set the multicast send port for client mode.
 * @param[in]  mcast_socketID     multicast port.
 * @return  ::OC_STATUS_OK or Appropriate error code.
 */	
extern OCResult_t set_mcast_port(uint8_t *mcast_socketID);
/**
 * send response  client/server mode.
 * @param[in]  mcast_socketID     multicast port.
 * @return  ::OC_STATUS_OK or Appropriate error code.
 */	
/**
* Get the Interface Info(Allocated IP address)
* @param[in] address  endpoint ipv4 address
*/
OCResult_t oc_ard_get_iface_addr(uint8_t *address);


#ifdef OC_SERVER
extern OCResult_t ard_send_data(uint8_t *dest_addr, 
                               uint16_t *dest_port, 
															 uint8_t *data, const uint16_t len);	
#else 
extern OCResult_t ard_send_data(uint8_t *dest_addr, 
                               uint16_t *dest_port, 
															 uint8_t *data, const uint16_t len, 
                               uint8_t isMulticast);	
#endif															 
#ifdef __cplusplus
}
#endif

#endif /* OC_ARDUINO_ETHERNET_ADAPTER_UTILS_ */
