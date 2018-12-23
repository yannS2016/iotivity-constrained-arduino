#include <Arduino.h>
#include "IotivitySerial.h"
#include "HardwareSerial.h"
 
struct iotivitySerial {
//#ifdef SERIAL_DYNAMIC
    void *serial_object;

};

iotivitySerial_t *iotivitySerial_create()
{
    iotivitySerial_t *holder;
    HardwareSerial *serial_object;
    holder     = (typeof(holder))malloc(sizeof(*holder));
    #ifdef SERIAL_DYNAMIC
    serial_object    = Serial; // thus we already have a pointer
    #else
    serial_object    = &Serial; // the serial object is on the global space get a ref
    #endif
    holder->serial_object = serial_object;
 
    return holder;
}
inline void clean_ref(HardwareSerial *serial_object){
 serial_object = NULL;
}

void iotivitySerial_destroy(iotivitySerial_t *holder)
{
    if (holder== NULL)
        return;
    
    #ifdef SERIAL_DYNAMIC
    delete static_cast<HardwareSerial *>(holder->serial_object);
    #else
    clean_ref(static_cast<HardwareSerial *>(holder->serial_object));
    #endif
    free(holder);
}

void OCLogv(PROGMEM const char *level,const char *file, const char * func, 
                          const int lineNum, PROGMEM const char *format, uint8_t size,...) {
  if(holder_ref == NULL) {
    holder_ref = iotivitySerial_create();
  }
  HardwareSerial *serial_object;
 
  if (holder_ref == NULL)
      return;
  serial_object = static_cast<HardwareSerial *>(holder_ref->serial_object);
  do {  
      va_list ap;
      va_start(ap, format);
      uint8_t formatLength = strlen_P((PGM_P)format) ; // cast it to PGM_P , which is const char *
      uint8_t levelLength = strlen_P((PGM_P)level) ; // cast it to PGM_P , which is const char *
      if(formatLength == 0 || levelLength == 0) return;
      char print_buffer[size + formatLength + levelLength]; 
      strcpy_P(print_buffer, level); //copy the string 
      serial_object->print(print_buffer);
      serial_object->print(F(" : "));                                   
      serial_object->print(file);                                   
      serial_object->print(F(" : "));                                   
      serial_object->print(func);                                   
      serial_object->print(F(" : "));                                   
      serial_object->print(lineNum);                                   
      serial_object->print(F(" : "));
      vsnprintf_P(print_buffer, sizeof(print_buffer), (const char *)format, ap); 
      serial_object->println(print_buffer);  
      va_end(ap);      
  } while (0);
}
void OCLog(PROGMEM const char *level,const char *file, const char * func, 
                          const int lineNum, PROGMEM const char *logStr){
  if(holder_ref == NULL) {
    holder_ref = iotivitySerial_create();
  }
  HardwareSerial *serial_object;
 
  if (holder_ref == NULL)
      return;
  serial_object = static_cast<HardwareSerial *>(holder_ref->serial_object);
  do {  
      uint8_t logStrLength = strlen_P((PGM_P)logStr) ; // cast it to PGM_P , which is const char *
      uint8_t levelLength = strlen_P((PGM_P)level) ; // cast it to PGM_P , which is const char *
      if(logStrLength == 0 || levelLength == 0) return;
      char print_buffer[logStrLength + levelLength] ; 
      strcpy_P(print_buffer, level); //copy the string 
      serial_object->print(print_buffer);
      serial_object->print(F(" : "));                                   
      serial_object->print(file);                                   
      serial_object->print(F(" : "));                                   
      serial_object->print(func);                                   
      serial_object->print(F(" : "));                                   
      serial_object->print(lineNum);                                   
      serial_object->print(F(" : "));
      strcpy_P(print_buffer, logStr); //copy the string 
      serial_object->println(print_buffer);    
  } while (0);

}
