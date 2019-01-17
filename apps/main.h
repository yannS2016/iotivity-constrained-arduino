#ifdef __cplusplus
/*
* Treat oc_log.h as cpp header because it includes Arduino.h which deals with classes, 
* function overloading which c will fail to process and return  multiple definition or inknown type
*/
//#include "oc_log.h" // wont need to include this: just call IotivitySerial.h
// we declare this headers inside extern so to enabling linking of c code insde cpp

#include "FreeMemory.h"
extern "C" {
#endif
// remove warning: ISO C++11 requires at least one argument for the "..." in a variadic macro
//#pragma GCC system_header 
#include "oc_api.h"
#include "utils.h"

//#include "oc_log.h" // wont need to include this: just call IotivitySerial.h
#ifdef __cplusplus
}
#endif
