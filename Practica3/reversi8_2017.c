#include "reversi8_2017.h"

#include <stdint.h>


#include "button_rebotes.h"
#include "pila.h"

#include "44blib.h"
#include "44b.h"
#include "timer2.h"
#include "exception_handler.h"
#include "8led.h"
#include "lcdManager.h"
#include "tpManager.h"
#include "tp.h"
#include "timer1.h"

// Tama?o del tablero

// Valores que puede devolver la funci?n patron_volteo())
#define PATRON_ENCONTRADO    1
#define NO_HAY_PATRON        0

// Estados de las casillas del tablero
#define CASILLA_VACIA 0
#define FICHA_BLANCA  1
#define FICHA_NEGRA   2

// candidatas: indica las posiciones a explorar
// Se usa para no explorar todo el tablero innecesariamente
// Sus posibles valores son NO, SI, CASILLA_OCUPADA
#define NO               0
#define SI               1
#define CASILLA_OCUPADA  2

#define DIM   8


/////////////////////////////////////////////////////////////////////////////
// TABLAS AUXILIARES
// declaramos las siguientes tablas como globales para que sean m?s f?ciles visualizarlas en el simulador
// __attribute__ ((aligned (8))): specifies a minimum alignment for the variable or structure field, measured in bytes, in this case 8 bytes

static const char __attribute__ ((aligned (8))) tabla_valor[8][8] =
        {
                {8, 2, 7, 3, 3, 7, 2, 8},
                {2, 1, 4, 4, 4, 4, 1, 2},
                {7, 4, 6, 5, 5, 6, 4, 7},
                {3, 4, 5, 0, 0, 5, 4, 3},
                {3, 4, 5, 0, 0, 5, 4, 3},
                {7, 4, 6, 5, 5, 6, 4, 7},
                {2, 1, 4, 4, 4, 4, 1, 2},
                {8, 2, 7, 3, 3, 7, 2, 8}
        };


// Tabla de direcciones. Contiene los desplazamientos de las 8 direcciones posibles
const char vSF[DIM] = {-1, -1, 0, 1, 1, 1, 0, -1};
const char vSC[DIM] = {0, 1, 1, 1, 0, -1, -1, -1};

