/*********************************************************************************************
* Fichero:	main.c
* Autor:	
* Descrip:	codigo de entrada de C
* Version:	<P6-ARM>	
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "lcd.h"

/*--- declaracion de funciones ---*/
void main(void);

/*--- funciones externas ---*/
extern void Lcd_Test();

/*--- codigo de la funcion ---*/

#define CASILLA_VACIA 0
#define FICHA_BLANCA  1
#define FICHA_NEGRA   2
#define DIM 8
char __attribute__ ((aligned (8))) tablero[DIM][DIM] = {
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {FICHA_NEGRA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,FICHA_BLANCA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
	    };

void Main(void)
{
    sys_init();        /* inicializacion de la placa, interrupciones, puertos y UART */
    Lcd_Init();
    //Lcd_mostrar_tablero_completo(tablero,12,13);
    Lcd_mostrar_tablero_zoom(tablero,1);
    //Lcd_actualizar_tiempo_ejecucion_tablero_completo(9);

	//Lcd_Test();        /* inicializacion LCD y visualizacion texto y rectangulos     */
	while(1);
}

