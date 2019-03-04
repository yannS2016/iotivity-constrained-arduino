#include "arduino_log.h"
#include <stdarg.h>
char *logstr = NULL;
void OCLogv(PROGMEM const char *level,const char *file, const char * func, 
                          const int lineNum, PROGMEM const char *format, ...){
  va_list ap;
  va_start(ap, format);
  do {   

    if(!logstr) {
      logstr = (char*)malloc( 50 * sizeof(char));
      if(!logstr) {
        return -1;
      }
    }

    //strcpy_P(logstr, (PGM_P)level); //copy the string in using strcpy_P
    printf_P("%s", (PGM_P)level);
    printf_P( (PGM_P)format, ap);
    va_end(ap);
  } while (0);
}