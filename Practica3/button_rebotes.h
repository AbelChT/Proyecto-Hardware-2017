/*********************************************************************************************
* Fichero:	button.h
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

#ifndef _BUTTON_REBOTES_H_
#define _BUTTON_REBOTES_H_

#define BOTON_PULSADO_IZQUIERDA 2
#define BOTON_PULSADO_DERECHA 1
#define BOTON_NO_PULSADO 0

// Tiempo que tardan los rebotes iniciales medido en ---
#define TIEMPO_RETARDO_REBOTES 2462

// Frecuencia con la que se encuestará despues de los rebotes iniciales para saber si se ha soltado el botón
#define TIEMPO_ENCUESTA_PERIODICA 1845

// Tiempo que ha de pasar el boton mantenido para que se vea como una pulsacion
// medido en ---
#define TIEMPO_MANTENIDO_PULSACION 4

/*--- declaracion de funciones visibles del m�dulo button.c/button.h ---*/
void Botones_anti_inicializar(void);

int estado_botones(void);

#endif /* _BUTTON_REBOTES_H_ */
