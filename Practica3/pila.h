#ifndef _PILA_H_
#define _PILA_H_

/**
 * Pre: ---
 * Post: Inicializa la pila de depuración
 * */
void init_pila_depuracion();

/**
 * Pre: La pila de depuración ha de estar inicializada
 * Post: Guarda un evento en la pila de depuración
 * */
void push_debug(int ID_evento, int auxData);

/**
 * Pre: La pila de depuración ha de estar inicializada y existe algún elemento en la pila de depuración
 * Post: Recupera un evento de la pila de depuración
 * */
void pop_debug(int *IDevento, int *auxData, int *time);

#endif