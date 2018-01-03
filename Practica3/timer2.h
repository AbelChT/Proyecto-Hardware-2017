#ifndef _TIMER2_H_
#define _TIMER2_H_

/**
 * Pre: ---
 * Post: Inicializa el timer 2
 * */
void timer2_inicializar(void);

/**
 * Pre: El timer 2 ha de estar inicializado
 * Post: Comienza la cuenta del timer 2
 * */
void timer2_empezar(void);

/**
 * Pre: Se ha de haber comenzado una cuenta con el timer 2
 * Post: Devuelve la cuenta del timer 2
 * */
int timer2_leer(void);

#endif /* _TIMER2_H_ */
