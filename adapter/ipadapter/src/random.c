/******************************************************************
*
* Copyright 2018 iThemba LABS All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at

*    http://www.apache.org/licenses/LICENSE-2.0

* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
******************************************************************/

#include "port/oc_random.h"
#include "oc_log.h"
#include "oc_helpers.h"

#ifdef __AVR__
#include "prng.h"
#else
#include <WMath.h>
#endif 

void
oc_random_init(void)
{
#ifdef __AVR__
  iotConstrainedRand = prng_create();
#else
  //randomSeed(analogRead(0));
#endif

}

unsigned int
oc_random_value(void)
{
#ifdef __AVR__
  if(iotConstrainedRand == NULL) {
    iotConstrainedRand = prng_create();
  }
 
  if (iotConstrainedRand == NULL)
      return 0;    
  return (unsigned int)prng_getRndInt(iotConstrainedRand);
#else
  return 0;
#endif
}

void
oc_random_destroy(void)
{
#ifdef __AVR__
  prng_destroy(iotConstrainedRand);
#endif
}
