#include "lcd.h"
#include "lcdManager.h"

void Init_LcdManager() {
    Lcd_Init();
    Lcd_Clr();
}

// No poner estas variables como static const y globales hace que el compilador
// las elimine al compilar con -O2 (Si se declaran dentro de una funcion este las
// sustituye por un array vacio)
static const  INT8U  texto_instrucciones[]  = "Instrucciones: ";
static const  INT8U  texto_instrucciones_linea1[]  = "TouchPad: ";
static const  INT8U  texto_instrucciones_linea1b[]  = "-Pulse una region para ampliarla";
static const  INT8U  texto_instrucciones_linea1bb[]  = "-Pulse fuera para volver del zoom";
static const  INT8U  texto_instrucciones_linea1c[]  = "-Pulse una casilla para colocar";
static const  INT8U  texto_instrucciones_linea1c2[]  = " una ficha";
static const  INT8U  texto_instrucciones_linea1d[]  = "-Pulse durante el parpadeo para";
static const  INT8U  texto_instrucciones_linea1d2[]  = " cancelar el movimiento";
static const  INT8U  texto_instrucciones_linea1e[]  = "-Pulse abajo para pasar o finalizar";
static const  INT8U  texto_instrucciones_linea2[]  = "Botones: ";
static const  INT8U  texto_instrucciones_linea2b[]  = "-Pulse Izquierdo para aumentar";
static const  INT8U  texto_instrucciones_linea2b2[]  = " el contador";
static const  INT8U  texto_instrucciones_linea2c[]  = "-Pulse Derecho para confirmar";
static const  INT8U  texto_instrucciones_linea2c2[]  = " fila(f)/columna(C)";
static const  INT8U  texto_instrucciones_linea2d[]  = "-Marque un 8 para pasar/finalizar";
static const  INT8U  texto_instrucciones_linea2e[]  = "-Izquierdo->Pasar  Derecho->Finalizar";
static const  INT8U  texto_instrucciones_pulsa[]  = "Pulsa para inicializar: ";

void LcdM_Show_Instructions(void) {
    Lcd_Clr();
    Lcd_DspAscII8x16(10, 20, BLACK, texto_instrucciones);
    Lcd_DspAscII8x16(18, 40, BLACK,texto_instrucciones_linea1);
    Lcd_DspAscII8x16(22, 50, BLACK,texto_instrucciones_linea1b);
    Lcd_DspAscII8x16(22, 60, BLACK,texto_instrucciones_linea1bb);
    Lcd_DspAscII8x16(22, 70, BLACK,texto_instrucciones_linea1c);
    Lcd_DspAscII8x16(22, 80, BLACK,texto_instrucciones_linea1c2);
    Lcd_DspAscII8x16(22, 90, BLACK,texto_instrucciones_linea1d);
    Lcd_DspAscII8x16(22, 100, BLACK,texto_instrucciones_linea1d2);
    Lcd_DspAscII8x16(22, 110, BLACK,texto_instrucciones_linea1e);
    Lcd_DspAscII8x16(18, 130, BLACK, texto_instrucciones_linea2);
    Lcd_DspAscII8x16(22, 140, BLACK, texto_instrucciones_linea2b);
    Lcd_DspAscII8x16(22, 150, BLACK, texto_instrucciones_linea2b2);
    Lcd_DspAscII8x16(22, 160, BLACK, texto_instrucciones_linea2c);
    Lcd_DspAscII8x16(22, 170, BLACK, texto_instrucciones_linea2c2);
    Lcd_DspAscII8x16(22, 180, BLACK, texto_instrucciones_linea2d);
    Lcd_DspAscII8x16(22, 190, BLACK, texto_instrucciones_linea2e);
    Lcd_DspAscII8x16(10, 220, BLACK, texto_instrucciones_pulsa);
    Lcd_Dma_Trans();

}

static int mostrando = 0;

