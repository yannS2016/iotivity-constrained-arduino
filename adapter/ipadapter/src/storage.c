#include "port/oc_storage.h"
#include "port/oc_log.h"
#ifdef OC_SECURITY
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <SdFat.h>
// https://hackingmajenkoblog.wordpress.com/2016/03/25/fast-efficient-data-storage-on-an-arduino/
/*
https://gerrit.iotivity.org/gerrit/#/c/22513/
https://lists.iotivity.org/g/iotivity-dev/message/8939?p=,,,20,0,0,0::Created,,iotivity+node,20,2,0,18970665
https://dzone.com/articles/keeping-the-javascript-based-iot-devices-secure
https://urlf.duocircle.io/?url=https%3A%2F%2F01.org%2Fblogs%2Fttzeng%2F2017%2Fsecurely-accessing-iot-devices-based-javascript&id=31d5&rcpt=ralshafi@...&tss=1513593475&msgid=8131ebd8-e3df-11e7-8fcd-5f906d21262c&html=1&h=7e525f59
*/
#define STORE_PATH_SIZE 8

static char store_path[STORE_PATH_SIZE];
static int8_t store_path_len;
static bool path_set = false;

int8_t
oc_storage_config(const char *store)
{
  store_path_len = strlen(store);
  if (store_path_len >= STORE_PATH_SIZE)
    return -ENOENT;

  strncpy(store_path, store, store_path_len);
  store_path[store_path_len] = '\0';
  path_set = true;
  OC_WRN("SECURITY FILE STORAGE IS: %s", store_path);
  return 0;
}

long
oc_storage_read(const char *store, uint8_t *buf, size_t size)
{
  FILE *fp = 0;
  size_t store_len = strlen(store);

  if (!path_set || (1 + store_len + store_path_len >= STORE_PATH_SIZE))
    return -ENOENT;

  store_path[store_path_len] = '/';
  strncpy(store_path + store_path_len + 1, store, store_len);
  store_path[1 + store_path_len + store_len] = '\0';
	OC_WRN("FILE TO OPEN IS: %s", store_path);


  fp = fopen(store_path, "rb");
  if (!fp)
    return -EINVAL;

  size = fread(buf, 1, size, fp);
  fclose(fp);
  return size;
}

long
oc_storage_write(const char *store, uint8_t *buf, size_t size)
{
  FILE *fp;
  size_t store_len = strlen(store);

  if (!path_set || (store_len + store_path_len >= STORE_PATH_SIZE))
    return -ENOENT;

  store_path[store_path_len] = '/';
  strncpy(store_path + store_path_len + 1, store, store_len);
  store_path[1 + store_path_len + store_len] = '\0';
  fp = fopen(store_path, "wb");
  if (!fp)
    return -EINVAL;

  size = fwrite(buf, 1, size, fp);
  fclose(fp);
  return size;
}
#endif /* OC_SECURITY */
