#include "tpManager.h"
#include "tp.h"

#define x_sup_tablero 212 // Posición de la linea vertical que delimita el tablero por la derecha
#define y_sup_tablero 212 // Posición de la linea horizontal que delimita el tablero por abajo
#define x_inf_tablero 20 // Posición de la linea vertical que delimita el tablero por la izquierda
#define y_inf_tablero 20 // Posición de la linea horizontal que delimita el tablero por arriba

#define y_mitad_pantalla 160 // Posición de la linea horizontal que divide en dos partes iguales la pantalla
#define x_mitad_pantalla 120 // Posición de la linea vertical que divide en dos partes iguales la pantalla

#define tam_ficha_zoom 48 // Tamaño de una ficha cuando el tablero tiene zoom


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
int estado_tp_no_zoom(void) {
    int res;
    if (haPulsado() == 1) {
        int x = getX();
        int y = getY();
        if (((x >= x_inf_tablero) && (x <= x_sup_tablero)) && ((y <= y_sup_tablero) && (y >= y_inf_tablero))) {
            if (x >= ((x_sup_tablero + x_inf_tablero) / 2)) {
                if (y >= ((y_sup_tablero + y_inf_tablero) / 2)) {
                    res = 4;
                } else {
                    res = 2;
                }
            } else {
                if (y >= ((y_sup_tablero + y_inf_tablero) / 2)) {
                    res = 3;
                } else {
                    res = 1;
                }
            }

        } else if (y > y_sup_tablero) {
            res = TP_NO_ZOOM_PASAR;
        } else {
            res = TP_NO_ZOOM_FUERA_PANTALLA;
        }
    } else {
        res = TP_NO_ZOOM_NO_PULSADO;
    }
    return res;
}


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
int estado_tp_zoom(void) {
    int res;
    if (haPulsado() == 1) {
        int x = getX();
        int y = getY();
        if (((x >= x_inf_tablero) && (x <= x_sup_tablero)) && ((y <= y_sup_tablero) && (y >= y_inf_tablero))) {

            int columna = (x - x_inf_tablero) / tam_ficha_zoom;

            int fila = (y - y_inf_tablero) / tam_ficha_zoom;

            res = 4 * fila + columna + 1;

        } else if (y > y_sup_tablero) {
            res = TP_ZOOM_PASAR;
        } else {
            res = TP_ZOOM_FUERA_PANTALLA;
        }
    } else {
        res = TP_ZOOM_NO_PULSADO;
    }
    return res;
}


/**
 * Pre: ---
 * Post: Devuelve TP_PULSADO si se ha pulsado sobre la pantalla o TP_NO_PULSADO en caso contrario
 * */
int estado_tp(void) {
    if (haPulsado())
        return TP_PULSADO;
    else
        return TP_NO_PULSADO;
}


/**
 * Pre: ---
 * Post: Devuelve TP_PASAR_O_FINALIZAR_PASAR si se ha pulsado sobre la pantalla en el recuadro que pone
 * PASAR, en el caso de que se pulse sobre el recuadro que pone FINALIZAR, devolverá TP_PASAR_O_FINALIZAR_FINALIZAR
 * y en el caso de que se pulse sobre el recuadro que pone VOLVER devolverá TP_PASAR_O_FINALIZAR_VOLVER.
 * Mientras no se pulse la pantalla devolverá TP_PASAR_O_FINALIZAR_NO_PULSADO.
 * */
int estado_tp_pasar_o_finalizar(void) {
    int res;
    if (haPulsado() == 1) {
        int y = getY();
        int x = getX();
        if (y >= y_mitad_pantalla) {
            //Mitad inferior
            res = TP_PASAR_O_FINALIZAR_VOLVER;

        } else if (x >= x_mitad_pantalla) {
            //Mitad derecha
            res = TP_PASAR_O_FINALIZAR_FINALIZAR;

        } else {
            //Mitad izquierda
            res = TP_PASAR_O_FINALIZAR_PASAR;
        }

    } else {
        res = TP_PASAR_O_FINALIZAR_NO_PULSADO;
    }
    return res;
}
