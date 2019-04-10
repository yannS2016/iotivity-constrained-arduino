#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

extern unsigned int __heap_start;
extern void *__brkval;

/*
 * The free list structure as maintained by the 
 * avr-libc memory allocation routines.
 */
typedef struct __freelist {
  size_t sz;
  struct __freelist *nx;
} FREELIST;
 
extern FREELIST *__flp;

/**
 * Get the total memory used by your program. The total will
 * include accounting overhead internal to the library
 */
size_t getMemoryUsed()
{
  size_t used;
  FREELIST *fp;
 
// __brkval=0 if nothing has been allocated yet
 
  if(__brkval==0)
    return 0;
 
// __brkval moves up from __malloc_heap_start to
// __malloc_heap_end as memory is used
 
  used=static_cast<char *>(__brkval)-__malloc_heap_start;
 
// memory free'd by you is collected in the free list and
// compacted with adjacent blocks. This, combined with malloc's
// intelligent picking of candidate blocks drastically reduces
// heap fragmentation. Anyway, since blocks in the free list
// are available to you at no cost we need to take them off.
 
  for(fp=__flp;fp;fp=fp->nx)
    used-=fp->sz+sizeof(size_t);
 
  return used;
}

size_t freeMemory()
{

  return (size_t)AVR_STACK_POINTER_REG-
         (size_t)__malloc_margin-
         (size_t)__malloc_heap_start-
         getMemoryUsed();
}
 