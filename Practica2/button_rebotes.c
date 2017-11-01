/*********************************************************************************************
* Fichero:	button.c
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "button.h"
#include "8led.h"
#include "44blib.h"
#include "44b.h"
#include "def.h"

// Distintos estados del boton
typedef enum {NO_PULSADO, MANTENIDO, REBOTES_INICIALES, REBOTES_FINALES} estados_boton;

/*--- variables globales del m�dulo ---*/
/* int_count la utilizamos para sacar un n�mero por el 8led.
  Cuando se pulsa un bot�n sumamos y con el otro restamos. �A veces hay rebotes! */
static unsigned int int_count = 0;

static estados_boton estado_botones = NO_PULSADO;

/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de funciones ---*/
void Eint4567_ISR(void)
{
	/* Identificar la interrupcion (hay dos pulsadores)*/
	int which_int = rEXTINTPND;

	switch (which_int)
	{
		case 4:
			int_count++; // incrementamos el contador
			break;
		case 8:
			int_count--; // decrementamos el contador
			break;
		default:
			break;
	}

  // Se llama a gestorRebotes
  maquina_de_estados_gestor_rebotes();

	/* Finalizar ISR */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
}

void Eint4567_init(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros est�n definidos en 44b.h */
	rI_ISPC    = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD    = 0x0;		// Configura las linas como de tipo IRQ
	rINTCON    = 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK    = rINTMSK & ~(BIT_GLOBAL | BIT_EINT4567 ); // Enmascara todas las lineas excepto eint4567, el bit global y el timer0

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int) Eint4567_ISR;

	/* Configuracion del puerto G */
	rPCONG  = 0xffff;        		// Establece la funcion de los pines (EINT0-7)
	rPUPG   = 0x0;                  // Habilita el "pull up" del puerto
	rEXTINT = rEXTINT | 0x22222222;   // Configura las lineas de int. como de flanco de bajada

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rI_ISPC    |= (BIT_EINT4567);
	rEXTINTPND = 0xf;

  // inicializa el timer que se encargará de gestionar los rebotes
  timer3_init();
}

void maquina_de_estados_gestor_rebotes(void){
  switch (estado_botones) {
    case NO_PULSADO:
      // Inhabilitacion de las excepciones
      rINTMSK    = rINTMSK & ( BIT_EINT4567 );
      estado_botones = REBOTES_INICIALES;
      timer3_gestionar_rebotes_iniciales();
      break;

    case REBOTES_INICIALES:
      estado_botones = MANTENIDO;
      timer3_encuesta_mantenido();
      break;

    case MANTENIDO:
      estado_botones = REBOTES_FINALES;
      timer3_gestionar_rebotes_finales();
      break;

    case REBOTES_FINALES:
      estado_botones = NO_PULSADO;
      // Habilito las excepciones
      rINTMSK    = rINTMSK & ~( BIT_EINT4567 );
      break;

  }

}
