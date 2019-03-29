#ifndef __SERIAL_H__
#define __SERIAL_H__
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

typedef struct _serial {
    void *serial;
}serial_t;

// allocate memory for Holder and serial object
extern serial_t *_serial_holder;// = NULL;

void logV(PROGMEM const char *format, ...);
                          													

#define LOG_V(format, ...) logV(format,##__VA_ARGS__) 
    
#ifdef __cplusplus
}
#endif
 
#endif /* __SERIAL_H__ */