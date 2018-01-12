/*********************************************************************************************
* Fichero:		timer2.h
* Autor:
* Descrip:		funciones de control del timer2 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER2_H_
#define _TIMER2_H_

/*--- declaracion de funciones visibles del m�dulo timer.c/timer.h ---*/
void timer2_inicializar(void);
void timer2_empezar(void);
int timer2_leer(void);

#endif /* _TIMER2_H_ */
