#include "port/oc_storage.h"
#include "port/oc_log.h"
#ifdef OC_SECURITY
#include <errno.h>
#include <stdio.h>
#include <string.h>
/*#include <SdFat.h>
#include <SdFatConfig.h>
#include <sdios.h>
#include <SysCall.h>*/
// https://hackingmajenkoblog.wordpress.com/2016/03/25/fast-efficient-data-storage-on-an-arduino/
/*
https://gerrit.iotivity.org/gerrit/#/c/22513/
https://lists.iotivity.org/g/iotivity-dev/message/8939?p=,,,20,0,0,0::Created,,iotivity+node,20,2,0,18970665
https://dzone.com/articles/keeping-the-javascript-based-iot-devices-secure
https://urlf.duocircle.io/?url=https%3A%2F%2F01.org%2Fblogs%2Fttzeng%2F2017%2Fsecurely-accessing-iot-devices-based-javascript&id=31d5&rcpt=ralshafi@...&tss=1513593475&msgid=8131ebd8-e3df-11e7-8fcd-5f906d21262c&html=1&h=7e525f59
*/
#define STORE_PATH_SIZE 20
// SD chip select pin
const uint8_t chipSelect = 4;

//SdFat *sd;
//SdFile root;

static char store_path[STORE_PATH_SIZE];
static int8_t store_path_len;
static bool path_set = false;

int oc_storage_config(const char *store)
{
/*  OC_WRN("Initializing SD card...");
  store_path_len = strlen(store);
  if (store_path_len > STORE_PATH_SIZE){
    OC_WRN("Error in directory length %d...", store_path_len);
    return -ENOENT;
  }
  strncpy(store_path, store, store_path_len);
  store_path[store_path_len] = '\0';
  OC_WRN("path : %s", store_path);
  path_set = true;
  sd = new SdFat();
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd->begin(chipSelect, SD_SCK_MHZ(50))) {
    sd->initErrorHalt();
  }
  OC_WRN("initialization done.");
// see if the directory exists, create it if not.
  if( !sd->exists(store_path) ) 
  {
    if( sd->mkdir(store_path) ) 
    {
      OC_ERR("Credential directory:  %s created", store_path);
    } 
    else {
      OC_ERR("Error creating credential directory");
    }
  }
  list_dir();*/
  return 0;
}

void list_dir(){
// Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
/*  if (!root.open("/")) {
    sd->errorHalt("open root failed");
  }
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);*/
}

long
oc_storage_write(const char *store, uint8_t *buf, size_t len)
{

 /* size_t store_len = strlen(store);

  if (!path_set || (store_len + store_path_len >= STORE_PATH_SIZE))
    return -ENOENT;
  OC_WRN("path : %s", store_path);
  store_path[store_path_len] = '/';
  OC_WRN("path so far: %s", store_path);
  strncpy(store_path + store_path_len + 1, store, store_len);
  store_path[1 + store_path_len + store_len] = '\0';

  OC_WRN("Writing security file: %s", store_path);
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

 /* size_t store_len = strlen(store);

  if (!path_set || (1 + store_len + store_path_len >= STORE_PATH_SIZE))
    return -ENOENT;

  store_path[store_path_len] = '/';
  strncpy(store_path + store_path_len + 1, store, store_len);
  store_path[1 + store_path_len + store_len] = '\0';
  OC_WRN("Reading security file: %s", store_path);
  SdFile rdfile(store_path, O_RDONLY);
 
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
  return len;
}
#endif /* OC_SECURITY */
