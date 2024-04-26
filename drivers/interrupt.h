#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

#define enable_irq() asm ("CPSIE I")
#define disable_irq() asm ("CPSID I")
#define set_primask(V) asm ("MSR primask, %0" : : "r" (V))

static int32_t get_primask(){
    uint32_t retval;
    asm volatile ("MRS %0, primask" : "=r" (retval) : );
    return retval;
}

#endif
