#ifndef _BUTTON_REBOTES_H_
#define _BUTTON_REBOTES_H_

#define BOTON_PULSADO_IZQUIERDA 2
#define BOTON_PULSADO_DERECHA 1
#define BOTON_NO_PULSADO 0

// Tiempo que tardan los rebotes iniciales
#define TIEMPO_RETARDO_REBOTES 2262

// Frecuencia con la que se encuestará despues de los rebotes iniciales para saber si se ha soltado el botón
#define TIEMPO_ENCUESTA_PERIODICA 1645

// Tiempo que ha de pasar el boton mantenido para que se vea como una pulsacion
#define TIEMPO_MANTENIDO_PULSACION 4

// Inicializa los botones
void Botones_anti_inicializar(void);

// Devuelve el boton pulsado (BOTON_PULSADO_IZQUIERDA o BOTON_PULSADO_DERECHA) en el caso de que lo esté alguno
// o BOTON_NO_PULSADO, en caso de que no lo esté ninguno
int estado_botones(void);

#endif /* _BUTTON_REBOTES_H_ */
