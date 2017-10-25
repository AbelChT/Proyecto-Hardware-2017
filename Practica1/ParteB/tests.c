/*
* Fichero de pruebas
*/
#define DIM   8
#define PATRON_ENCONTRADO    1
#define NO_HAY_PATRON        0
#define CASILLA_VACIA 0
#define FICHA_BLANCA  1
#define FICHA_NEGRA   2
#define NO               0
#define SI               1
#define CASILLA_OCUPADA  2

// Funciones ARM
extern char ficha_valida_arm(char tablero[][DIM], char f, char c, int *posicion_valida);
extern char ficha_valida_thumb(char tablero[][DIM], char f, char c, int *posicion_valida);
extern int patron_volteo_arm(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color);

// Los siguientes punteros a funciones se utilizan para lograr hacer las pruebas autom�ticas de las 6 combinaciones
char (*ficha_valida_a_usar)(char tablero[][DIM], char f, char c, int *posicion_valida);
int (*patron_volteo_a_usar)(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color);

static char __attribute__ ((aligned (8))) tablero[DIM][DIM] = {
	        {3,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,7,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA},
	        {CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,CASILLA_VACIA,17}
};

/* Función ficha valida original */
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

/* Función patrón volteo original */
int patron_volteo(char tablero[][DIM], int *longitud, char FA, char CA, char SF, char SC, char color)
{
    int posicion_valida; // indica si la posici�n es valida y contiene una ficha de alg�n jugador
    int patron; //indica si se ha encontrado un patr�n o no
    char casilla;   // casilla es la casilla que se lee del tablero
    FA = FA + SF;
    CA = CA + SC;
    casilla = (*ficha_valida_a_usar)(tablero, FA, CA, &posicion_valida);
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
 * Testea el correcto funcionamiento de ficha_valida
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

	int j=10; // Colocar un breackpoint para conocer si ha fallado alguna prueba

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

    char SF, SC; // cantidades a sumar para movernos en la direcci�n que toque
    int i, j, pruebas, PrimerFallo;
    int flip_C_C, flip_C_ARM, flip_C_THUMB, flip_ARM_C, flip_ARM_ARM, flip_ARM_THUMB,
    patron_C_C, patron_C_ARM, patron_C_THUMB, patron_ARM_C, patron_ARM_ARM, patron_ARM_THUMB;

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
			flip_C_C = 0;
			flip_C_ARM = 0;
			flip_C_THUMB = 0;
			flip_ARM_C = 0;
			flip_ARM_ARM = 0;
			flip_ARM_THUMB = 0;

			// Pruebas C-C
			ficha_valida_a_usar = &ficha_valida;
			patron_volteo_a_usar = &patron_volteo;
			patron_C_C = (*patron_volteo_a_usar)(tableros[j], &flip_C_C, cases[j][0],cases[j][1], SF, SC, cases[j][2]);

      // Pruebas C-ARM
      ficha_valida_a_usar = &ficha_valida_arm;
			patron_volteo_a_usar = &patron_volteo;
			patron_C_ARM = (*patron_volteo_a_usar)(tableros[j], &flip_C_ARM, cases[j][0],cases[j][1], SF, SC, cases[j][2]);

      // Pruebas C-THUMB
      ficha_valida_a_usar = &ficha_valida_thumb;
			patron_volteo_a_usar = &patron_volteo;
			patron_C_THUMB = (*patron_volteo_a_usar)(tableros[j], &flip_C_THUMB, cases[j][0],cases[j][1], SF, SC, cases[j][2]);

      // Pruebas ARM-C
      ficha_valida_a_usar = &ficha_valida;
			patron_volteo_a_usar = &patron_volteo_arm;
			patron_ARM_C = (*patron_volteo_a_usar)(tableros[j], &flip_ARM_C, cases[j][0],cases[j][1], SF, SC, cases[j][2]);

      // Pruebas ARM-ARM
      ficha_valida_a_usar = &ficha_valida_arm;
			patron_volteo_a_usar = &patron_volteo_arm;
			patron_ARM_ARM = (*patron_volteo_a_usar)(tableros[j], &flip_ARM_ARM, cases[j][0],cases[j][1], SF, SC, cases[j][2]);

      ficha_valida_a_usar = &ficha_valida_thumb;
			patron_volteo_a_usar = &patron_volteo_arm;
			patron_ARM_THUMB = (*patron_volteo_a_usar)(tableros[j], &flip_ARM_THUMB, cases[j][0],cases[j][1], SF, SC, cases[j][2]);


			//printf("Flip: %d \n", flip);

			if (patron_C_C != patron_C_ARM || patron_C_C != patron_C_THUMB || patron_C_C != patron_ARM_C || patron_C_C != patron_ARM_ARM || patron_C_C != patron_ARM_THUMB  ||
         flip_C_C != flip_C_ARM || flip_C_C != flip_C_THUMB || flip_C_C != flip_ARM_C || flip_C_C != flip_ARM_ARM || flip_C_C != flip_ARM_THUMB){

				PrimerFallo=j+1;
				break;
			}
			i++;
		}
		j++;
    }
    // Colocar un breackpoint para conocer si ha fallado alguna prueba
}

static char __attribute__ ((aligned (8))) tablero_tiempos[DIM][DIM] = {
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

void test_tiempo(){
	ficha_valida_a_usar = &ficha_valida;
	patron_volteo_a_usar = &patron_volteo;

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
	int i=0;
	while ( i<1000000 ){
		flip=0;
		(*patron_volteo_a_usar)(tablero_tiempos,&flip,2,3,-1,0,2);
		i++;
	}
	int e=0;

}
