#include "44b.h"
#include "exception_handler.h"
#include "8led.h"
#include "led.h"
#include "44blib.h"
#include "pila.h"

void exception_manager(void) __attribute__((interrupt("UNDEF"))) __attribute__((interrupt("PABORT")));
void exception_manager_dabort(void) __attribute__((interrupt("DABORT")));


typedef enum {DABORT, PABORT, UNDEF} tipo_interrupcion;
volatile int IS_DABORT=0;
const int UNDEF_=0;
const int PABORT_=1;
const int DABORT_=2;
const int SWI_=3;
const int CPSR_=0x1f;
const int ABORT_B=0x17;
const int SWI=0x13;

unsigned int lr;


void exception_manager(void){
	unsigned int cpsr, type, debug_lr;

	if (IS_DABORT==0){
		asm("mov %0,lr\n":"=r" (lr));
	}
	debug_lr=lr;
	asm("mrs %0,CPSR\n":"=r" (cpsr));
	int mask=cpsr & CPSR_;
	if (mask==ABORT_B){
		if(IS_DABORT==1){
			//DABORT
			type=DABORT_;
			IS_DABORT=0;
		} else {
			//PABORT
			type=PABORT_;
		}
	}
	else if (mask==SWI){
		type=SWI_;
	}
	else {
		//UNDEF
		type=UNDEF_;
	}
	push_debug(type,lr);
	D8Led_symbol(type);
	int i=0;
	while (1){
		if(i==0){
			D8Led_symbol(type);
			led1_on(); led2_off();
			i=1;
		} else {
			D8Led_symbol(8);
			led2_on(); led1_off();
			i=0;
		}
		Delay(100);
	}
}

void exception_manager_dabort(void){
	asm("mov %0,lr\n":"=r" (lr));
	IS_DABORT=1;
	exception_manager();
}

void  init_exceptions(){
	pISR_UNDEF = (unsigned) exception_manager;
	pISR_PABORT = (unsigned) exception_manager;
	pISR_SWI = (unsigned) exception_manager;
	pISR_DABORT = (unsigned) exception_manager_dabort;
}