void LcdM_Show_Board_Position_Selected(int position) {

    int fila_parpadeo = (position - 1) >> 3;
    int columna_parpadeo = (position - 1) % 8;


    if (mostrando == 0) {
        mostrando = 1;
        Lcd_Draw_Disks(24 + 24 * columna_parpadeo, 24 + 24 * fila_parpadeo, 17, BLACK);
        Lcd_Dma_Trans();

    } else {
        mostrando = 0;
        Lcd_Draw_Disks(24 + 24 * columna_parpadeo, 24 + 24 * fila_parpadeo, 17, WHITE);
        Lcd_Dma_Trans();

    }

}

void miAtoi(char resultado[], int numero) {
    char inverso[256];
    int i = 0;
    do {
        inverso[i] = '0' + numero % 10;
        numero = numero / 10;

    } while (numero > 0 && (i = i + 1) < 256);

    int j;
    for (j = 0; j <= i; j++) {
        resultado[j] = inverso[i - j];
    }
    resultado[j] = '\0';
}


void LcdM_Show_Processing_Time(int tiempo_calculos) {
	volatile INT8U  resultado[256];
    miAtoi(resultado, tiempo_calculos);
    Lcd_Draw_Rectangles(240, 134, 40, 16, WHITE); // Limpiamos region anterior
    Lcd_DspAscII8x16(240, 134, BLACK, resultado);
    Lcd_Dma_Trans();
}

void LcdM_Show_Game_Time(int tiempo_juego) {
	volatile INT8U  resultado[256];
    miAtoi(resultado, tiempo_juego);
    Lcd_Draw_Rectangles(232, 46, 40, 16, WHITE); // Limpiamos region anterior
    Lcd_DspAscII8x16(244, 44, BLACK, resultado);
    Lcd_Dma_Trans();
}

static const INT8U texto_resultado[] =  "Resultado: ";
static const INT8U texto_fichas_negras[] =   "Fichas negras: ";
static const INT8U texto_fichas_blancas[] =   "Fichas blancas: ";
static const INT8U texto_ganado_negras[] = "Han ganado las negras";
static const INT8U texto_ganado_blancas[] = "Han ganado las blancas";
static const INT8U texto_empate[] = "Han habido un empate";
static const INT8U texto_volver_jugar[] = "Pulsa para volver a jugar";

void LcdM_Show_Score(int fichas_negras, int fichas_blancas) {
    Lcd_Clr();
    Lcd_DspAscII8x16(10, 20, BLACK, texto_resultado);
    volatile INT8U resultado_negras[256];
    volatile INT8U resultado_blancas[256];
    miAtoi(resultado_blancas, fichas_blancas);
    miAtoi(resultado_negras, fichas_negras);

    Lcd_DspAscII8x16(10, 30, BLACK, texto_fichas_negras);

    Lcd_DspAscII8x16(10 + 8 * 15, 30, BLACK, resultado_negras);


    Lcd_DspAscII8x16(10, 40, BLACK, texto_fichas_blancas);

    Lcd_DspAscII8x16(10 + 8 * 16, 40, BLACK, resultado_blancas);

    if (fichas_negras > fichas_blancas) {

        Lcd_DspAscII8x16(10, 60, BLACK,texto_ganado_negras );
    } else if (fichas_negras < fichas_blancas) {

        Lcd_DspAscII8x16(10, 60, BLACK, texto_ganado_blancas );
    } else {

        Lcd_DspAscII8x16(10, 60, BLACK, texto_empate);
    }

    Lcd_DspAscII8x16(10, 80, BLACK, texto_volver_jugar);
    Lcd_Dma_Trans();
}


static const INT8U texto_pasar_o_finalizar[] ="Pulse o introduzca (8, 8) para pasar";
static const INT8U texto_tiempo[] ="TIEMPO";
static const INT8U texto_juego[] ="JUEGO";
static const INT8U texto_segundos[] ="segundos";
static const INT8U texto_calculo[] ="CALCULO";
static const INT8U texto_ns[] ="ns";

