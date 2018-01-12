/*********************************************************************************************
* Fichero:	main.c
* Autor:
* Descrip:	punto de entrada de C
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "reversi8_2017.h"

/*--- variables globales ---*/
#define USAR_PLACA	// Variable que se define para ejecutar el código de emulación sin placa

void Main(void)
{
#ifdef USAR_PLACA
	reversi8_placa();

#else
	reversi8();

#endif

}
