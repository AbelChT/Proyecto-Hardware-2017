/*********************************************************************************************
* Fichero:	button.c
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "button_rebotes.h"
#include "44b.h"
#include "pila.h"


/*--- variables globales del mï¿½dulo ---*/
/* int_count la utilizamos para sacar un nï¿½mero por el 8led.
  Cuando se pulsa un botï¿½n sumamos y con el otro restamos. ï¿½A veces hay rebotes! */
static unsigned int int_count = 0;

typedef enum {ESPERANDO, PULSADO, MANTENIDO, REBOTES_INICIALES, REBOTES_FINALES} estados_boton;

typedef enum {DERECHO, IZQUIERDO, NINGUNO} posiciones_boton;


static estados_boton estado_actual;
static posiciones_boton boton_pulsado;
static int interrupciones_mantenido;
static int se_ha_pulsado;

/* declaraciï¿½n de funciï¿½n que es rutina de servicio de interrupciï¿½n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void Botones_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de funciones ---*/
void Botones_ISR(void)
{
	/* Identificar la interrupcion (hay dos pulsadores)*/
/*
	int cpsr;
	asm("mrs %0,CPSR\n":"=r" (cpsr));
	cpsr = cpsr & 0x3f;
	asm("msr CPSR,%0\n":"=r" (cpsr));
*/
	int i = rPDATG;
	int which_int = rEXTINTPND;

	//push_debug(60,which_int);

	switch (which_int)
	{
		case 4:
			if(int_count < 8)
				int_count++; // incrementamos el contador
			else int_count = 0;

			boton_pulsado = IZQUIERDO;
			break;
		case 8:
			//if(int_count > 0)
			//int_count--; // decrementamos el contador
			boton_pulsado = DERECHO;
			break;
		default:
			break;
	}

	estado_actual = PULSADO;

	gestor_anti_rebotes();


	/* Finalizar ISR */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND

	se_ha_pulsado = 1;

}

void Botones_anti_inicializar(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros estï¿½n definidos en 44b.h */
	rI_ISPC    = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD    = 0x00;		// Configura las linas como de tipo IRQ
	rINTCON    = 0x0;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK    = rINTMSK & ~(BIT_GLOBAL | BIT_EINT4567 ); // Enmascara todas las lineas excepto eint4567, el bit global y el timer0

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int) Botones_ISR;

	/* Configuracion del puerto G */
	rPCONG  = 0xffff;        		// Establece la funcion de los pines (EINT0-7)
	rPUPG   = 0x0;                  // Habilita el "pull up" del puerto
	rEXTINT = rEXTINT | 0x22222222;   // Configura las lineas de int. como de flanco de bajada

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rI_ISPC    |= (BIT_EINT4567);
	rEXTINTPND = 0xf;

  // inicializa el timer que se encargarÃ¡ de gestionar los rebotes
  //timer1_inicializar();

	estado_actual = ESPERANDO;

	timer0_inicializar();

	init_pila_depuracion();

	se_ha_pulsado = 0;
}

void resetCounter(){
	int_count=0;
}


/* declaraciï¿½n de funciï¿½n que es rutina de servicio de interrupciï¿½n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer0_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer0_ISR(void)
{/*
	int cpsr;
	asm("mrs %0,CPSR\n":"=r" (cpsr));
	cpsr = cpsr & 0xef;
	asm("msr CPSR,%0\n":"=r" (cpsr));*/

	gestor_anti_rebotes();

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupciï¿½n*/
	rI_ISPC |= BIT_TIMER0; // BIT_TIMER2 estï¿½ definido en 44b.h y pone un uno en el bit 13 que correponde al Timer2
}

void timer0_inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x00; // Configura las linas como de tipo IRQ  8????
	rINTCON = 0x0; // Habilita int. vectorizadas y la linea IRQ (FIQ no)

	//rINTMSK = rINTMSK & ~( BIT_TIMER0); // Emascara todas las lineas excepto Timer2 y el bit global (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 estï¿½n definidos en 44b.h)

	/* Establece la rutina de servicio para TIMER2 */
	pISR_TIMER0 = (unsigned) timer0_ISR;

	/* Configura el Timer0 */
	rTCFG0 = rTCFG0 | 0x255; // factor de preescalado maximo, para aumentar el retardo de los pulsos
	rTCFG1 = (rTCFG1 & ~(0xf)) | 0x4; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
	rTCNTB0 = 65535; // valor inicial de cuenta (la cuenta es descendente)
	rTCMPB0 = 200; // valor de comparaciï¿½n

  //estado_botones = REBOTES_INICIALES;

}

