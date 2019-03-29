#include <Arduino.h>
#include "serial.h"
#include "HardwareSerial.h"
#define LOG_RATE 2
#define MAX_LOG_BUFFER_SIZE 500
serial_t *_serial_holder = NULL;
static uint8_t  log_num = 0;

serial_t *serial_create()
{
    serial_t *serial_holder;
    HardwareSerial *serial_ref;
    serial_holder = (typeof(serial_holder))malloc(sizeof(*serial_holder));
    serial_ref    = &Serial; // the serial object is on the global space get a ref
    serial_holder->serial = serial_ref;
    return serial_holder;
}
inline void clean_ref(HardwareSerial *serial_ref){
 serial_ref = NULL;
}

void serial_destroy(serial_t *serial_holder)
{
    if (serial_holder== NULL)
        return;
    
    #ifdef SERIAL_DYNAMIC
    delete static_cast<HardwareSerial *>(serial_holder->serial);
    #else
    clean_ref(static_cast<HardwareSerial *>(serial_holder->serial));
    #endif
    free(serial_holder);
}

void logV(PROGMEM const char *format, ...) {
  if(_serial_holder == NULL) {
    _serial_holder = serial_create();
  }
  HardwareSerial *serial_ref;
 
  if (_serial_holder == NULL)
      return;
  serial_ref = static_cast<HardwareSerial *>(_serial_holder->serial);
  do {  
      va_list ap;
      va_start(ap, format);
      uint16_t formatLength = strlen_P((PGM_P)format) ; // cast it to PGM_P , which is const char *
      if(formatLength == 0 ) return;
      char print_buffer[MAX_LOG_BUFFER_SIZE]; 
      vsnprintf_P(print_buffer, sizeof(print_buffer), (const char *)format, ap); 
      serial_ref->print(print_buffer);  
      va_end(ap); 
			log_num++;
  } while (0);
	if(log_num == LOG_RATE) {
		serial_ref->println();
		log_num = 0;
	}
}
