#include "44b.h"
#include "exception_handler.h"
#include "8led.h"
#include "led.h"
#include "44blib.h"
#include "pila.h"

void exception_manager(void) __attribute__((interrupt("UNDEF"))) __attribute__((interrupt("DABORT")));
void exception_manager_pabort(void) __attribute__((interrupt("PABORT")));
void exception_manager_save(unsigned int type, unsigned int lr);

#define TYPE_UNDEF 0;
#define TYPE_PREFETCH_ABORT 1;
#define TYPE_DATA_ABORT 2;
#define TYPE_SWI 3;

void exception_manager(void) {
    unsigned int cpsr, type, lr;
    asm("mov %0,lr\n":"=r" (lr));
    asm("mrs %0,CPSR\n":"=r" (cpsr));

    int mask = cpsr & 0x1f;

    switch (mask) {
        case 0x17:
            type = TYPE_DATA_ABORT;
            break;
        case 0x13:
            type = TYPE_SWI;
            break;
        default:
            type = TYPE_UNDEF;
    }

    exception_manager_save(type, lr);
}

void exception_manager_pabort(void) {
    unsigned int lr, type;
    asm("mov %0,lr\n":"=r" (lr));
    type = TYPE_PREFETCH_ABORT;
    exception_manager_save(type, lr);
}


void exception_manager_save(unsigned int type, unsigned int lr) {
    push_debug(type, lr);
    D8Led_symbol(type);
    int i = 0;
    while (1) {
        if (i == 0) {
            D8Led_symbol(type);
            led1_on();
            led2_off();
            i = 1;
        } else {
            D8Led_symbol(8);
            led2_on();
            led1_off();
            i = 0;
        }
        DelayTime(100);
    }
}

void init_exceptions() {
    pISR_UNDEF = (unsigned) exception_manager;
    pISR_PABORT = (unsigned) exception_manager_pabort;
    pISR_SWI = (unsigned) exception_manager;
    pISR_DABORT = (unsigned) exception_manager;
}
