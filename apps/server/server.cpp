#include "../main.h"
#include "Ethernet2.h"
#include "oc_clock.h"
#include "util/oc_process.h"
#include "rs232.h"

/*Huawey ip details for mega:
* local ip address: 192.168.8.103
* subnet mask: 255.255.255.0
* gateway: 192.168.8.1
* dns: 192.168.8.1
*/

OC_PROCESS(sample_server_process, "server");
static bool state = false;
int power;
oc_string_t name;

static int
app_init(void)
{
  int ret = oc_init_platform(PCF("Intel"), NULL, NULL);
  ret |= oc_add_device(PCF("/oic/d"), PCF("oic.d.light"), PCF("Lamp"), PCF("ocf.1.0.0"),
                       PCF("ocf.res.1.0.0"), NULL, NULL);
  //_oc_new_string_(&name, PCF("Yann's Light"), 12);
  return ret;
}

static void
get_light(oc_request_t *request, oc_interface_mask_t interface, void *user_data)
{
  (void)user_data;
  ++power;

  OC_DBG("GET_light:\n");
  oc_rep_start_root_object();
  switch (interface) {
  case OC_IF_BASELINE:
    oc_process_baseline_interface(request->resource);
  /* fall through */
  case OC_IF_RW:
    oc_rep_set_boolean(root, state, state);
    oc_rep_set_int(root, power, power);
    oc_rep_set_text_string(root, name, oc_string(name));
    break;
  default:
    break;
  }
  oc_rep_end_root_object();
  oc_send_response(request, OC_STATUS_OK);
}

static void
post_light(oc_request_t *request, oc_interface_mask_t interface, void *user_data)
{
  (void)interface;
  (void)user_data;
  OC_DBG("POST_light:\n");
  oc_rep_t *rep = request->request_payload;
  while (rep != NULL) {
    OC_DBG(("key: %s "), oc_string(rep->name));
    switch (rep->type) {
    case OC_REP_BOOL:
      state = rep->value.boolean;
      OC_DBG("value: %d\n", state);
      break;
    case OC_REP_INT:
      power = rep->value.integer;
      OC_DBG("value: %d\n", power);
      break;
    case OC_REP_STRING:
      oc_free_string(&name);
      oc_new_string(&name, oc_string(rep->value.string),
                    oc_string_len(rep->value.string));
      break;
    default:
      oc_send_response(request, OC_STATUS_BAD_REQUEST);
      return;
      break;
    }
    rep = rep->next;
  }
  oc_send_response(request, OC_STATUS_CHANGED);
}

static void
put_light(oc_request_t *request, oc_interface_mask_t interface,
           void *user_data)
{
  (void)interface;
  (void)user_data;
  post_light(request, interface, user_data);
}

static void
register_resources(void)
{
  oc_resource_t *res = oc_new_resource(PCF("Yann's Light"), PCF("/a/light"), 2, 0);
  oc_resource_bind_resource_type(res, PCF("core.light"));
  oc_resource_bind_resource_type(res, PCF("core.brightlight"));
  oc_resource_bind_resource_interface(res, OC_IF_RW);
  oc_resource_set_default_interface(res, OC_IF_RW);
  oc_resource_set_discoverable(res, true);
  oc_resource_set_periodic_observable(res, 1);
  oc_resource_set_request_handler(res, OC_GET, get_light, NULL);
  oc_resource_set_request_handler(res, OC_PUT, put_light, NULL);
  oc_resource_set_request_handler(res, OC_POST, post_light, NULL);
  oc_add_resource(res);
}

static void
signal_event_loop(void)
{
  oc_process_post(&sample_server_process, OC_PROCESS_EVENT_TIMER, NULL);
}


OC_PROCESS_THREAD(sample_server_process, ev, data)
{
  (void)data;
  static struct oc_etimer et;
  static const oc_handler_t handler = {.init = app_init,
                                       .signal_event_loop = signal_event_loop,
                                       .register_resources = register_resources };
  static oc_clock_time_t next_event;
  OC_PROCESS_BEGIN();

  OC_DBG("Initializing server for arduino\n");
	
  while (ev != OC_PROCESS_EVENT_EXIT) {
		oc_etimer_set(&et, (oc_clock_time_t)next_event);
		
		if(ev == OC_PROCESS_EVENT_INIT){
			int init = oc_main_init(&handler);
			if (init < 0){
				OC_DBG("Server Init failed!\n");
				return init;
			}
      OC_DBG("Server process init!\n");
		}
		else if(ev == OC_PROCESS_EVENT_TIMER){
			OC_DBG("Timer event registered!\n");
			next_event = oc_main_poll();
			next_event -= oc_clock_time();
		}
    OC_PROCESS_WAIT_EVENT();
  }
 OC_PROCESS_END();
}
// Arduino Ethernet Shield
uint8_t ConnectToNetwork()
{
	// Note: ****Update the MAC address here with your shield's MAC address****
	uint8_t ETHERNET_MAC[] = {0x90, 0xA2, 0xDA, 0x11, 0x44, 0xA9};
	uint8_t error = Ethernet.begin(ETHERNET_MAC);
	if (error  == 0)
	{
		OC_ERR("Error connecting to Network: %d\n", error);
		return -1;
	}
  IPAddress ip = Ethernet.localIP();
  OC_DBG("Connected to Ethernet IP: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
	return 0;
}
void
init_lowlevel(void)
{
  rs232_init(USART_PORT, USART_BAUD,
             USART_PARITY_NONE | USART_STOP_BITS_1 | USART_DATA_BITS_8);
  rs232_redirect_stdout(USART_PORT);
  rs232_set_input(USART_PORT, NULL);
}
void setup() {
	
	init_lowlevel();
	delay(500);
	if (ConnectToNetwork() != 0)
	{
		OC_ERR("Unable to connect to network\n");
		return;
	}
	oc_process_start(&sample_server_process, NULL);
	OC_DBG("freememory: %d\n\n", freeMemory());
  delay(2000);
}

//main loop
void loop() {
	oc_process_run();
}