/*********************************************************************************************
* Fichero:	main.c
* Autor:
* Descrip:	punto de entrada de C
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/


void init_pila_depuracion();
void push_debug(int ID_evento, int auxData);
void pop_debug(int *IDevento, int *auxData, int *time);