#ifndef __PRNG_H__
#define __PRNG_H__
 
#ifdef __cplusplus
extern "C" {
#endif

uint8_t rand_setSeed(rand_t *m);
uint16_t rand_getRndInt(rand_t *m);
uint32_t rand_getRndLong(rand_t *m);
 
#ifdef __cplusplus
}
#endif
 
#endif /* __PRNG_H__ */
