#ifndef CONFIG_H
#define CONFIG_H

/* Time resolution */
#include <stdint.h>
#include <TimeLib.h>

#define OCF_MCAST_PORT_UNSECURED (5683)
#define OCF_SERVER_PORT_UNSECURED (56789)
/** Multicast IP address.*/
#define OCF_IPv4_MULTICAST      "224.0.1.187"
//arduino does not define a 64 bits int
typedef uint32_t oc_clock_time_t;
/* 1 clock tick = 1ms or 1 ticks   1000 ticks -- 1 s --- 1000 ticks --- 1000ms --- 1 tick -- 1 ms*/
#define OC_CLOCK_CONF_TICKS_PER_SECOND (1)
#define OC_BYTES_POOL_SIZE (600)
#define OC_INTS_POOL_SIZE (100)
#define OC_DOUBLES_POOL_SIZE (4)

/* Server-side parameters */
/* Maximum number of server resources */
#define OC_MAX_APP_RESOURCES (15)

// uncomment if using collections 
//#define OC_MAX_NUM_COLLECTIONS (10)

/* Common paramters */
/* Prescriptive lower layers MTU size, enable block-wise transfers */
#define OC_BLOCK_WISE_SET_MTU (700)

/* Maximum size of request/response payloads */
//#define OC_MAX_APP_DATA_SIZE (1024)

/* Maximum number of concurrent requests */
#define OC_MAX_NUM_CONCURRENT_REQUESTS (3)

/* Maximum number of nodes in a payload tree structure */
#define OC_MAX_NUM_REP_OBJECTS (150)

/* Number of devices on the OCF platform */
#define OC_MAX_NUM_DEVICES (2)

/* Maximum number of endpoints */
#define OC_MAX_NUM_ENDPOINTS (20)

/* Security layer */
/* Maximum number of authorized clients */
#define OC_MAX_NUM_SUBJECTS (2)

/* Maximum number of concurrent DTLS sessions */
#define OC_MAX_DTLS_PEERS (1)

/* Max inactivity timeout before tearing down DTLS connection */
#define OC_DTLS_INACTIVITY_TIMEOUT (10)


#endif /* CONFIG_H */
