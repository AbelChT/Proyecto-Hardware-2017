/*********************************************************************************************
* File��	tp.H
* Author:	embest	
* Desc��	Touch Screen define file
* History:	
*********************************************************************************************/

#ifndef __TP_H__
#define __TP_H__

/**
 * Pre: ---
 * Post: Inicializa el touch screen
 */
void TS_init(void);

/**
 * Pre: Se ha inicializado el touch screen
 * Post: Finaliza el touch screen
 */
void TS_close(void);

/**
 * Pre: Se ha inicializado el touch screen
 * Post: Obtiene la coordenada x de la última pulsación (el sistema de referencia es el que utiliza el lcd
 * para dibujar los píxeles)
 */
int getX(void);


/**
 * Pre: Se ha inicializado el touch screen
 * Post: Obtiene la coordenada y de la última pulsación (el sistema de referencia es el que utiliza el lcd
 * para dibujar los píxeles)
 */
int getY(void);


/**
 * Pre: Se ha inicializado el touch screen
 * Post: Devuelve 1 si se ha pulsado el touch screen desde la última vez que se invocó a esta función
 */
int haPulsado(void);

#endif /*__TP_H__*/
