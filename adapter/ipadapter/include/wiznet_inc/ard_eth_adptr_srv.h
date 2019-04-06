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
#ifndef ARD_ETH_ADPTR_SRV_
#define ARD_ETH_ADPTR_SRV_

#include <socket.h>
#include "ard_eth_adptr_utls.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct  sdset_t {
  uint8_t sdsset;
  uint8_t sds[MAX_SOCK_NUM];
  uint8_t ready_sds;
  uint16_t rcv_size;
} sdset_t;

#define SETSIZE (8)
#define SD_ZERO(_setsds) (((sdset_t*)_setsds)->sdsset = 0 )
#define SD_SET(sd,_setsds)																	\
do {																													\
	((sdset_t*)_setsds)->sds[sd] = sd; 											\
	((sdset_t*)_setsds)->sdsset |= (1 << (sd % SETSIZE));	\
} while(0)
#define SD_CLR(sd, _setsds)   (((sdset_t*)_setsds)->sdsset &= ~(1 << (sd % SETSIZE)))
#define SD_ISSET(sd, _setsds) (((sdset_t*)_setsds)->sdsset & (1 << (sd % SETSIZE)))
uint8_t select(uint8_t nsds, sdset_t *setsds);
int16_t recv_msg(uint8_t *socketID, uint8_t *sender_addr, 
				uint16_t *sender_port, uint8_t *data, uint16_t packets_size);
                                
uint8_t start_udp_server(uint16_t *local_port);

uint8_t start_udp_mcast_server(const char *mcast_addr, uint16_t *mcast_port, uint16_t *local_port);	
                                     
#ifdef __cplusplus
}
#endif

#endif /* ARD_ETH_ADPTR_SRV */
