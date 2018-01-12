#include "timer1.h"
#include "44b.h"

volatile static int cuenta = 0;

/**
 * declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html
 * */
void timer1_ISR(void) __attribute__((interrupt("IRQ")));

void timer1_ISR(void) {
    ++cuenta;
    /* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
    rI_ISPC |= BIT_TIMER1;
}

/**
 * Pre: ---
 * Post: Inicializa el timer 1
 * */
void timer1_inicializar(void) {
    /* Establece la rutina de servicio para TIMER1 */
    pISR_TIMER1 = (unsigned) timer1_ISR;

    /* Configura el Timer1 */
    rTCFG0 = rTCFG0 | 0x255; // factor de preescalado maximo, para aumentar el retardo de los pulsos
    rTCFG1 = (rTCFG1 & ~(0xf0)) |
             0x40; // selecciona la entrada del mux que proporciona el reloj. La 10xx corresponde a un divisor de 1/32.
    rTCNTB1 = 16383;
    rTCMPB1 = 0; // valor de comparación
}

/**
 * Pre: El timer 1 ha de estar inicializado
 * Post: Comienza la cuenta del timer 1
 * */
void timer1_start(void) {
    rTCON = (rTCON & ~(0xf00)) | 0x200;
    rTCON = (rTCON & ~(0xf00)) | 0x900;
    rINTMSK = rINTMSK & ~(BIT_GLOBAL | BIT_TIMER1);
    cuenta = 0;
}

/**
 * Pre: Se ha de haber comenzado una cuenta con el timer 1
 * Post: Devuelve la cuenta del timer 1
 * */
int timer1_leer(void) {
    return cuenta;
}
