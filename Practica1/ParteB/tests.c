/*
 * Test_ARM_ficha_valida.c
 *
 *  Created on: 30/09/2017
 *      Author: Abel
 */
// Tamaï¿½o del tablero


#define DIM   8

// Valores que puede devolver la funciï¿½n patron_volteo())
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
// La funciï¿½n patrï¿½n volteo es una funciï¿½n recursiva que busca el patrï¿½n de volteo
// (n fichas del rival seguidas de una ficha del jugador actual) en una direcciï¿½n determinada
// SF y SC son las cantidades a sumar para movernos en la direcciï¿½n que toque
// color indica el color de la pieza que se acaba de colocar
// la funciï¿½n devuelve PATRON_ENCONTRADO (1) si encuentra patrï¿½n y NO_HAY_PATRON (0) en caso contrario
// FA y CA son la fila y columna a analizar
// longitud es un parï¿½metro por referencia. Sirve para saber la longitud del patrï¿½n que se estï¿½ analizando. Se usa para saber cuantas fichas habrï¿½a que voltear
int patron_volteo(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color)
{
    int posicion_valida; // indica si la posiciï¿½n es valida y contiene una ficha de algï¿½n jugador
    int patron; //indica si se ha encontrado un patrï¿½n o no
    char casilla;   // casilla es la casilla que se lee del tablero
    FA = FA + SF;
    CA = CA + SC;
    casilla = ficha_valida(tablero, FA, CA, &posicion_valida);
    // mientras la casilla estï¿½ en el tablero, no estï¿½ vacï¿½a,
    // y es del color rival seguimos buscando el patron de volteo
    if ((posicion_valida == 1) && (casilla != color))
    {
        *longitud = *longitud + 1;
        patron = patron_volteo(tablero, longitud, FA, CA, SF, SC, color);
        //printf("longitud: %d \n", *longitud);
        //printf("fila: %d; columna: %d \n", FA, CA);
        return patron;
    }
    // si la ultima posiciï¿½n era vï¿½lida y la ficha es del jugador actual,
    // entonces hemos encontrado el patrï¿½n
    else if ((posicion_valida == 1) && (casilla == color))
    {
        if (*longitud > 0) // longitud indica cuantas fichas hay que voltear
            {
            return PATRON_ENCONTRADO; // si hay que voltear una ficha o mï¿½s hemos encontrado el patrï¿½n
            //printf("PATRON_ENCONTRADO \n");
            }
        else {
            return NO_HAY_PATRON; // si no hay que voltear no hay patrï¿½n
            //printf("NO_HAY_PATRON \n");
            }
    }
    // en caso contrario es que no hay patrï¿½n
    else
    {
        return NO_HAY_PATRON;
        //printf("NO_HAY_PATRON \n");
    }
}

