/*********************************************************************************************
* Fichero:	button.h
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

#ifndef _BUTTON_REBOTES_H_
#define _BUTTON_REBOTES_H_

/*--- declaracion de funciones visibles del m�dulo button.c/button.h ---*/
void Botones_anti_inicializar(void);

int get_contador(void);

// Devuelve 1 solo la primera vez
int pulsado_izquierda(void);

// Devuelve 1 solo la primera vez
int pulsado_derecha(void);

void resetCounter();


#endif /* _BUTTON_REBOTES_H_ */
