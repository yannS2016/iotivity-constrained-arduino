/******************************************************************
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
#ifndef ARD_ETH_ADPTR_SRV_
#define ARD_ETH_ADPTR_SRV_

#include "ard_eth_adptr_utls.h"

#ifdef __cplusplus
extern "C"
{
#endif
typedef void (* ard_eth_udp_callback)(uint8_t *sender_addr, uint16_t *sender_port,
                                     uint8_t *data, const uint16_t dataLength);
                                    
OCResult_t start_arduino_servers();
OCResult_t ard_ucast_server_shutdown();                                       
OCResult_t ard_mcast_server_shutdown();
void ard_servers_shutdown();

OCResult_t start_arduino_ucast_server(uint16_t *local_port);

OCResult_t start_arduino_mcast_server(const char *mcast_addr, uint16_t *mcast_port, uint16_t *local_port);	

                                        
// definitions for hadling the reception of data
// and com with the itovity constrained stack
void ard_sock_poll_data();
/** Retrieve any available data from UDP socket and call callback.
 *  This is a non-blocking call.
 */
OCResult_t ard_sock_get_data(uint8_t *socketID);
// call this method from user init code
//OCResult_t arduino_recv_data(uint8_t *sockFd);                          
void set_ard_packet_recvcb(ard_eth_udp_callback cb);


#ifdef __cplusplus
}
#endif

#endif /* ARD_ETH_ADPTR_SRV */
