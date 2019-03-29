#ifdef OC_SECURITY
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "port/oc_storage.h"
#include "port/oc_log.h"
#include "sdfat.h"
#define STORE_PATH_SIZE 20
// SD chip select pin
const uint8_t chipSelect = 4;



static char store_path[STORE_PATH_SIZE];
static int8_t store_path_len;
static bool path_set = false;

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
  path_set = true;
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
  list_dir();
  return 0;
}

void list_dir(){
// Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sdfile_open_read(_file_holder,"/", O_RDONLY)) {
    sdfat_errorHalt(_sd_holder, "open root failed");
  }
  // list all files in the card with date and size
  sdfile_ls(_file_holder);
}

long
oc_storage_write(const char *store, uint8_t *buf, size_t len)
{

  size_t store_len = strlen(store);

  if (!path_set || (store_len + store_path_len >= STORE_PATH_SIZE))
    return -ENOENT;
  OC_WRN("path : %s", store_path);
  store_path[store_path_len] = '/';
  OC_WRN("path so far: %s", store_path);
  strncpy(store_path + store_path_len + 1, store, store_len);
  store_path[1 + store_path_len + store_len] = '\0';

  OC_WRN("Writing security file: %s", store_path);
  sdfile_open_write(_file_holder, store_path, O_WRONLY | O_CREAT | O_TRUNC);
  if(!sdfile_isOpen(_file_holder)) {
    OC_ERR("error opening %s", store_path);
    //return -1;
  }else {
      //len  =  wrfile.write(buf, len);
      if((len  =  sdfile_write(_file_holder, buf, len)) == -1) {
          OC_ERR("Error writing to: %s",store_path );
         // return -1;
      }
      OC_WRN("Bytes written: %d", len);
      sdfile_close(_file_holder);
  }
  /*
  SdFile wrfile(store_path, O_WRONLY | O_CREAT | O_TRUNC);
  if (!wrfile.isOpen()) {
     OC_ERR("error opening %s", store_path);
  } else {
      //len  =  wrfile.write(buf, len);
      if((len  =  wrfile.write(buf, len)) == -1) {
          OC_ERR("Error writing to: %s",store_path );
      }
      OC_WRN("Bytes written: %d", len);
      wrfile.close();
  }*/
  return len;
}

long
oc_storage_read(const char *store, uint8_t *buf, size_t len)
{

  size_t store_len = strlen(store);
   OC_ERR("LENGHT BEFORE: %d",len );
  if (!path_set || (1 + store_len + store_path_len >= STORE_PATH_SIZE))
    return -ENOENT;
  OC_WRN("file to open %s", store);
  store_path[store_path_len] = '/';
  strncpy(store_path + store_path_len + 1, store, store_len);
  store_path[1 + store_path_len + store_len] = '\0';
  OC_WRN("Reading security file: %s", store_path);
  sdfile_open_read(_file_holder, store_path,  O_RDONLY);

  if(!sdfile_isOpen(_file_holder)) {
    OC_ERR("error opening %s", store_path);
    //return -1;
  }else {
      while(sdfile_available(_file_holder)){
        if((len  =  sdfile_read(_file_holder, buf, len)) == -1) {
          OC_ERR("Error reading from: %s",store_path );
        } 
      }
       OC_ERR("lenght: %d",len );
      sdfile_close(_file_holder);
  }
/*
 
  // check for open error
  if (!rdfile.isOpen()) {
   OC_ERR("error opening %s", store_path);
   return -1;
  } else {
      while (rdfile.available()) {
        if((len =  rdfile.read(buf, len)) == -1 ) {
          OC_ERR("Error reading: %s",store_path );
        }
      } 
    rdfile.close(); 
  }*/
  list_dir();
  return len;
}
#endif /* OC_SECURITY */
