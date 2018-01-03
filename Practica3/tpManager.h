#ifndef _TP_MANAGER_H_
#define _TP_MANAGER_H_


#define TP_NO_ZOOM_NO_PULSADO 0
#define TP_NO_ZOOM_PASAR 5
#define TP_NO_ZOOM_FUERA_PANTALLA 7

/**
 * Pre: En el lcd se está visualizando el tablero sin modo zoom
 * Post: Devuelve la región del tablero pulsada, siguiendo la
 * siguiente matriz:
 *
 *   1  2
 *   3  4
 *
 *  En el caso de que no se haya pulsado devolverá TP_NO_ZOOM_NO_PULSADO,
 *  en el caso de que se haya pulsado sobre la región asignada a pasar devolverá TP_NO_ZOOM_PASAR
 *  en cualquier otro caso devolverá TP_NO_ZOOM_FUERA_PANTALLA
 * */
int estado_tp_no_zoom(void);


#define TP_ZOOM_NO_PULSADO 0
#define TP_ZOOM_PASAR 17
#define TP_ZOOM_FUERA_PANTALLA 19

/**
 * Pre: En el lcd se está visualizando el tablero en modo zoom
 * Post: Devuelve la posición del tablero pulsada, siguiendo la
 * siguiente matriz:
 *
 *   1  2  3  4
 *   5  6  7  8
 *   9 10 11 12
 *  13 14 15 16
 *
 *  En el caso de que no se haya pulsado devolverá TP_ZOOM_NO_PULSADO,
 *  en el caso de que se haya pulsado sobre la región asignada a pasar devolverá TP_ZOOM_PASAR
 *  en cualquier otro caso devolverá TP_ZOOM_FUERA_PANTALLA
 * */
int estado_tp_zoom(void);


#define TP_PULSADO 1
#define TP_NO_PULSADO 0

/**
 * Pre: ---
 * Post: Devuelve TP_PULSADO si se ha pulsado sobre la pantalla o TP_NO_PULSADO en caso contrario
 * */
int estado_tp(void);


#define TP_PASAR_O_FINALIZAR_NO_PULSADO 0
#define TP_PASAR_O_FINALIZAR_PASAR 1
#define TP_PASAR_O_FINALIZAR_FINALIZAR 2
#define TP_PASAR_O_FINALIZAR_VOLVER 3

/**
 * Pre: ---
 * Post: Devuelve TP_PASAR_O_FINALIZAR_PASAR si se ha pulsado sobre la pantalla en el recuadro que pone
 * PASAR, en el caso de que se pulse sobre el recuadro que pone FINALIZAR, devolverá TP_PASAR_O_FINALIZAR_FINALIZAR
 * y en el caso de que se pulse sobre el recuadro que pone VOLVER devolverá TP_PASAR_O_FINALIZAR_VOLVER.
 * Mientras no se pulse la pantalla devolverá TP_PASAR_O_FINALIZAR_NO_PULSADO.
 * */
int estado_tp_pasar_o_finalizar(void);

#endif //_TP_MANAGER_H_
