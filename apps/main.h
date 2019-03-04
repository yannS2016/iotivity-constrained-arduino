#ifdef __cplusplus
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
/*
* Treat oc_log.h as cpp header because it includes Arduino.h which deals with classes, 
* function overloading which c will fail to process and return  multiple definition or inknown type
*/
//#include "oc_log.h" // wont need to include this: just call IotivitySerial.h
// we declare this headers inside extern so to enabling linking of c code insde cpp
//#include "FreeMemory.h"
extern "C" {
#endif
#include "oc_api.h"
#include "oc_assert.h"
#include "oc_storage.h"
#include "oc_random.h"
#include "oc_network_events_mutex.h"
#include "oc_connectivity.h"
#include "oc_clock.h"
#include "rs232.h"
#include "util/oc_process.h"
#ifdef __cplusplus
}
#endif
