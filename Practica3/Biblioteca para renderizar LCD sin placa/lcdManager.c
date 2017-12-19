#include "lcd.h"
#include "lcdManager.h"

void LcdM_Show_Instructions(void) {
    Lcd_Clr();
    Lcd_DspAscII8x16(10, 20, BLACK, "Reversi");
    Lcd_DspAscII8x16(10, 30, BLACK, "Instrucciones2: ");
    Lcd_DspAscII8x16(10, 40, BLACK, "Instrucciones3: ");
    Lcd_DspAscII8x16(10, 50, BLACK, "Pulsa para inicializar: ");
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
    char resultado[256];
    miAtoi(resultado, tiempo_calculos);
    Lcd_Draw_Rectangles(240, 134, 40, 16, WHITE); // Limpiamos region anterior
    Lcd_DspAscII8x16(240, 134, BLACK, resultado);
    Lcd_Dma_Trans();
}

void LcdM_Show_Game_Time(int tiempo_juego) {
    char resultado[256];
    miAtoi(resultado, tiempo_juego);
    Lcd_Draw_Rectangles(232, 46, 40, 16, WHITE); // Limpiamos region anterior
    Lcd_DspAscII8x16(244, 44, BLACK, resultado);
    Lcd_Dma_Trans();
}

void LcdM_Show_Score(int fichas_negras, int fichas_blancas) {
    Lcd_Clr();
    Lcd_DspAscII8x16(10, 20, BLACK, "Resultado: ");
    char resultado_negras[256];
    char resultado_blancas[256];
    miAtoi(resultado_blancas, fichas_blancas);
    miAtoi(resultado_negras, fichas_negras);

    Lcd_DspAscII8x16(10, 30, BLACK, "Fichas negras: ");

    Lcd_DspAscII8x16(10 + 8 * 15, 30, BLACK, resultado_negras);

    Lcd_DspAscII8x16(10, 40, BLACK, "Fichas blancas: ");

    Lcd_DspAscII8x16(10 + 8 * 16, 40, BLACK, resultado_blancas);


    if (fichas_negras > fichas_blancas) {
        Lcd_DspAscII8x16(10, 60, BLACK, "Han ganado las negras");
    } else if (fichas_negras < fichas_blancas) {
        Lcd_DspAscII8x16(10, 60, BLACK, "Han ganado las negras");
    } else {
        Lcd_DspAscII8x16(10, 60, BLACK, "Han habido un empate");
    }
    //Lcd_DspAscII8x16(10,40,BLACK,"Instrucciones3: ");
    Lcd_DspAscII8x16(10, 80, BLACK, "Pulsa para volver a jugar");
    Lcd_Dma_Trans();
}


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
    Lcd_DspAscII8x16(10, 220, BLACK, "Pulse o introduzca (8, 8) para pasar");

    // Dibujar mensajes lados
    Lcd_Draw_Box(224, 10, 304, 42, BLACK);
    Lcd_DspAscII8x16(240, 10, BLACK, "TIEMPO");
    Lcd_DspAscII8x16(244, 26, BLACK, "JUEGO");

    Lcd_Draw_Box(224, 42, 304, 76, BLACK);

    Lcd_DspAscII8x16(232, 60, BLACK, "segundos");


    Lcd_Draw_Box(224, 100, 304, 132, BLACK);
    Lcd_DspAscII8x16(240, 100, BLACK, "TIEMPO");
    Lcd_DspAscII8x16(236, 116, BLACK, "CALCULO");

    Lcd_Draw_Box(224, 132, 304, 164, BLACK);

    Lcd_DspAscII8x16(256, 150, BLACK, "ms");


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
    Lcd_DspAscII8x16(10, 220, BLACK, "Pulse o introduzca (8, 8) para pasar");

    // Dibujar mensajes lados
    Lcd_Draw_Box(224, 10, 304, 42, BLACK);
    Lcd_DspAscII8x16(240, 10, BLACK, "TIEMPO");
    Lcd_DspAscII8x16(244, 26, BLACK, "JUEGO");

    Lcd_Draw_Box(224, 42, 304, 76, BLACK);


    Lcd_DspAscII8x16(232, 60, BLACK, "segundos");


    Lcd_Draw_Box(224, 100, 304, 132, BLACK);
    Lcd_DspAscII8x16(240, 100, BLACK, "TIEMPO");
    Lcd_DspAscII8x16(236, 116, BLACK, "CALCULO");

    Lcd_Draw_Box(224, 132, 304, 164, BLACK);

    Lcd_DspAscII8x16(256, 150, BLACK, "ms");


    Lcd_Dma_Trans();
}

void LcdM_Show_Skip() {
    Lcd_Clr();
    Lcd_DspAscII8x16(60, 36, BLACK, "PULSA");
    Lcd_DspAscII8x16(64, 52, BLACK, "PARA");
    Lcd_DspAscII8x16(60, 68, BLACK, "PASAR");

    Lcd_Draw_VLine(0, 120, 160, BLACK, 2);




    Lcd_DspAscII8x16(220, 36, BLACK, "PULSA");
    Lcd_DspAscII8x16(224, 52, BLACK, "PARA");
    Lcd_DspAscII8x16(204, 68, BLACK, "FINALIZAR");

    Lcd_Draw_HLine(0, 320, 120, BLACK, 2);

    Lcd_DspAscII8x16(140, 156, BLACK, "PULSA");
    Lcd_DspAscII8x16(144, 172, BLACK, "PARA");
    Lcd_DspAscII8x16(136, 188, BLACK, "VOLVER");


    Lcd_Dma_Trans();

}

void LcdM_Show_Calibrar(int num_pantalla) {
    Lcd_Clr();
    switch (num_pantalla) {
        case 1:
            // Arriba
            Lcd_DspAscII8x16(160, 0, BLACK, "1");
            break;
        case 2:
            //Abajo
            Lcd_DspAscII8x16(160, 220, BLACK, "2");
            break;
        case 3:
            //Izquierda
            Lcd_DspAscII8x16(0, 120, BLACK, "3");
            break;
        case 4:
            //Derecha
            Lcd_DspAscII8x16(310, 120, BLACK, "4");
            break;
    }
    Lcd_DspAscII8x16(80, 120, BLACK, "Pulsa sobre el numero");
    Lcd_Dma_Trans();
}
