/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer2.h"
#include "44b.h"
#include "44blib.h"

/*--- variables privadas ---*/
static int timer2_num_int;

/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer2_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer2_ISR(void)
{
  // Hacer lo que sea
  timer2_num_int = timer2_num_int + 1;

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupci�n*/
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER2 est� definido en 44b.h y pone un uno en el bit 13 que correponde al Timer2
}

void timer2_inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK = rINTMSK & ~(BIT_GLOBAL | BIT_TIMER2); // Emascara todas las lineas excepto Timer2 y el bit global (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 est�n definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER2 */
	pISR_TIMER2 = (unsigned) timer2_ISR;

	/* Configura el Timer0 */
	rTCFG0 = 0x0; // factor de preescalado minimo, para aumentar la precisión
	rTCFG1 = 0x0; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB2 = 65535;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB2 = 12800;// valor de comparaci�n
	/* establecer update=manual (bit 1) + inverter=on (�? ser� inverter off un cero en el bit 2 pone el inverter en off)*/
	//rTCON = 0x2;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/
	//rTCON = 0x09;
  // Lo hago de vez para evitar posible error
  //rTCON = (0x1 << 13) | (0x1 << 15);
	//rTCON = (0x1 << 13);
	//rTCON = (0x1 << 15);
	rTCON =   0x2000;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/

	rTCON =  0x9000 ;

  timer2_num_int = 0;
}
void timer2_empezar(void){

	rTCNTB2 = 65535;// valor inicial de cuenta (la cuenta es descendente)


  /* Configura el Timer0 */
	rTCON =   0x2000;
	/* iniciar timer (bit 0) con auto-reload (bit 3)*/

	rTCON =  0x9000 ;
	timer2_num_int=0;
}

int timer2_leer(void){
  int timer_actual = timer2_num_int * 65535;
  timer_actual = timer_actual + (65535 - rTCNTB2 );
  /* Si durante la operación ha cambiado timer2_num_int, implica que se ha terminado
     la cuenta y que se ha activado la subrutina */
  if(timer2_num_int * 65535 > timer_actual) return (timer2_num_int * 65535)/32;
  else return timer_actual/32;
  //return timer_actual;
  //return timer2_num_int;
}
