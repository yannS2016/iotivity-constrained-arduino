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
#include "oc_buffer.h"
#include "oc_endpoint.h"
#include "util/oc_process.h"
#include "util/oc_etimer.h"
#include "port/oc_connectivity.h"
#include "ard_eth_adptr_srv.h"


OC_PROCESS(ip_adapter_process, "IP Adapter");
static oc_endpoint_t *eps;

static void
free_endpoints(void)
{
  oc_endpoint_t *ep = eps, *next;
  while (ep != NULL) {
    next = ep->next;
    oc_free_endpoint(ep);
    ep = next;
  }
}
void
handle_incoming_message(uint8_t *addr, uint16_t *port, uint8_t *buffer, uint16_t size)
{
  oc_message_t *message = oc_allocate_message();
  if (message) 
	{
		size_t bytes_read = size;
		bytes_read = (bytes_read < OC_MAX_APP_DATA_SIZE) ? bytes_read : OC_MAX_APP_DATA_SIZE;
		memcpy(message->data, buffer, bytes_read);
		message->length = bytes_read;
    	message->endpoint.flags = IPV4;
    	memcpy(message->endpoint.addr.ipv4.address, addr, 16);
    	message->endpoint.addr.ipv4.port = *port;
		OC_DBG("Incoming message of size %d bytes from port %u:  ", message->length, *port);
    	OC_LOGipaddr(message->endpoint);
    	oc_network_event(message);
    	return;
	}
  	OC_WRN("ipadapter: No free RX/TX buffers to handle incoming message\n");
}

oc_endpoint_t *
oc_connectivity_get_endpoints(size_t device)
{
	(void)device;
	if (!eps) {
		oc_endpoint_t *ep = oc_new_endpoint();
		if (!ep) {
		return NULL;
		}
		memset(ep, 0, sizeof(oc_endpoint_t));
		ep->flags = IPV4;
		oc_ard_get_iface_addr(ep->addr.ipv4.address);
		ep->addr.ipv4.port = OCF_SERVER_PORT_UNSECURED;
		ep->device = 0;
		eps = ep;
	}
	return eps;
}


OC_PROCESS_THREAD(ip_adapter_process, ev, data)
{
  (void)data;
	static struct oc_etimer et;
  OC_PROCESS_BEGIN();
	uint16_t local_port = (uint16_t)OCF_SERVER_PORT_UNSECURED;      // local port to listen on
	uint16_t mcast_port = (uint16_t)OCF_MCAST_PORT_UNSECURED; 
	init_ip_context();
  	start_arduino_ucast_server(&local_port);
	#ifdef OC_SERVER
	start_arduino_mcast_server(OCF_IPv4_MULTICAST, &mcast_port, &mcast_port);
	OC_DBG("Initializing connectivity for arduino server");
	#endif
	#ifdef OC_CLIENT
	start_arduino_mcast_server(OCF_IPv4_MULTICAST, &mcast_port, &local_port);
	OC_DBG("Initializing connectivity for arduino client");
	#endif
	
  while (ev != OC_PROCESS_EVENT_EXIT) {
		oc_etimer_set(&et, (oc_clock_time_t)0.01);
		
		if(ev == OC_PROCESS_EVENT_INIT){
			set_ard_packet_recvcb(handle_incoming_message);
			OC_DBG("ipadapter: Initialized ip_adapter_process");
		}
		else if(ev == OC_PROCESS_EVENT_TIMER){
			ard_sock_poll_data();
		}
    OC_PROCESS_WAIT_EVENT();
  }
 OC_PROCESS_END();
}

int
oc_connectivity_init(size_t device)
{
  oc_process_start(&ip_adapter_process, NULL);
  return 0;
}

void
oc_connectivity_shutdown(size_t device)
{
	(void)device;
	ard_servers_shutdown();
	oc_process_exit(&ip_adapter_process);
}

void
oc_network_event_handler_mutex_init(void)
{

}

void
oc_network_event_handler_mutex_lock(void)
{

}

void
oc_network_event_handler_mutex_unlock(void)
{

}

void oc_network_event_handler_mutex_destroy(void) {}

int oc_send_buffer(oc_message_t *message) {
  PRINT("Outgoing message to ");
  PRINTipaddr(message->endpoint);
  PRINT("\n");
#ifdef OC_CLIENT									
 ard_send_data(message->endpoint.addr.ipv4.address, &message->endpoint.addr.ipv4.port, 
			   message->data, message->length, (message->endpoint.flags & DISCOVERY));
#else
	ard_send_data(message->endpoint.addr.ipv4.address, &message->endpoint.addr.ipv4.port, 
				  message->data, message->length);
#endif
	return message->length;
}

#ifdef OC_CLIENT
void
oc_send_discovery_request(oc_message_t *message)
{
	oc_send_buffer(message);	
}
#endif /* OC_CLIENT */
