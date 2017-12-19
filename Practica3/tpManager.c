#include "tpManager.h"
#include "tp.h"

#define x_sup_tablero 212
#define y_sup_tablero 212
#define x_inf_tablero 20
#define y_inf_tablero 20

#define y_mitad_pantalla 160
#define x_mitad_pantalla 120

#define tam_ficha_zoom 48

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

int estado_tp(void) {
    if(haPulsado())
        return TP_PULSADO;
    else
        return TP_NO_PULSADO;
}

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