void LcdM_Show_Board_No_Zoom(char tablero[][8]) {
    Lcd_Clr();
    //Dibujar numeraci�n
    int i = 0;
    while (i <= 7) {
        char numero_mostrar[] = "0";
        numero_mostrar[0] += i;
        // Dibuja numero horizontal
        Lcd_DspAscII8x16(28 + 24 * i, 2, BLACK, numero_mostrar);

        // Dibuja n�mero vertical
        Lcd_DspAscII8x16(7, 24 + 24 * i, BLACK, numero_mostrar);
        ++i;
    }

    //Dibujar lineas
    i = 0;
    Lcd_Draw_VLine(20, 212, 0, BLACK, 2);
    Lcd_Draw_HLine(20, 212, 0, BLACK, 2);
    while (i <= 8) {
        // Lineas verticales
        Lcd_Draw_VLine(0, 212, 20 + 24 * i, BLACK, 2);
        // Lineas horizontaless
        Lcd_Draw_HLine(0, 212, 20 + 24 * i, BLACK, 2);
        ++i;
    }

    i = 0;
    while (i < 8) {
        int j = 0;
        while (j < 8) {
            int fila = i;
            int columna = j;
            int color = tablero[fila][columna];
            switch (color) {
                case 1: // Blanca
                    Lcd_Draw_Circle(24 + 24 * columna, 24 + 24 * fila, 17, BLACK);
                    break;
                case 2: // Negra
                    Lcd_Draw_Disks(24 + 24 * columna, 24 + 24 * fila, 17, BLACK);
            }

            ++j;
        }
        ++i;
    }

    // Dibujar mensaje abajo
    Lcd_Draw_Box(2, 218, 314, 236, BLACK);
    Lcd_DspAscII8x16(10, 220, BLACK, texto_pasar_o_finalizar);

    // Dibujar mensajes lados
    Lcd_Draw_Box(224, 10, 304, 42, BLACK);
    Lcd_DspAscII8x16(240, 10, BLACK, texto_tiempo);
    Lcd_DspAscII8x16(244, 26, BLACK, texto_juego);
    Lcd_Draw_Box(224, 42, 304, 76, BLACK);
    Lcd_DspAscII8x16(232, 60, BLACK, texto_segundos);
    Lcd_Draw_Box(224, 100, 304, 132, BLACK);
    Lcd_DspAscII8x16(240, 100, BLACK, texto_tiempo);
    Lcd_DspAscII8x16(236, 116, BLACK, texto_calculo);
    Lcd_Draw_Box(224, 132, 304, 164, BLACK);
    Lcd_DspAscII8x16(256, 150, BLACK, texto_ns);

    Lcd_Dma_Trans();
}

