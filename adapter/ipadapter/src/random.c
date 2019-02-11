/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/
#include "port/oc_random.h"
#include "prng.h"
#include "oc_log.h"
#include "oc_helpers.h"


void
oc_random_init(void)
{
  iotConstrainedRand = prng_create();
  #ifdef ARDUINO_SERIAL
  //OIC_LOG_V("INFO","rand object initialization completed %s", "INIT") ;
  //OIC_LOG("INFO","rand object initialization completed");
  #endif
  
}

unsigned int
oc_random_value(void)
{
  if(iotConstrainedRand == NULL) {
    iotConstrainedRand = prng_create();
  }
 
  if (iotConstrainedRand == NULL)
      return 0;    
  return (unsigned int)prng_getRndInt(iotConstrainedRand);
}

void
oc_random_destroy(void)
{
  prng_destroy(iotConstrainedRand);
}