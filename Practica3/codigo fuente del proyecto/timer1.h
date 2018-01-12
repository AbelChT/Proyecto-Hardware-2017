#ifndef _TIMER1_H_
#define _TIMER1_H_

/**
 * Pre: ---
 * Post: Inicializa el timer 1
 * */
void timer1_inicializar(void);

/**
 * Pre: El timer 1 ha de estar inicializado
 * Post: Comienza la cuenta del timer 1
 * */
void timer1_start(void);

/**
 * Pre: Se ha de haber comenzado una cuenta con el timer 1
 * Post: Devuelve la cuenta del timer 1
 * */
int timer1_leer(void);

#endif
