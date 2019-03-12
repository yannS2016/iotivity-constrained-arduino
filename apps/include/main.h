#ifdef __cplusplus
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef OC_XMEM
#include "FreeMemory.h"
#endif
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
#ifdef OC_SECURITY
#include "rsa_internal.h"
#include "ssl_internal.h"
#endif
#ifdef __cplusplus
}
#endif