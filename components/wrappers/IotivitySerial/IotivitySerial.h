#ifndef __IOTIVITYSERIAL_H__
#define __IOTIVITYSERIAL_H__
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

#define ARGS_SIZE(...) sizeof(#__VA_ARGS__)
#define ARGS(...) (#__VA_ARGS__) 

struct iotivitySerial;
typedef struct iotivitySerial iotivitySerial_t;
 
iotivitySerial_t *iotivitySerial_create();
void iotivitySerial_destroy(iotivitySerial_t *holder);
 

// allocate memory for Holder and serial object
static iotivitySerial_t *holder_ref = NULL;

void OCLogv(PROGMEM const char *level,const char *file, const char * func, 
                          const int lineNum, PROGMEM const char *format, uint8_t size,...);
                          
void OCLog(PROGMEM const char *level,const char *file, const char * func, 
                          const int lineNum, PROGMEM const char *logStr);                          

#define OIC_LOG_V(level, format, ...) \
    OCLogv(PCF(level),__FILE__,__func__,__LINE__,PCF(format), ARGS_SIZE(__VA_ARGS__),__VA_ARGS__) 
// here VA_ARGS is expected to be a string literal , we use VA_ARGS for compatibility with OC_LOG     
#define OIC_LOG(level, ...)    \
    OCLog(PCF(level),__FILE__,__func__,__LINE__, PCF(ARGS(__VA_ARGS__)))   
    

      
#ifdef __cplusplus
}
#endif
 
#endif /* __IOTIVITYSERIAL_H__ */