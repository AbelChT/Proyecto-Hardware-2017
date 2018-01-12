/*********************************************************************************************
* Fichero:	main.c
* Autor:
* Descrip:	punto de entrada de C
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "reversi8_2017.h"
#include "lcdManager.h"
#include "button_rebotes.h"
#include "tpManager.h"
#include "tp.h"
#include "timer1.h"
#include "exception_handler.h"
#include "timer2.h"
#include "pila.h"

void cambio_modo_usuario() {
    volatile int cpsr;
    asm("mrs %0,CPSR\n":"=r" (cpsr));
    cpsr = cpsr & ~(0x1f);
    cpsr = cpsr | 0x10;
    asm("msr CPSR,%0\n":"=r" (cpsr));
}

void inicializar_el_sistema() {
    /* Inicializa controladores */
    sys_init(); // Inicializacion de la placa, interrupciones y puertos
    Botones_anti_inicializar(); // inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
    init_exceptions();
    Init_LcdManager();
    timer1_inicializar();
    timer2_inicializar();
    TS_init();
    cambio_modo_usuario();
    init_pila_depuracion();
}

void fn_calibracion() {

    int i = 0;

    for (i = 1; i <= 4; i++) {
        LcdM_Show_Calibrar(i);
        while (estado_tp() == TP_NO_PULSADO) {}
    }

    for (i = 1; i <= 4; i++) {
        LcdM_Show_Calibrar(i);
        while (estado_tp() == TP_NO_PULSADO) {}
    }
}

void Main(void) {
    inicializar_el_sistema();
    fn_calibracion();
    reversi8();
}
