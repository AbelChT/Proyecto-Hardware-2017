#include "timer1.h"
#include "44b.h"

volatile static int cuenta = 0;
/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer1_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer1_ISR(void)
{
	++cuenta;
	/* borrar bit en I_ISPC para desactivar la solicitud de interrupci�n*/
	rI_ISPC |= BIT_TIMER1; // BIT_TIMER2 est� definido en 44b.h y pone un uno en el bit 13 que correponde al Timer2
}

void timer1_inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	//rINTMOD = 0x00; // Configura las linas como de tipo IRQ  8????
	//rINTCON = 0x0; // Habilita int. vectorizadas y la linea IRQ (FIQ no)

	/* Establece la rutina de servicio para TIMER2 */
	pISR_TIMER1 = (unsigned) timer1_ISR;

	/* Configura el Timer0 */
	rTCFG0 = rTCFG0 | 0x255; // factor de preescalado maximo, para aumentar el retardo de los pulsos
	rTCFG1 = (rTCFG1 & ~(0xf0)) | 0x40; // selecciona la entrada del mux que proporciona el reloj. La 10xx corresponde a un divisor de 1/32.
	rTCNTB1 = 16383;
	rTCMPB1 = 0; // valor de comparaci�n
}

void timer1_start(void)
{
	rTCON = (rTCON & ~(0xf00))| 0x200;
		/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	rTCON = (rTCON & ~(0xf00)) | 0x900;

	rINTMSK = rINTMSK & ~(BIT_GLOBAL | BIT_TIMER1);

	cuenta = 0;
}

// Ajustar para que la cuenta se aumente cada 250 ms
int timer1_leer(void){
	return cuenta;
}

void timer1_stop(void)
{
	rTCON = (rTCON & ~(0xf00));
	rINTMSK = rINTMSK | ( BIT_TIMER1);
}
