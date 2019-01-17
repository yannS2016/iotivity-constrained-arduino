#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdint.h>
#include <stdlib.h> 
#include "Arduino.h" 

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
#define PCF(str)  ((PROGMEM const char *)(F(str)))
#else
#define PCF(str)  ((PROGMEM const char *)(PSTR(str)))
#endif
    
#ifdef __cplusplus
}
#endif
 
#endif /* __UTILS_H__ */