////////////////////////////////////////////////////////////////////////////////
// voltea n fichas en la dirección que toque
// SF y SC son las cantidades a sumar para movernos en la dirección que toque
// color indica el color de la pieza que se acaba de colocar
// FA y CA son la fila y columna a analizar
void voltear(char tablero[][DIM], char FA, char CA, char SF, char SC, int n, char color)
{
    int i;

    for (i = 0; i < n; i++)
    {
        FA = FA + SF;
        CA = CA + SC;
        tablero[FA][CA] = color;
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
	char cv = CASILLA_VACIA;
	char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
	char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};
	char __attribute__ ((aligned (8))) tableros[3][DIM][DIM] = {
		        {{cv,cv,cv,cv,cv,cv,cv,cv},		// tablero de prueba 1
		        {cv,cv,cv,cv,cv,cv,cv,cv},
		        {cv,cv,cv,cv,cv,cv,cv,cv},
		        {cv,cv,1,2,2,1,cv,cv},			//(3,2) blancas
		        {cv,cv,cv,cv,cv,cv,cv,cv},
		        {cv,cv,cv,cv,cv,cv,cv,cv},
		        {cv,cv,cv,cv,cv,cv,cv,cv},
		        {cv,cv,cv,cv,cv,cv,cv,cv}},

		        {{cv,cv,cv,cv,cv,cv,cv,cv},		// tablero de prueba 2
		        {cv,cv,2,cv,cv,cv,cv,cv},
		        {2,cv,1,cv,cv,cv,cv,cv},
		        {cv,1,1,cv,cv,cv,cv,cv},
		        {2,1,2,1,1,1,2,cv},				//(4,2) negras
		        {cv,cv,1,cv,cv,cv,cv,cv},
		        {cv,cv,2,cv,cv,cv,cv,cv},
		        {cv,cv,cv,cv,cv,cv,cv,cv}},

		        {{cv,cv,cv,cv,2,1,1,2},			// tablero de prueba 3
		        {cv,cv,cv,cv,cv,cv,1,1},		// (0,7) negras
		        {cv,cv,cv,cv,cv,1,cv,2},
		        {cv,cv,cv,cv,1,cv,cv,cv},
		        {cv,cv,cv,1,cv,cv,cv,cv},
		        {cv,cv,1,cv,cv,cv,cv,cv},
		        {cv,1,cv,cv,cv,cv,cv,cv},
		        {2,cv,cv,cv,cv,cv,cv,cv}}
	};

	char cases[][3]={
			{3,2,1},{4,2,2},{0,7,2}
	};

    char SF, SC; // cantidades a sumar para movernos en la direcciï¿½n que toque
    int i,j, flipC, flipT, patronC, patronT, pruebas,PrimerFallo;
    pruebas=3;
    PrimerFallo=0;
    j=0;
    while (j<pruebas && !PrimerFallo){
    	i=0;
    	while (i < DIM && !PrimerFallo)
		{
			SF = vSF[i];
			SC = vSC[i];
			// flip: numero de fichas a voltear
			flipC = 0; flipT = 0;
			patronC = patron_volteo(tableros[j], &flipC, cases[j][0],cases[j][1], SF, SC, cases[j][2]);
			patronT = patron_volteo_arm(tableros[j], &flipT, cases[j][0],cases[j][1], SF, SC, cases[j][2]); //patron_volteo_arm
			//printf("Flip: %d \n", flip);

			if (patronC != patronT || flipC != flipT){
				PrimerFallo=j+1;
				break;
			}
			i++;
		}
		j++;
    }
}

char __attribute__ ((aligned (8))) tablero_tiempos[DIM][DIM] = {
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA}
	    };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 0 indica CASILLA_VACIA, 1 indica FICHA_BLANCA y 2 indica FICHA_NEGRA
// pone el tablero a cero y luego coloca las fichas centrales.
void init_table(char tablero[][DIM], char candidatas[][DIM])
{
    int i, j;

    for (i = 0; i < DIM; i++)
    {
        for (j = 0; j < DIM; j++)
            tablero[i][j] = CASILLA_VACIA;
    }
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

// Tabla de direcciones. Contiene los desplazamientos de las 8 direcciones posibles
const char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
const char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};

////////////////////////////////////////////////////////////////////////////////
// comprueba si hay que actualizar alguna ficha
// no comprueba que el movimiento realizado sea válido
// f y c son la fila y columna a analizar
// char vSF[DIM] = {-1,-1, 0, 1, 1, 1, 0,-1};
// char vSC[DIM] = { 0, 1, 1, 1, 0,-1,-1,-1};
int actualizar_tablero(char tablero[][DIM], char f, char c, char color)
{
    char SF, SC; // cantidades a sumar para movernos en la dirección que toque
    int i, flip, patron;

    for (i = 0; i < DIM; i++) // 0 es Norte, 1 NE, 2 E ...
    {
        SF = vSF[i];
        SC = vSC[i];
        // flip: numero de fichas a voltear
        flip = 0;
        patron = patron_volteo(tablero, &flip, f, c, SF, SC, color);
        //printf("Flip: %d \n", flip);
        if (patron == PATRON_ENCONTRADO )
        {
            voltear(tablero, f, c, SF, SC, flip, color);
        }
    }
    return 0;
}

void test_tiempo(){
	char __attribute__ ((aligned (8))) candidatas[DIM][DIM] =
    {
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO},
        {NO,NO,NO,NO,NO,NO,NO,NO}
    };

	init_table(tablero_tiempos, candidatas);
	int longitud=0;
	int flip=0;
	//actualizar_tablero(tablero_tiempos,2,3,2);
	int i=0;
	while ( i<1000000 ){
		flip=0;
		patron_volteo(tablero_tiempos,&flip,2,3,-1,0,2);
		i++;
	}
	int e=0;

}
