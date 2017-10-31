#include "44b.h"
#include "44blib.h"
#include "exception_handler.h"

void exception_manager(void) __attribute__((interrupt("ABORT"))) __attribute__((interrupt("UNDEF")));

void exception_manager(void){
	int r =0;
}

void  init_exceptions(){
	pISR_UNDEF = (unsigned) exception_manager;
	pISR_SWI = (unsigned) exception_manager;
	pISR_PABORT = (unsigned) exception_manager;
	pISR_DABORT = (unsigned) exception_manager;
}
