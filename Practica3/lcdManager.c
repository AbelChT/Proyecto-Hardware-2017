#include "lcd.h"
#include "lcdManager.h"

void LcdM_Show_Instructions(void){
    Lcd_Clr();
    Lcd_Active_Clr();
    Lcd_DspAscII8x16(10,20,BLACK,"Instrucciones: ");
    Lcd_DspAscII8x16(10,30,BLACK,"Instrucciones2: ");
    Lcd_DspAscII8x16(10,40,BLACK,"Instrucciones3: ");
    Lcd_DspAscII8x16(10,50,BLACK,"Pulsa para inicializar: ");
    Lcd_Dma_Trans();

}

void LcdM_Show_Board(char tablero[][8], int tiempo_juego, int tiempo_calculos){
    const int COLOR_FICHA_NEGRA = 14;
    const int COLOR_FICHA_BLANCA = 10;
    Lcd_Clr();
    Lcd_Active_Clr();

    //Dibujar numeraci�n
    int i = 0;
    while (i <= 7){
        //char numero_char = '0' + i;
        //char numero_mostrar[] = {numero_char};
        char numero_mostrar[] = "0";
        numero_mostrar[0]+=i;
        // Dibuja numero horizontal
        Lcd_DspAscII8x16(30 + 24*i,0,BLACK,numero_mostrar);

        // Dibuja n�mero vertical
        Lcd_DspAscII8x16(0,25 + 24*i,BLACK,numero_mostrar);
        ++i;
    }

    //Dibujar lineas
    i = 0;
    while (i <= 8){
        // Lineas verticales
        Lcd_Draw_VLine (20, 212, 20 + 24*i, 14, 2);
        // Lineas horizontaless
        Lcd_Draw_HLine (20, 212, 20 + 24*i, 14, 2);
        ++i;
    }

    i = 0;
    while(i<8){
        int j = 0;
        while(j<8){
            int fila = i;
            int columna = j;
            int color = tablero[fila][columna];
            switch(color){
                case 1: // Blanca
                    Lcd_Draw_Disks(24 + 24*columna, 24 + 24*fila, 17, COLOR_FICHA_BLANCA);
                    break;
                case 2: // Negra
                    Lcd_Draw_Disks(24 + 24*columna, 24 + 24*fila, 17, COLOR_FICHA_NEGRA);
            }

            ++j;
        }
        ++i;
    }

    // Dibujar mensaje abajo
    Lcd_DspAscII8x16(10,220,BLACK,"Pulse o introduzca (8, 8) para pasar");

    // Dibujar mensajes lados
    Lcd_DspAscII8x16(232,10,BLACK,"Tiempo");
    Lcd_DspAscII8x16(232,26,BLACK,"juego:");

    char t_juego [] = "012";
    Lcd_DspAscII8x16(232,46,BLACK,t_juego);

    Lcd_DspAscII8x16(232,76,BLACK,"Tiempo");
    Lcd_DspAscII8x16(232,92,BLACK,"calculo:");
    char buffer[20];
    char t_calculo [] = "012";// Implementar funciones atoi
    Lcd_DspAscII8x16(232, 112, BLACK, t_calculo);

    Lcd_Dma_Trans();
}

void LcdM_Show_Board_Zoom(char tablero[][8], int region){
    const int COLOR_FICHA_NEGRA = 14;
    const int COLOR_FICHA_BLANCA = 10;
    Lcd_Clr();
    Lcd_Active_Clr();
    int despX=0, despY=0;
    switch (region) {
        case 2:
            despX=4;
            break;
        case 3:
            despY=4;
            break;
        case 4:
            despX=4;
            despY=4;
            break;
    }

    //Dibujar numeraci�n

    int i = 0;
    while (i <= 3){
        //char numero_char = '0' + i;
        //char numero_mostrar[] = {numero_char};
        char numero_mostrarX[] = "0";
        char numero_mostrarY[] = "0";
        numero_mostrarX[0]+=(i+despX);
        numero_mostrarY[0]+=(i+despY);
        // Dibuja numero horizontal
        Lcd_DspAscII8x16(30 + 48*i,0,BLACK,numero_mostrarX);

        // Dibuja n�mero vertical
        Lcd_DspAscII8x16(0,25 + 48*i,BLACK,numero_mostrarY);
        ++i;
    }

    //Dibujar lineas
    i = 0;
    while (i <= 4){
        // Lineas verticales
        Lcd_Draw_VLine (20, 212, 20 + 48*i, 14, 2);
        // Lineas horizontaless
        Lcd_Draw_HLine (20, 212, 20 + 48*i, 14, 2);
        ++i;
    }

    i = 0;
    while(i<4){
        int j = 0;
        while(j<4){
            int fila = i;
            int columna = j;
            int color = tablero[fila+despX][columna+despY];
            switch(color){
                case 1: // Blanca
                    Lcd_Draw_Disks(28 + 48*columna, 28 + 48*fila, 34, COLOR_FICHA_BLANCA);
                    break;
                case 2: // Negra
                    Lcd_Draw_Disks(28 + 48*columna, 28 + 48*fila, 34, COLOR_FICHA_NEGRA);
            }

            ++j;
        }
        ++i;
    }

    // Dibujar mensaje abajo
    Lcd_DspAscII8x16(10,220,BLACK,"Pulse o introduzca (8, 8) para pasar");

    // Dibujar mensajes lados
    Lcd_DspAscII8x16(232,10,BLACK,"Tiempo");
    Lcd_DspAscII8x16(232,26,BLACK,"juego:");

    char t_juego [] = "012";
    Lcd_DspAscII8x16(232,46,BLACK,t_juego);

    Lcd_DspAscII8x16(232,76,BLACK,"Tiempo");
    Lcd_DspAscII8x16(232,92,BLACK,"calculo:");
    char buffer[20];
    char t_calculo [] = "012";// Implementar funciones atoi
    Lcd_DspAscII8x16(232, 112, BLACK, t_calculo);

    Lcd_Dma_Trans();
}
