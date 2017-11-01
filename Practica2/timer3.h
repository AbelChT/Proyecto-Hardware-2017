/*********************************************************************************************
* Fichero:		timer3.h
* Autor:
* Descrip:		funciones de control del timer3 del s3c44b0x
* Version:
*********************************************************************************************/

#ifndef _TIMER3_H_
#define _TIMER3_H_

/*--- declaracion de funciones visibles del m�dulo timer.c/timer.h ---*/
void timer3_inicializar(void);
void timer3_gestionar_rebotes_iniciales(void);
void timer3_encuesta_mantenido(void);
void timer3_gestionar_rebotes_finales(void);

#endif /* _TIMER2_H_ */
