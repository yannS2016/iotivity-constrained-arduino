#include <Arduino.h>
//#include <stdint.h>
#include "prng.h"
#include "pRNG.h"
#include <stdio.h>
struct prng {
    void *cpp_ref;
};
prng_t *iotConstrainedRand = NULL;

prng_t *prng_create()
{
    prng_t *cpp_ref_holder;
    pRNG *cpp_ref;
    
    cpp_ref_holder     = (typeof(cpp_ref_holder))malloc(sizeof(*cpp_ref_holder));
    cpp_ref    = new pRNG();
    cpp_ref_holder->cpp_ref = cpp_ref;
    //printf_P(PSTR("\r\nInit Random\r\n"));
    return cpp_ref_holder;
}
 
void prng_destroy(prng_t *cpp_ref_holder)
{
    if (cpp_ref_holder== NULL)
        return;
    delete static_cast<pRNG *>(cpp_ref_holder->cpp_ref);
    free(cpp_ref_holder);
}
uint8_t prng_getRndByte(prng_t *cpp_ref_holder){

    pRNG *cpp_ref;
 
    if (cpp_ref_holder== NULL)
        return 1;
 
    cpp_ref = static_cast<pRNG *>(cpp_ref_holder->cpp_ref);
    return cpp_ref->getRndByte();
}

uint16_t prng_getRndInt(prng_t *cpp_ref_holder){

    pRNG *cpp_ref;
 
    if (cpp_ref_holder== NULL)
        return 1;
 
    cpp_ref = static_cast<pRNG *>(cpp_ref_holder->cpp_ref);
    return cpp_ref->getRndInt();
}

uint32_t prng_getRndLong(prng_t *cpp_ref_holder){

    pRNG *cpp_ref;
 
    if (cpp_ref_holder== NULL)
        return 1;
 
    cpp_ref = static_cast<pRNG *>(cpp_ref_holder->cpp_ref);
    return cpp_ref->getRndLong();
}