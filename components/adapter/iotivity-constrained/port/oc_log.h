/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#ifndef OC_LOG_H
#define OC_LOG_H

#include <stdio.h>
#ifdef ARDUINO_SERIAL
#include "IotivitySerial.h"
#define PRINT(format, ...) OIC_LOG_V("INFO" ,format, __VA_ARGS__) 
#else
#define PRINT(...) printf(__VA_ARGS__)
#endif



#define PRINTipaddr(endpoint)                                                  \
  do {                                                                         \
    if ((endpoint).flags & IPV4) {                                             \
      PRINT("[%d.%d.%d.%d]:%d", ((endpoint).addr.ipv4.address)[0],             \
            ((endpoint).addr.ipv4.address)[1],                                 \
            ((endpoint).addr.ipv4.address)[2],                                 \
            ((endpoint).addr.ipv4.address)[3], (endpoint).addr.ipv4.port);     \
    } else {                                                                   \
      PRINT(                                                                   \
        "[%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%"    \
        "02x%"                                                                 \
        "02x]:%d",                                                             \
        ((endpoint).addr.ipv6.address)[0], ((endpoint).addr.ipv6.address)[1],  \
        ((endpoint).addr.ipv6.address)[2], ((endpoint).addr.ipv6.address)[3],  \
        ((endpoint).addr.ipv6.address)[4], ((endpoint).addr.ipv6.address)[5],  \
        ((endpoint).addr.ipv6.address)[6], ((endpoint).addr.ipv6.address)[7],  \
        ((endpoint).addr.ipv6.address)[8], ((endpoint).addr.ipv6.address)[9],  \
        ((endpoint).addr.ipv6.address)[10],                                    \
        ((endpoint).addr.ipv6.address)[11],                                    \
        ((endpoint).addr.ipv6.address)[12],                                    \
        ((endpoint).addr.ipv6.address)[13],                                    \
        ((endpoint).addr.ipv6.address)[14],                                    \
        ((endpoint).addr.ipv6.address)[15], (endpoint).addr.ipv6.port);        \
    }                                                                          \
  } while (0)

#if OC_DEBUG                    
#define OC_LOG(level, ...)                                                     \
  do {                                                                         \
    PRINT("%s: %s <%s:%d>: ", level, __FILE__, __func__, __LINE__);            \
    PRINT(__VA_ARGS__);                                                        \
  } while (0)
#define OC_LOGipaddr(endpoint) PRINTipaddr(endpoint)
#define OC_DBG(...) OC_LOG("DEBUG", __VA_ARGS__)
#define OC_WRN(...) OC_LOG("WARNING", __VA_ARGS__)
#define OC_ERR(...) OC_LOG("ERROR", __VA_ARGS__)
#define OC_DBG_V(format, ...) OC_LOG_V("DEBUG", format,__VA_ARGS__)
#define OC_WRN_V(format, ...) OC_LOG_V("WARNING", format, __VA_ARGS__)
#define OC_ERR_V(format, ...) OIC_LOG_V("ERROR",format, __VA_ARGS__)
#else
#ifdef ARDUINO_SERIAL
#define OC_LOG(level, ...)            OIC_LOG(level ,__VA_ARGS__)     
#define OC_LOG_V(level, format ...)   OIC_LOG_V(level, format,__VA_ARGS__) 
#define OC_DBG(...)                   OIC_LOG("DEBUG", format,__VA_ARGS__)
#define OC_WRN(...)                   OIC_LOG("WARNING", format, __VA_ARGS__)
#define OC_ERR(...)                   OIC_LOG("ERROR",format, __VA_ARGS__)
#define OC_DBG_V(format, ...)         OIC_LOG_V("DEBUG", format,__VA_ARGS__)
#define OC_WRN_V(format, ...)         OIC_LOG_V("WARNING", format, __VA_ARGS__)
#define OC_ERR_V(format, ...)         OIC_LOG_V("ERROR",format, __VA_ARGS__)
#define OC_LOGipaddr(endpoint)        PRINTipaddr(endpoint)
#else
#define OC_LOG(level, ...)            
#define OC_LOG_V(level, format ...)    
#define OC_DBG(...)                  
#define OC_WRN(...)                   
#define OC_ERR(...)                   
#define OC_DBG_V(format, ...)         
#define OC_WRN_V(format, ...)         
#define OC_ERR_V(format, ...)         
#define OC_LOGipaddr(endpoint)        
#endif
#endif

#endif /* OC_LOG_H */