void gestor_anti_rebotes(void){
	switch(estado_actual){
	case PULSADO:
			// Se inicializa el timer
			rINTMSK = rINTMSK & ~(BIT_GLOBAL | BIT_TIMER0); // Emascara todas las lineas excepto Timer2 y el bit global (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 estï¿½n definidos en 44b.h)

		   // rINTMSK = rINTMSK & ~(BIT_TIMER0);

		    rTCNTB0 = 2462;// el timer saltara a los 0,2 s

		   	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/

			rTCON = (rTCON & ~(0xf)) | 0x2;
			/* iniciar timer (bit 0) con auto-reload (bit 3)*/
			rTCON = (rTCON & ~(0xf)) | 0x09;
			rINTMSK    = rINTMSK  | ( BIT_EINT4567 );
			estado_actual = REBOTES_INICIALES;
			interrupciones_mantenido = 0;


		break;
	case REBOTES_INICIALES:
	        rTCNTB0 = 1845;// el timer saltara unas 15 veces por segundo

	    	/* establecer update=manual (bit 1) + inverter=on (¿? será inverter off un cero en el bit 2 pone el inverter en off)*/

			rTCON = (rTCON & ~(0xf)) | 0x2;
			/* iniciar timer (bit 0) con auto-reload (bit 3)*/
			rTCON = (rTCON & ~(0xf)) | 0x09;

			estado_actual = MANTENIDO;

			//push_debug(61,1);
	      break;

	    case MANTENIDO:



	    	if(((boton_pulsado == DERECHO) && ((rPDATG & (0x1 << 7)) != 0)) || ((boton_pulsado == IZQUIERDO) &&((rPDATG & (0x1 << 6)) != 0))){ // Si alguna de las posiciones es igual a 0 implica que esta pulsado
	    		rTCNTB0 = 2462;// el timer saltara a los 0,2 s

				rTCON = (rTCON & ~(0xf)) | 0x2;
				/* iniciar timer (bit 0) con auto-reload (bit 3)*/
				rTCON = (rTCON & ~(0xf)) | 0x09;

				estado_actual = REBOTES_FINALES;
	    	}else if(interrupciones_mantenido == 4){
	    		interrupciones_mantenido=0;
	    	   // push_debug(68,rPDATG);

	    	    if((boton_pulsado == IZQUIERDO) && (int_count < 8))
	    	      int_count++; // incrementamos el contador
	    	    else int_count = 0;

	    	    //if((boton_pulsado == DERECHO) && (int_count > 0))
	    	    //  int_count--; // incrementamos el contador

	    	}
	    	else {
	    		 ++ interrupciones_mantenido;
	    		// push_debug(62,rPDATG);
	    	}

	      break;

	    case REBOTES_FINALES:

	    	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	    	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND

	      // Habilito las excepciones
	      rINTMSK    = rINTMSK & ~( BIT_EINT4567 );

	      rINTMSK = rINTMSK | ( BIT_TIMER0);

	      //Elimino interrupciones timer

	      rTCON = rTCON & ~(0xf);

	      estado_actual = ESPERANDO;
	      boton_pulsado = NINGUNO;

	      //push_debug(63,1);

	      break;

	    case ESPERANDO:
	    break;
	}
}

int get_contador(void){
	return int_count;
}

// Devuelve 1 solo la primera vez
int pulsado_izquierda(void){
	if((boton_pulsado==IZQUIERDO) && (se_ha_pulsado == 1)){
		se_ha_pulsado = 0;
		return 1;
	}else{
		return 0;
	}
}

// Devuelve 1 solo la primera vez
int pulsado_derecha(void){
	if((boton_pulsado==DERECHO) && (se_ha_pulsado == 1)){
		se_ha_pulsado = 0;
		return 1;
	}else{
		return 0;
	}
}
