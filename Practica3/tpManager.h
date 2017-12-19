//
// Created by abel on 10/12/17.
//

#ifndef PROYECTOHARDWARE_TPMANAGER_H
#define PROYECTOHARDWARE_TPMANAGER_H


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

//TODO: Devuelve valor incorrecto
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

//TODO: Devuelve valor incorrecto
int estado_tp_pasar_o_finalizar(void);

#endif //PROYECTOHARDWARE_TPMANAGER_H
