/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer3.h"
#include "44b.h"
#include "44blib.h"

/*--- variables privadas ---*/
static int timer3_num_int;

/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer3_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer3_ISR(void)
{
  // Hacer lo que sea
  timer3_num_int = timer3_num_int + 1;

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupci�n*/
	rI_ISPC |= BIT_TIMER3; // BIT_TIMER2 est� definido en 44b.h y pone un uno en el bit 13 que correponde al Timer2
}

void timer3_inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK = rINTMSK & ~(BIT_GLOBAL | BIT_TIMER3); // Emascara todas las lineas excepto Timer2 y el bit global (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 est�n definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER2 */
	pISR_TIMER2 = (unsigned) timer3_ISR;

	/* Configura el Timer0 */
	rTCFG0 = 0x0; // factor de preescalado minimo, para aumentar la precisión
	rTCFG1 = 0x0; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB3 = 65535;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB3 = 12800;// valor de comparaci�n
  
}


void timer3_gestionar_rebotes_iniciales(void){
  rTCNTB3 = 65535;// valor inicial de cuenta (la cuenta es descendente)

  rTCON =   0x2000; // Cambiar por numeros timer 3
 	rTCON =  0x9000 ; // Cambiar por numeros timer 3
}

void timer3_encuesta_mantenido(void){
  rTCNTB3 = 65535;// valor inicial de cuenta (la cuenta es descendente)

  rTCON =   0x2000; // Cambiar por numeros timer 3
  rTCON =  0x9000 ; // Cambiar por numeros timer 3
}

void timer3_gestionar_rebotes_finales(void){
  rTCNTB3 = 65535;// valor inicial de cuenta (la cuenta es descendente)

  rTCON =   0x2000; // Cambiar por numeros timer 3
  rTCON =  0x9000 ; // Cambiar por numeros timer 3
}
