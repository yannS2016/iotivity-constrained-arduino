#ifdef OC_SECURITY
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "port/oc_storage.h"
#include "port/oc_log.h"
#include "sdfat.h"
#define STORE_PATH_SIZE 20
// SD chip select pin
#ifdef __AVR__
const uint8_t chipSelect = 4;
#else 
const uint8_t chipSelect = SDCARD_SS_PIN;
#endif


static char store_path[STORE_PATH_SIZE];
static int8_t store_path_len;
static bool path_set = false;

void list_dir(){
// Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sdfile_open_read(_file_holder,"/", O_RDONLY)) {
    sdfat_errorHalt(_sd_holder, "open root failed");
  }
  // list all files in the card with date and size
  sdfile_ls(_file_holder);
}

int oc_storage_config(const char *store)
{
   OC_WRN("Initializing SD card...");
  store_path_len = strlen(store);
  if (store_path_len > STORE_PATH_SIZE){
    OC_WRN("Error in directory length %d...", store_path_len);
    return -ENOENT;
  }
  strncpy(store_path, store, store_path_len);
  store_path[store_path_len] = '\0';
  OC_WRN("path : %s", store_path);
  _sd_holder = sdfat_create(); 
  path_set = 0;
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sdfat_begin(_sd_holder, chipSelect)) {
    sdfat_initErrorHalt(_sd_holder);
    return -1;
  }
  OC_WRN("initialization done.");
  // see if the directory exists, create it if not.
  
  if( !sdfat_exists(_sd_holder, store_path)) 
  {
    if( sdfat_mkdir(_sd_holder, store_path) ) 
    {
      OC_ERR("Credential directory: %s created", store_path);
    } 
    else {
      OC_ERR("Error creating credential directory");
    }
  }
  _file_holder = sdfile_create(); 
  //uint8_t mydata[215] = {0};
  //oc_storage_write("doxm_0", mydata, sizeof(mydata));
  list_dir();
  sdfile_close(_file_holder);
  return 0;
}


long
oc_storage_write(const char *store, uint8_t *buf, size_t len)
{

  size_t store_len = strlen(store);
  //OC_WRN("Store length %s:%d", store, store_len);
//  if (!path_set || (store_len + store_path_len >= STORE_PATH_SIZE)) {
  if (path_set) {
    OC_WRN("Problem....");
    return -ENOENT;
  }
  //OC_WRN("All Good....");
  
  store_path[store_path_len] = '/';
  //OC_WRN("path so far: %s", store_path);
  strncpy(store_path + store_path_len + 1, store, store_len);
  store_path[1 + store_path_len + store_len] = '\0';
  OC_WRN("path : %s", store_path);
  //OC_WRN("Writing security file: %s", store_path);
  sdfile_open_write(_file_holder, store_path, O_WRONLY | O_CREAT | O_TRUNC);
  if(!sdfile_isOpen(_file_holder)) {
    OC_ERR("error opening %s", store_path);
    return -1;
  }else {
      if((len  =  sdfile_write(_file_holder, buf, len)) == -1) {
          OC_ERR("Error writing to: %s",store_path );
         // return -1;
      }
      //OC_WRN("Bytes written: %d", len);
      sdfile_close(_file_holder);
  }
  //list_dir();
 // delay(10);
  return len;
}

long
oc_storage_read(const char *store, uint8_t *buf, size_t len)
{

  size_t store_len = strlen(store);
  OC_WRN("trying to read file %s", store);
  if (path_set || (1 + store_len + store_path_len >= STORE_PATH_SIZE))
    return -ENOENT;
  //OC_WRN("file to open %s", store);
  store_path[store_path_len] = '/';
  strncpy(store_path + store_path_len + 1, store, store_len);
  store_path[1 + store_path_len + store_len] = '\0';
  sdfile_open_read(_file_holder, store_path,  O_RDONLY);
  OC_WRN("Ready to read file %s", store);
  if(!sdfile_isOpen(_file_holder)) {
    OC_ERR("error opening %s", store_path);
    return -1;
  }
  OC_WRN("Reading a file");
  while(sdfile_available(_file_holder)){
    if((len  =  sdfile_read(_file_holder, buf, len)) == -1) {
      OC_ERR("Error reading from: %s",store_path );
    } 
  }
  sdfile_close(_file_holder);
  //list_dir();
  return len;
}
#endif /* OC_SECURITY */
