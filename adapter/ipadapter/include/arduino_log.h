#ifndef __ARDUINOLOG_H__
#define __ARDUINOLOG_H__

#include "Arduino.h"
#ifdef __cplusplus
#define PCF(str)  ((PROGMEM const char *)(F(str)))
#else
#define PCF(str)  ((PROGMEM const char *)(PSTR(str)))
#endif

//extern char *logstr;

void OCLogv(PROGMEM const char *level,const char *file, const char * func, 
                          const int lineNum, PROGMEM const char *format, ...);
 
#endif /* __ARDUINOLOG_H__ */
