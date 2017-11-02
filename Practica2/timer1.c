/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "timer1.h"
#include "44b.h"
#include "44blib.h"


/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer1_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer1_ISR(void)
{
  // Hacer lo que sea
	switch (estado_botones) {

    case REBOTES_INICIALES:
      estado_botones = MANTENIDO;
        rTCNTB1 = 520;// el timer saltara unas 15 veces por segundo

  		rTCON =   0x200; 
  		rTCON =  0x900 ;
      break;

    case MANTENIDO:
    	if( se ha soltado){
    		estado_botones = REBOTES_FINALES;
    		rTCNTB1 = 1562;// el timer saltara a los 0,2 s

  			rTCON =   0x200; 
  			rTCON =  0x900 ; 
    	}
      break;

    case REBOTES_FINALES:
      estado_botones = NO_PULSADO;
      // Habilito las excepciones
      rINTMSK    = rINTMSK & ~( BIT_EINT4567 );

      //Elimino interrupciones timer
      rTCON = rTCON & ~(0x1 << 8);
      break;

  }


	/* borrar bit en I_ISPC para desactivar la solicitud de interrupci�n*/
	rI_ISPC |= BIT_TIMER1; // BIT_TIMER2 est� definido en 44b.h y pone un uno en el bit 13 que correponde al Timer2
}

void timer1_inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK = rINTMSK & ~(BIT_GLOBAL | BIT_TIMER1); // Emascara todas las lineas excepto Timer2 y el bit global (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 est�n definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER2 */
	pISR_TIMER1 = (unsigned) timer1_ISR;

	/* Configura el Timer0 */
	rTCFG0 = rTCFG0 | 0x255; // factor de preescalado maximo, para aumentar el retardo de los pulsos
	rTCFG1 = (rTCFG1 & ~(0xf0)) | 0x40; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB1 = 65535;// valor inicial de cuenta (la cuenta es descendente)
	rTCMPB1 = 200;// valor de comparaci�n
  
}


void timer1_gestionar_rebotes_iniciales(void){
  rTCNTB1 = 1562;// el timer saltara a los 0,2 s

  rTCON =   0x200; 
  rTCON =  0x900 ;
}

void timer1_encuesta_mantenido(void){
  rTCNTB1 = 520;// el timer saltara unas 15 veces por segundo

  rTCON =   0x200; 
  rTCON =  0x900 ;
}

void timer1_gestionar_rebotes_finales(void){
  rTCNTB1 = 1562;// el timer saltara a los 0,2 s

  rTCON =   0x200; 
  rTCON =  0x900 ; 
}