//////////////////////////////////////////////////////////////////////////////////////
// Variables globales que no deber?an serlo
// tablero, fila, columna y ready son varibles que se deber?an definir como locales dentro de reversi8.
// Sin embargo, las hemos definido como globales para que sea m?s f?cil visualizar el tablero y las variables en la memoria
//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// Tablero sin inicializar
////////////////////////////////////////////////////////////////////
char __attribute__ ((aligned (8))) tablero[DIM][DIM] = {
        {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
        {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
        {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
        {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
        {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
        {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
        {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA},
        {CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA, CASILLA_VACIA}
};

////////////////////////////////////////////////////////////////////
// VARIABLES PARA INTERACCIONAR CON LA ENTRADA SALIDA
// Pregunta: ?hay que hacer algo con ellas para que esto funcione bien?
// (por ejemplo a?adir alguna palabra clave para garantizar que la sincronizaci?n a trav?s de esa variable funcione)
char fila = 0, columna = 0, ready = 0;



// extern int patron_volteo(char tablero[][8], int *longitud,char f, char c, char SF, char SC, char color);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 0 indica CASILLA_VACIA, 1 indica FICHA_BLANCA y 2 indica FICHA_NEGRA
// pone el tablero a cero y luego coloca las fichas centrales.
void init_table(char tablero[][DIM], char candidatas[][DIM]) {
    int i, j;

    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++)
            tablero[i][j] = CASILLA_VACIA;
    }
#if 0
    for (i = 3; i < 5; ++i) {
    for(j = 3; j < 5; ++j) {
        tablero[i][j] = i == j ? FICHA_BLANCA : FICHA_NEGRA;
    }
    }

    for (i = 2; i < 6; ++i) {
    for (j = 2; j < 6; ++j) {
        if((i>=3) && (i < 5) && (j>=3) && (j<5)) {
        candidatas[i][j] = CASILLA_OCUPADA;
        } else {
        candidatas[i][j] = CASILLA_LIBRE;
        }
    }
    }
#endif
    // arriba hay versi?n alternativa
    tablero[3][3] = FICHA_BLANCA;
    tablero[4][4] = FICHA_BLANCA;
    tablero[3][4] = FICHA_NEGRA;
    tablero[4][3] = FICHA_NEGRA;

    candidatas[3][3] = CASILLA_OCUPADA;
    candidatas[4][4] = CASILLA_OCUPADA;
    candidatas[3][4] = CASILLA_OCUPADA;
    candidatas[4][3] = CASILLA_OCUPADA;

    // casillas a explorar:
    candidatas[2][2] = SI;
    candidatas[2][3] = SI;
    candidatas[2][4] = SI;
    candidatas[2][5] = SI;
    candidatas[3][2] = SI;
    candidatas[3][5] = SI;
    candidatas[4][2] = SI;
    candidatas[4][5] = SI;
    candidatas[5][2] = SI;
    candidatas[5][3] = SI;
    candidatas[5][4] = SI;
    candidatas[5][5] = SI;
}

////////////////////////////////////////////////////////////////////////////////
// Espera a que ready valga 1.
// CUIDADO: si el compilador coloca esta variable en un registro, no funcionar?.
// Hay que definirla como "volatile" para forzar a que antes de cada uso la cargue de memoria

void esperar_mov(char *ready) {
    while (*ready ==
           0) {};  // bucle de espera de respuestas hasta que el se modifique el valor de ready (hay que hacerlo manualmente)

    *ready = 0;  //una vez que pasemos el bucle volvemos a fijar ready a 0;
}

////////////////////////////////////////////////////////////////////////////////
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// IMPORTANTE: AL SUSTITUIR FICHA_VALIDA() Y PATRON_VOLTEO()
// POR RUTINAS EN ENSAMBLADOR HAY QUE RESPETAR LA MODULARIDAD.
// DEBEN SEGUIR SIENDO LLAMADAS A FUNCIONES Y DEBEN CUMPLIR CON EL ATPCS
// (VER TRANSPARENCIAS Y MATERIAL DE PRACTICAS):
//  - DEBEN PASAR LOS PARAMETROS POR LOS REGISTROS CORRESPONDIENTES
//  - GUARDAR EN PILA SOLO LOS REGISTROS QUE TOCAN
//  - CREAR UN MARCO DE PILA TAL Y COMO MUESTRAN LAS TRANSPARENCIAS
//    DE LA ASIGNATURA (CON EL PC, FP, LR,....)
//  - EN EL CASO DE LAS VARIABLES LOCALES, SOLO HAY QUE APILARLAS
//    SI NO SE PUEDEN COLOCAR EN UN REGISTRO.
//    SI SE COLOCAN EN UN REGISTRO NO HACE FALTA
//    NI GUARDARLAS EN PILA NI RESERVAR UN ESPACIO EN LA PILA PARA ELLAS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////////////////////////////////////////////////////////////////////////////////
// Devuelve el contenido de la posici?n indicadas por la fila y columna actual.
// Adem?s informa si la posici?n es v?lida y contiene alguna ficha.
// Esto lo hace por referencia (en *posicion_valida)
// Si devuelve un 0 no es v?lida o est? vacia.
char ficha_valida(char tablero[][DIM], char f, char c, int *posicion_valida) {
    char ficha;

    // ficha = tablero[f][c];
    // no puede accederse a tablero[f][c]
    // ya que alg?n ?ndice puede ser negativo

    if ((f < DIM) && (f >= 0) && (c < DIM) && (c >= 0) && (tablero[f][c] != CASILLA_VACIA)) {
        *posicion_valida = 1;
        ficha = tablero[f][c];
    } else {
        *posicion_valida = 0;
        ficha = CASILLA_VACIA;
    }
    return ficha;
}

////////////////////////////////////////////////////////////////////////////////
// La funci?n patr?n volteo es una funci?n recursiva que busca el patr?n de volteo
// (n fichas del rival seguidas de una ficha del jugador actual) en una direcci?n determinada
// SF y SC son las cantidades a sumar para movernos en la direcci?n que toque
// color indica el color de la pieza que se acaba de colocar
// la funci?n devuelve PATRON_ENCONTRADO (1) si encuentra patr?n y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un par?metro por referencia. Sirve para saber la longitud del patr?n que se est? analizando. Se usa para saber cuantas fichas habr?a que voltear
int patron_volteo(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color) {
    int posicion_valida; // indica si la posici?n es valida y contiene una ficha de alg?n jugador
    int patron; //indica si se ha encontrado un patr?n o no
    char casilla;   // casilla es la casilla que se lee del tablero
    FA = FA + SF;
    CA = CA + SC;
    casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
    // mientras la casilla est? en el tablero, no est? vac?a,
    // y es del color rival seguimos buscando el patron de volteo
    if ((posicion_valida == 1) && (casilla != color)) {
        *longitud = *longitud + 1;
        patron = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
        //printf("longitud: %d \n", *longitud);
        //printf("fila: %d; columna: %d \n", FA, CA);
        return patron;
    }
        // si la ultima posici?n era v?lida y la ficha es del jugador actual,
        // entonces hemos encontrado el patr?n
    else if ((posicion_valida == 1) && (casilla == color)) {
        if (*longitud > 0) // longitud indica cuantas fichas hay que voltear
        {
            return PATRON_ENCONTRADO; // si hay que voltear una ficha o m?s hemos encontrado el patr?n
            //printf("PATRON_ENCONTRADO \n");
        } else {
            return NO_HAY_PATRON; // si no hay que voltear no hay patr?n
            //printf("NO_HAY_PATRON \n");
        }
    }
        // en caso contrario es que no hay patr?n
    else {
        return NO_HAY_PATRON;
        //printf("NO_HAY_PATRON \n");
    }
}

////////////////////////////////////////////////////////////////////////////////
// voltea n fichas en la direcci?n que toque
// SF y SC son las cantidades a sumar para movernos en la direcci?n que toque
// color indica el color de la pieza que se acaba de colocar
// FA y CA son la fila y columna a analizar
void voltear(char tablero[][DIM], char FA, char CA, char SF, char SC, int n, char color) {
    int i;

    for (i = 0; i < n; i++) {
        FA = FA + SF;
        CA = CA + SC;
        tablero[FA][CA] = color;
    }
}

////////////////////////////////////////////////////////////////////////////////
// comprueba si hay que actualizar alguna ficha
// no comprueba que el movimiento realizado sea v?lido
// f y c son la fila y columna a analizar
// char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
// char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};
int actualizar_tablero(char tablero[][DIM], char f, char c, char color) {
    char SF, SC; // cantidades a sumar para movernos en la direcci?n que toque
    int i, flip, patron;

    for (i = 0; i < DIM; i++) // 0 es Norte, 1 NE, 2 E ...
    {
        SF = vSF[i];
        SC = vSC[i];
        // flip: numero de fichas a voltear
        flip = 0;
        patron = patron_volteo(tablero, &flip, f, c, SF, SC, color);
        //printf("Flip: %d \n", flip);
        if (patron == PATRON_ENCONTRADO) {
            voltear(tablero, f, c, SF, SC, flip, color);
        }
    }
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Recorre todo el tablero comprobando en cada posici?n si se puede mover
// En caso afirmativo, consulta la puntuaci?n de la posici?n y si es la mejor
// que se ha encontrado la guarda
// Al acabar escribe el movimiento seleccionado en f y c

// Candidatas
// NO    0
// SI    1
// CASILLA_OCUPADA 2
int elegir_mov(char candidatas[][DIM], char tablero[][DIM], char *f, char *c) {
    int i, j, k, found;
    int mf = -1; // almacena la fila del mejor movimiento encontrado
    int mc;      // almacena la columna del mejor movimiento encontrado
    char mejor = 0; // almacena el mejor valor encontrado
    int patron, longitud;
    char SF, SC; // cantidades a sumar para movernos en la direcci?n que toque

    // Recorremos todo el tablero comprobando d?nde podemos mover
    // Comparamos la puntuaci?n de los movimientos encontrados y nos quedamos con el mejor
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {   // indica en qu? casillas quiz? se pueda mover
            if (candidatas[i][j] == SI) {
                if (tablero[i][j] == CASILLA_VACIA) {
                    found = 0;
                    k = 0;

                    // en este bucle comprobamos si es un movimiento v?lido
                    // (es decir si implica voltear en alguna direcci?n)
                    while ((found == 0) && (k < DIM)) {
                        SF = vSF[k];    // k representa la direcci?n que miramos
                        SC = vSC[k];    // 1 es norte, 2 NE, 3 E ...

                        // nos dice qu? hay que voltear en cada direcci?n
                        longitud = 0;
                        patron = patron_volteo(tablero, &longitud, i, j, SF, SC, FICHA_BLANCA);
                        //  //printf("%d ", patron);
                        if (patron == PATRON_ENCONTRADO) {
                            found = 1;
                            if (tabla_valor[i][j] > mejor) {
                                mf = i;
                                mc = j;
                                mejor = tabla_valor[i][j];
                            }
                        }
                        k++;
                        // si no hemos encontrado nada probamos con la siguiente direcci?n
                    }
                }
            }
        }
    }
    *f = (char) mf;
    *c = (char) mc;
    // si no se ha encontrado una posici?n v?lida devuelve -1
    return mf;
}

////////////////////////////////////////////////////////////////////////////////
// Cuenta el n?mero de fichas de cada color.
// Los guarda en la direcci?n b (blancas) y n (negras)
void contar(char tablero[][DIM], int *b, int *n) {
    int i, j;

    *b = 0;
    *n = 0;

    // recorremos todo el tablero contando las fichas de cada color
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            if (tablero[i][j] == FICHA_BLANCA) {
                (*b)++;
            } else if (tablero[i][j] == FICHA_NEGRA) {
                (*n)++;
            }
        }
    }
}

void actualizar_candidatas(char candidatas[][DIM], char f, char c) {
    // donde ya se ha colocado no se puede volver a colocar
    // En las posiciones alrededor s?
    candidatas[f][c] = CASILLA_OCUPADA;
    if (f > 0) {
        if (candidatas[f - 1][c] != CASILLA_OCUPADA)
            candidatas[f - 1][c] = SI;

        if ((c > 0) && (candidatas[f - 1][c - 1] != CASILLA_OCUPADA))
            candidatas[f - 1][c - 1] = SI;

        if ((c < 7) && (candidatas[f - 1][c + 1] != CASILLA_OCUPADA))
            candidatas[f - 1][c + 1] = SI;
    }
    if (f < 7) {
        if (candidatas[f + 1][c] != CASILLA_OCUPADA)
            candidatas[f + 1][c] = SI;

        if ((c > 0) && (candidatas[f + 1][c - 1] != CASILLA_OCUPADA))
            candidatas[f + 1][c - 1] = SI;

        if ((c < 7) && (candidatas[f + 1][c + 1] != CASILLA_OCUPADA))
            candidatas[f + 1][c + 1] = SI;
    }
    if ((c > 0) && (candidatas[f][c - 1] != CASILLA_OCUPADA))
        candidatas[f][c - 1] = SI;

    if ((c < 7) && (candidatas[f][c + 1] != CASILLA_OCUPADA))
        candidatas[f][c + 1] = SI;
}

int aumentarContador(int contador) {
    if (contador < 8)
        return contador + 1; // incrementamos el contador
    else
        return 0;
}


////////////////////////////////////////////////////////////////////////////////
// Proceso principal del juego
// Utiliza el tablero,
// y las direcciones en las que indica el jugador la fila y la columna
// y la se?al de ready que indica que se han actualizado fila y columna
// tablero, fila, columna y ready son variables globales aunque deber?an ser locales de reversi8,
// la raz?n es que al meterlas en la pila no las pone juntas, y as? jugar es m?s complicado.
// en esta versi?n el humano lleva negras y la m?quina blancas
// no se comprueba que el humano mueva correctamente.
// S?lo que la m?quina realice un movimiento correcto.

//Variables globales

static int tiempo_juego = 0;
static int tiempo_procesamiento = 0;


#define ESTADO_INICIO_DEL_JUEGO 1
#define ESTADO_MUESTRA_TABLERO 2
#define ESTADO_MUESTRA_ZOOM 3
#define ESTADO_MUESTRA_PARPADEO 4
#define ESTADO_MUESTRA_SCORE 5
#define ESTADO_PASAR_O_FINALIZAR 6
#define ESTADO_PROCESA_MOVIMIENTO 7
#define ESTADO_PASAR 8

static int estado_juego = ESTADO_INICIO_DEL_JUEGO;


int fn_estado_inicio_del_juego(char candidatas[DIM][DIM]) {

    init_table(tablero, candidatas);
    LcdM_Show_Instructions();

    tiempo_procesamiento =0;
    tiempo_juego =0;

    // Espera pulsaci?n
    while (estado_botones() == BOTON_NO_PULSADO && estado_tp() == TP_NO_PULSADO) {
    }
    return ESTADO_MUESTRA_TABLERO;
}

#define ESTADO_TABLERO_NO_ZOOM_ESPERA_FILA  0
#define ESTADO_TABLERO_NO_ZOOM_ESPERA_COLUMNA 1
#define ESTADO_TABLERO_NO_ZOOM_INTRODUCIENDO_FILA 2
#define ESTADO_TABLERO_NO_ZOOM_INTRODUCIENDO_COLUMNA 3

//Devuelve el siguiente estado
int fn_estado_muestra_tablero(int *codigo_pulsacion, int *estado_filas, int *estado_columnas) {
    timer1_start();
    LcdM_Show_Board_No_Zoom(tablero);

    D8Led_symbol(15);

    int estado_tp = 0;
    int estado_bt = 0;

    *estado_filas = 0;
    *estado_columnas = 0;

    int estado_ms_tp = ESTADO_TABLERO_NO_ZOOM_ESPERA_FILA;

    LcdM_Show_Processing_Time(tiempo_procesamiento);
    LcdM_Show_Game_Time(tiempo_juego);

    while (1) {
    	if(timer1_leer() > 1){
    		timer1_start();
    		++tiempo_juego;
    		LcdM_Show_Game_Time(tiempo_juego);
    	}

        estado_tp = estado_tp_no_zoom();
        estado_bt = estado_botones();

        if (estado_tp != TP_NO_ZOOM_NO_PULSADO && estado_tp != TP_NO_ZOOM_FUERA_PANTALLA) {
            // Se ha pulsado el tp
            switch (estado_tp) {
                case TP_NO_ZOOM_PASAR:
                    return ESTADO_PASAR_O_FINALIZAR;

                default:
                    // Pulsado en región
                    *codigo_pulsacion = estado_tp;
                    return ESTADO_MUESTRA_ZOOM;
            }

        } else if (estado_bt == BOTON_PULSADO_IZQUIERDA) {
            switch (estado_ms_tp) {
                case ESTADO_TABLERO_NO_ZOOM_ESPERA_FILA:
                    D8Led_symbol(*estado_filas);
                    estado_ms_tp = ESTADO_TABLERO_NO_ZOOM_INTRODUCIENDO_FILA;
                    break;

                case ESTADO_TABLERO_NO_ZOOM_ESPERA_COLUMNA:
                    D8Led_symbol(*estado_columnas);
                    estado_ms_tp = ESTADO_TABLERO_NO_ZOOM_INTRODUCIENDO_COLUMNA;
                    break;

                case ESTADO_TABLERO_NO_ZOOM_INTRODUCIENDO_FILA:
                    *estado_filas = aumentarContador(*estado_filas);
                    D8Led_symbol(*estado_filas);
                    break;

                case ESTADO_TABLERO_NO_ZOOM_INTRODUCIENDO_COLUMNA:
                    *estado_columnas = aumentarContador(*estado_columnas);
                    D8Led_symbol(*estado_columnas);
                    break;
            }
        } else if (estado_bt == BOTON_PULSADO_DERECHA) {

            switch (estado_ms_tp) {
                case ESTADO_TABLERO_NO_ZOOM_INTRODUCIENDO_FILA:
                    estado_ms_tp = ESTADO_TABLERO_NO_ZOOM_ESPERA_COLUMNA;
                    D8Led_symbol(12);
                    break;
                case ESTADO_TABLERO_NO_ZOOM_INTRODUCIENDO_COLUMNA:
                    if (*estado_filas == 8 ||
                        *estado_columnas == 8) {// Se ha introducido algun 8, asi que ir al estado de pasar
                        return ESTADO_PASAR_O_FINALIZAR;
                    } else {
                        return ESTADO_MUESTRA_PARPADEO;
                    }
            }
        }
    }

}

#define MUESTRA_ZOOM_8_LED_SYMBOL 10
int fn_estado_muestra_zoom(int region_tablero, int *estado_filas, int *estado_columnas) {
    timer1_start();

    LcdM_Show_Board_Zoom(tablero, region_tablero);
    D8Led_symbol(MUESTRA_ZOOM_8_LED_SYMBOL);
    int estado_tp = 0;
    int estado_bt = 0;

    LcdM_Show_Processing_Time(tiempo_procesamiento);
    LcdM_Show_Game_Time(tiempo_juego);
    while (1) {
        if(timer1_leer() > 1){
            timer1_start();
            ++tiempo_juego;
            LcdM_Show_Game_Time(tiempo_juego);
        }
        estado_tp = estado_tp_zoom();
        estado_bt = estado_botones();
        if (estado_tp != TP_ZOOM_NO_PULSADO ) {
            //Se ha pulsado una casilla o pasar
            switch (estado_tp) {
                case TP_ZOOM_PASAR:
                    return ESTADO_PASAR_O_FINALIZAR;
                case TP_ZOOM_FUERA_PANTALLA:
                	return ESTADO_MUESTRA_TABLERO;

                default:
                    // Pulsado en región
                    estado_tp = estado_tp - 1;
                    *estado_columnas = estado_tp % 4;
                    *estado_filas =  estado_tp / 4;

                	switch(region_tablero){
                	case 1:
                		break;
                	case 2:
                		*estado_columnas = *estado_columnas + 4;
                		break;
                	case 3:
                		*estado_filas = *estado_filas + 4;
                		break;
                	case 4:
                		*estado_filas = *estado_filas + 4;
                		*estado_columnas = *estado_columnas + 4;
                		break;

                	}
                    //Sacar fila y columna
                    return ESTADO_MUESTRA_PARPADEO;
            }

        } else if (estado_bt != BOTON_NO_PULSADO) {
            return ESTADO_MUESTRA_TABLERO;

        }
    }
}

#define MUESTRA_PARPADEO_TIEMPO_PARPADEO 5
#define MUESTRA_PARPADEO_8_LED_SYMBOL 10
int fn_estado_muestra_parpadeo(int estado_filas, int estado_columnas) {
    timer1_start();

    LcdM_Show_Board_No_Zoom(tablero);
    D8Led_symbol(MUESTRA_PARPADEO_8_LED_SYMBOL);
    int cuenta_timer1 = 0;
    int tiempo_aumentado_4 = 0;
    int tiempo_aumentado_8 = 0;

    LcdM_Show_Processing_Time(tiempo_procesamiento);
    LcdM_Show_Game_Time(tiempo_juego);

    int ultimo_timer_sumado = 0;

    while (estado_botones() == BOTON_NO_PULSADO && estado_tp() == TP_NO_PULSADO && timer1_leer() < MUESTRA_PARPADEO_TIEMPO_PARPADEO) {
        int t1l= timer1_leer();
        if (t1l > cuenta_timer1) {
            cuenta_timer1 = timer1_leer();
            LcdM_Show_Board_Position_Selected(estado_filas * 8 + estado_columnas + 1);

        }
        if((t1l - ultimo_timer_sumado) > 1 ){
            ++tiempo_juego;
            LcdM_Show_Game_Time(tiempo_juego);
            ultimo_timer_sumado = t1l;

        }
    }
    cuenta_timer1 = timer1_leer();

    if (cuenta_timer1 < MUESTRA_PARPADEO_TIEMPO_PARPADEO) {
        // Implica que o bien se ha pulsado sobre el boton o la pantalla
        return ESTADO_MUESTRA_TABLERO;
    }else{
        return ESTADO_PROCESA_MOVIMIENTO;
    }
}

int fn_estado_pasar_o_finalizar() {
    LcdM_Show_Skip();
    int estado_tp, estado_bt;

    while (1) {
        estado_tp = estado_tp_pasar_o_finalizar();
        estado_bt = estado_botones();
        if(estado_bt != BOTON_NO_PULSADO){
            switch (estado_bt){
                case BOTON_PULSADO_IZQUIERDA:
                    return ESTADO_PASAR;
                case BOTON_PULSADO_DERECHA:
                    return ESTADO_MUESTRA_SCORE;
            }
        }else if(estado_tp != TP_PASAR_O_FINALIZAR_NO_PULSADO){
            switch (estado_tp){
                case TP_PASAR_O_FINALIZAR_PASAR:
                    return ESTADO_PASAR;
                case TP_PASAR_O_FINALIZAR_FINALIZAR:
                    return ESTADO_MUESTRA_SCORE;
                case TP_PASAR_O_FINALIZAR_VOLVER:
                    return ESTADO_MUESTRA_TABLERO;
            }
        }
    }
}

int fn_estado_muestra_score() {
    int puntos_negras, puntos_blancas;
    contar(tablero,
           &puntos_blancas, &puntos_negras);

    LcdM_Show_Score(puntos_negras, puntos_blancas);
    // Espera pulsaci?n

    while (estado_botones() == BOTON_NO_PULSADO && estado_tp() == TP_NO_PULSADO) {
    }

    return ESTADO_INICIO_DEL_JUEGO;
}

int fn_estado_pasar(char candidatas[DIM][DIM]) {
    timer2_empezar();
    char f, c;    // fila y columna elegidas por la máquina para su movimiento
    // escribe el movimiento en las variables globales fila columna

    int done = elegir_mov(candidatas, tablero, &f, &c);
    int resultado;
    if(done == -1){
        resultado = ESTADO_MUESTRA_SCORE;
    }else{
        tablero[f][c] = FICHA_BLANCA;
        actualizar_tablero(tablero, f, c, FICHA_BLANCA);
        actualizar_candidatas(candidatas, f, c);
        resultado = ESTADO_MUESTRA_TABLERO;
    }

    tiempo_procesamiento += timer2_leer();
    return resultado;
}

int fn_estado_procesa_movimiento(int estado_filas, int estado_columnas, char candidatas[DIM][DIM]){
    timer2_empezar();
    tablero[estado_filas][estado_columnas] = FICHA_NEGRA;
    actualizar_tablero(tablero, estado_filas, estado_columnas, FICHA_NEGRA);
    actualizar_candidatas(candidatas, estado_filas, estado_columnas);

    char f, c;

    // escribe el movimiento en las variables globales fila columna
    int done = elegir_mov(candidatas, tablero, &f, &c);
    if (done != -1) {
        tablero[f][c] = FICHA_BLANCA;
        actualizar_tablero(tablero, f, c, FICHA_BLANCA);
        actualizar_candidatas(candidatas, f, c);
    }

    tiempo_procesamiento += timer2_leer();

    int puntos_negras, puntos_blancas;
    contar(tablero,
               &puntos_blancas, &puntos_negras);

    if((puntos_negras + puntos_blancas) == 64)
    	return ESTADO_MUESTRA_SCORE;
    else
    	return ESTADO_MUESTRA_TABLERO;
}

void reversi8() {

    char __attribute__ ((aligned (8))) candidatas[DIM][DIM] =
            {
                    {NO, NO, NO, NO, NO, NO, NO, NO},
                    {NO, NO, NO, NO, NO, NO, NO, NO},
                    {NO, NO, NO, NO, NO, NO, NO, NO},
                    {NO, NO, NO, NO, NO, NO, NO, NO},
                    {NO, NO, NO, NO, NO, NO, NO, NO},
                    {NO, NO, NO, NO, NO, NO, NO, NO},
                    {NO, NO, NO, NO, NO, NO, NO, NO},
                    {NO, NO, NO, NO, NO, NO, NO, NO}
            };

    int filas_, columnas_, estado_pulsacion_;    // fila y columna elegidas por la máquina para su movimiento

    estado_juego = ESTADO_INICIO_DEL_JUEGO;
    timer1_start();
    while (1) {
        switch (estado_juego) {
            case ESTADO_INICIO_DEL_JUEGO:
                estado_juego = fn_estado_inicio_del_juego(candidatas);
                break;
            case ESTADO_MUESTRA_TABLERO:
                estado_juego = fn_estado_muestra_tablero(&estado_pulsacion_, &filas_, &columnas_);
                break;
            case ESTADO_MUESTRA_ZOOM:
                estado_juego = fn_estado_muestra_zoom(estado_pulsacion_ , &filas_, &columnas_);
                break;
            case ESTADO_PASAR_O_FINALIZAR:
                estado_juego = fn_estado_pasar_o_finalizar();
                break;
            case ESTADO_MUESTRA_PARPADEO:
                estado_juego = fn_estado_muestra_parpadeo(filas_,columnas_);
                break;
            case ESTADO_MUESTRA_SCORE:
                estado_juego = fn_estado_muestra_score();
                break;
            case ESTADO_PROCESA_MOVIMIENTO:
                estado_juego = fn_estado_procesa_movimiento(filas_,columnas_,candidatas);
                break;
            case ESTADO_PASAR:
                estado_juego = fn_estado_pasar(candidatas);
        }
    }
}

