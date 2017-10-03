/*
 * Test_ARM_ficha_valida.c
 *
 *  Created on: 30/09/2017
 *      Author: Abel
 */
// Tama�o del tablero
#define DIM   8

// Valores que puede devolver la funci�n patron_volteo())
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

extern char ficha_valida_arm(char tablero[][DIM], char f, char c, int *posicion_valida);
extern char ficha_valida_thumb(char tablero[][DIM], char f, char c, int *posicion_valida);
extern int patron_volteo_arm(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color);

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

    if ((f < DIM) && (f >= 0) && (c < DIM) && (c >= 0) && (tablero[f][c] != CASILLA_VACIA))
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

////////////////////////////////////////////////////////////////////////////////
// La funci�n patr�n volteo es una funci�n recursiva que busca el patr�n de volteo
// (n fichas del rival seguidas de una ficha del jugador actual) en una direcci�n determinada
// SF y SC son las cantidades a sumar para movernos en la direcci�n que toque
// color indica el color de la pieza que se acaba de colocar
// la funci�n devuelve PATRON_ENCONTRADO (1) si encuentra patr�n y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un par�metro por referencia. Sirve para saber la longitud del patr�n que se est� analizando. Se usa para saber cuantas fichas habr�a que voltear
int patron_volteo(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color)
{
    int posicion_valida; // indica si la posici�n es valida y contiene una ficha de alg�n jugador
    int patron; //indica si se ha encontrado un patr�n o no
    char casilla;   // casilla es la casilla que se lee del tablero
    FA = FA + SF;
    CA = CA + SC;
    casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
    // mientras la casilla est� en el tablero, no est� vac�a,
    // y es del color rival seguimos buscando el patron de volteo
    if ((posicion_valida == 1) && (casilla != color))
    {
        *longitud = *longitud + 1;
        patron = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
        //printf("longitud: %d \n", *longitud);
        //printf("fila: %d; columna: %d \n", FA, CA);
        return patron;
    }
    // si la ultima posici�n era v�lida y la ficha es del jugador actual,
    // entonces hemos encontrado el patr�n
    else if ((posicion_valida == 1) && (casilla == color))
    {
        if (*longitud > 0) // longitud indica cuantas fichas hay que voltear
            {
            return PATRON_ENCONTRADO; // si hay que voltear una ficha o m�s hemos encontrado el patr�n
            //printf("PATRON_ENCONTRADO \n");
            }
        else {
            return NO_HAY_PATRON; // si no hay que voltear no hay patr�n
            //printf("NO_HAY_PATRON \n");
            }
    }
    // en caso contrario es que no hay patr�n
    else
    {
        return NO_HAY_PATRON;
        //printf("NO_HAY_PATRON \n");
    }
}

/*
 * Mejorar
 */
void test_ficha_valida_arm()
{

	const int NUM_TEST_CASES = 10;

// 0 implica no fallo, 1 implica fallo
	int HA_FALLADO_THUMB=0, HA_FALLADO_ARM=0;

// numero de test case que no pasa
	int TEST_CASE_PRIMER_FALLO=0;
/*
 * Primer elemento = FA
 * Segundo elemento = CA
 */
	char TEST_CASES[][2]={
			{0,0},
			{-1,0},
			{-1,-1},
			{1,2},
			{DIM,DIM},
			{DIM - 1,DIM - 1},
			{0,DIM},
			{DIM - 1,DIM},
			{0,DIM},
			{DIM,0}
	};

	int i = 0;

	while(i < NUM_TEST_CASES){
		int posicion_valida,posicion_valida_thumb,posicion_valida_arm;
		char resultado,resultado_thumb,resultado_arm;
		char FA = TEST_CASES[i][0];
		char CA = TEST_CASES[i][1];
		resultado_arm = ficha_valida_arm(tablero, FA, CA, &posicion_valida_arm);
		int j=0;
		resultado_thumb = ficha_valida_thumb(tablero, FA, CA, &posicion_valida_thumb);
		resultado = ficha_valida(tablero, FA, CA, &posicion_valida);

		if(resultado != resultado_arm || posicion_valida!= posicion_valida_arm){
			HA_FALLADO_ARM=1;
			TEST_CASE_PRIMER_FALLO=i;
		}

		if(resultado != resultado_thumb || posicion_valida!= posicion_valida_thumb){
			 HA_FALLADO_THUMB=1;
			 TEST_CASE_PRIMER_FALLO=i;
		}

		if(HA_FALLADO_ARM == 1|| HA_FALLADO_THUMB == 1){
			break;
		}
		i++;
	}
	int j=10;

}

void test_patron_volteo_arm()
{
	int voltea=0;
	char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
	char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};
	char __attribute__ ((aligned (8))) tablero1[DIM][DIM] = {
		        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
		        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
		        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
		        {CASILLA_VACIA,CASILLA_VACIA,1,2,2,1,CASILLA_VACIA,CASILLA_VACIA},
		        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
		        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
		        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
		        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
	};
    char SF, SC; // cantidades a sumar para movernos en la direcci�n que toque
    int i, flip, patron;

    for (i = 0; i < DIM; i++) // 0 es Norte, 1 NE, 2 E ...
    {
        SF = vSF[i];
        SC = vSC[i];
        // flip: numero de fichas a voltear
        flip = 0;
        patron = patron_volteo(tablero, &flip, 3, 2, SF, SC, 1);
        //printf("Flip: %d \n", flip);
        if (patron == PATRON_ENCONTRADO )
        {
            voltear(tablero, 3, 2, SF, SC, flip, 1);
            voltea=1;
        }
    }

}
