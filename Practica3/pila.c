#include <stdint.h>
#include "pila.h"
#include"timer2.h"

/*--- variables globales ---*/
static uint32_t DSTACK_INIT = 0x0c7ff000;
int DSTACK_LIMIT = 64;
struct event *vDSTACK;
int INDEX;
int TAM;


typedef struct event {
    int event;
    int aux;
    int time;
};

void fill(int e, int a, int t, struct event *this) {
    (*this).event = e;
    (*this).aux = a;
    (*this).time = t;
}

/**
 * Pre: ---
 * Post: Inicializa la pila de depuración
 * */
void init_pila_depuracion() {
    vDSTACK = DSTACK_INIT; //DSTACK;
    INDEX = DSTACK_LIMIT - 1;;
    timer2_inicializar();
    timer2_empezar();
}

/**
 * Pre: La pila de depuración ha de estar inicializada
 * Post: Guarda un evento en la pila de depuración
 * */
void push_debug(int ID_evento, int auxData) {
    struct event ev;
    fill(ID_evento, auxData, timer2_leer(), &ev);
    vDSTACK[INDEX] = ev;
    INDEX = (INDEX - 1) & 0x3F;
    TAM += 1;
}

/**
 * Pre: La pila de depuración ha de estar inicializada y existe algún elemento en la pila de depuración
 * Post: Recupera un evento de la pila de depuración
 * */
void pop_debug(int *IDevento, int *auxData, int *time) {
    if (TAM > 0) {
        INDEX = (INDEX + 1) & 0x3F;
        *time = vDSTACK[INDEX].time;
        *auxData = vDSTACK[INDEX].aux;
        *IDevento = vDSTACK[INDEX].event;
        TAM -= 1;
    }
}