void LcdM_Show_Board_Zoom(char tablero[][8], int region) {
    Lcd_Clr();
    int despX = 0, despY = 0;
    switch (region) {
        case 2:
            despX = 4;
            break;
        case 3:
            despY = 4;
            break;
        case 4:
            despX = 4;
            despY = 4;
            break;
    }

    //Dibujar numeraci�n

    int i = 0;
    while (i <= 3) {
        //char numero_char = '0' + i;
        //char numero_mostrar[] = {numero_char};
        char numero_mostrarX[] = "0";
        char numero_mostrarY[] = "0";
        numero_mostrarX[0] += (i + despX);
        numero_mostrarY[0] += (i + despY);
        // Dibuja numero horizontal
        Lcd_DspAscII8x16(40 + 48 * i, 4, BLACK, numero_mostrarX);

        // Dibuja n�mero vertical
        Lcd_DspAscII8x16(6, 36 + 48 * i, BLACK, numero_mostrarY);
        ++i;
    }

    Lcd_Draw_VLine(20, 212, 0, BLACK, 2);
    Lcd_Draw_HLine(20, 212, 0, BLACK, 2);

    //Dibujar lineas
    i = 0;
    while (i <= 4) {
        // Lineas verticales
        Lcd_Draw_VLine(0, 212, 20 + 48 * i, 14, 2);
        // Lineas horizontaless
        Lcd_Draw_HLine(0, 212, 20 + 48 * i, 14, 2);
        ++i;
    }

    i = 0;
    while (i < 4) {
        int j = 0;
        while (j < 4) {
            int fila = i;
            int columna = j;
            int color = tablero[fila + despY][columna + despX];

            switch (color) {
                case 1: // Blanca
                    Lcd_Draw_Circle(28 + 48 * columna, 28 + 48 * fila, 34, BLACK);
                    break;

                case 2: // Negra
                    Lcd_Draw_Disks(28 + 48 * columna, 28 + 48 * fila, 34, BLACK);
            }
            ++j;
        }
        ++i;
    }

    // Dibujar mensaje abajo
    Lcd_Draw_Box(2, 218, 314, 236, BLACK);

    Lcd_DspAscII8x16(10, 220, BLACK, texto_pasar_o_finalizar);

    // Dibujar mensajes lados
    Lcd_Draw_Box(224, 10, 304, 42, BLACK);

    Lcd_DspAscII8x16(240, 10, BLACK, texto_tiempo);

    Lcd_DspAscII8x16(244, 26, BLACK, texto_juego);

    Lcd_Draw_Box(224, 42, 304, 76, BLACK);

    Lcd_DspAscII8x16(232, 60, BLACK, texto_segundos);


    Lcd_Draw_Box(224, 100, 304, 132, BLACK);
    Lcd_DspAscII8x16(240, 100, BLACK, texto_tiempo);

    Lcd_DspAscII8x16(236, 116, BLACK, texto_calculo);

    Lcd_Draw_Box(224, 132, 304, 164, BLACK);

    Lcd_DspAscII8x16(256, 150, BLACK, texto_ns);

    Lcd_Dma_Trans();
}


static const INT8U texto_pulsa[] ="PULSA";
static const INT8U texto_para[] ="PARA";
static const INT8U texto_pasar[] ="PASAR";

static const INT8U texto_finalizar[] ="FINALIZAR";
static const INT8U texto_volver[] ="VOLVER";

void LcdM_Show_Skip() {
    Lcd_Clr();

    Lcd_DspAscII8x16(60, 36, BLACK, texto_pulsa);
    Lcd_DspAscII8x16(64, 52, BLACK, texto_para);
    Lcd_DspAscII8x16(60, 68, BLACK, texto_pasar);

    Lcd_Draw_VLine(0, 120, 160, BLACK, 2);

    Lcd_DspAscII8x16(220, 36, BLACK, texto_pulsa);
    Lcd_DspAscII8x16(224, 52, BLACK, texto_para);
    Lcd_DspAscII8x16(204, 68, BLACK, texto_finalizar);

    Lcd_Draw_HLine(0, 320, 120, BLACK, 2);

    Lcd_DspAscII8x16(140, 156, BLACK, texto_pulsa);
    Lcd_DspAscII8x16(144, 172, BLACK,texto_para);
    Lcd_DspAscII8x16(136, 188, BLACK, texto_volver);

    Lcd_Dma_Trans();

}
static const char pulsa_numero[] ="Pulsa sobre el numero";
static const INT8U texto_1[] ="1";
static const INT8U texto_2[] ="2";
static const INT8U texto_3[] ="3";
static const INT8U texto_4[] ="4";

void LcdM_Show_Calibrar(int num_pantalla) {
    Lcd_Clr();

    switch (num_pantalla) {
        case 1:
            // Arriba
            Lcd_DspAscII8x16(160, 0, BLACK, texto_1);
            break;
        case 2:
            //Abajo
            Lcd_DspAscII8x16(160, 220, BLACK, texto_2);
            break;
        case 3:
            //Izquierda
            Lcd_DspAscII8x16(0, 120, BLACK, texto_3);
            break;
        case 4:
            //Derecha
            Lcd_DspAscII8x16(310, 120, BLACK, texto_4);
            break;
    }
    Lcd_DspAscII8x16(80, 120, BLACK, pulsa_numero );
    Lcd_Dma_Trans();
}
