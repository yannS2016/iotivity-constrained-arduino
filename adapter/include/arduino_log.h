#ifndef __ARDUINOLOG_H__
#define __ARDUINOLOG_H__

#include "Arduino.h"
#ifdef __cplusplus
#define PCF(str)  ((PROGMEM const char *)(F(str)))
#else
#define PCF(str)  ((PROGMEM const char *)(PSTR(str)))
#endif


 
#endif /* __ARDUINOLOG_H__ */
