/*
 * Test_ARM_ficha_valida.c
 *
 *  Created on: 30/09/2017
 *      Author: Abel
 */
#define DIM   8
#define CASILLA_VACIA 0

extern char ARM_ficha_valida (char tablero[][DIM], char f, char c, int *posicion_valida);


char __attribute__ ((aligned (8))) tablero[DIM][DIM] = {
	        {3,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,7,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,17}
};
char ficha_valida(char tablero[][DIM], char f, char c, int *posicion_valida)
{
    char ficha;

    if ((f <= DIM) && (f >= 0) && (c <= DIM) && (c >= 0) && (tablero[f][c] != CASILLA_VACIA))
    {
        *posicion_valida = 1;
        ficha = tablero[f][c];
    }
    else
    {
        *posicion_valida = 0;
        ficha = CASILLA_VACIA;
    }
    return ficha;
}

void test_ficha_valida_arm()
{
	int posicion_valida;
	char FA = DIM-1;
	char CA = DIM-1;


	char r1_m = ARM_ficha_valida(tablero, FA, CA, &posicion_valida);
	char r1_o = ficha_valida(tablero, FA, CA, &posicion_valida);

	// Posicion valida = 1
	// r1 = 17

	FA = 0;
	CA = 0;

	r1_m = ARM_ficha_valida(tablero, FA, CA, &posicion_valida);
	r1_o = ficha_valida(tablero, FA, CA, &posicion_valida);
	// Posicion valida = 1
	// r1 = 3

	FA = 1;
	CA = 2;

	r1_m = ARM_ficha_valida(tablero, FA, CA, &posicion_valida);
	r1_o = ficha_valida(tablero, FA, CA, &posicion_valida);
	// Posicion valida = 1
	// r1 = 7

	FA = -1;
	CA = -1;

	r1_m = ARM_ficha_valida(tablero, FA, CA, &posicion_valida);
	r1_o = ficha_valida(tablero, FA, CA, &posicion_valida);
	// Posicion valida = 0
	// r1 = 0

	FA = DIM;
	CA = DIM;

	r1_m = ARM_ficha_valida(tablero, FA, CA, &posicion_valida);
	r1_o = ficha_valida(tablero, FA, CA, &posicion_valida);

	// Posicion valida = 0
	// r1 = 0

	int i =0;



}


