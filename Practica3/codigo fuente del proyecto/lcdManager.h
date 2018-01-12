
#ifndef _LCDMANAGER_H
#define _LCDMANAGER_H

void LcdM_Show_Instructions();

void LcdM_Show_Score(int fichas_negras, int fichas_blancas);

void LcdM_Show_Board_No_Zoom(char tablero[][8]);

/**
 * Pre: position es conforme a la matriz
 *   1  2  3  4
 *   5  6  7  8
 *   9 10 11 12
 *  13 14 15 16
 * */
void LcdM_Show_Board_Position_Selected(int position);

void LcdM_Show_Game_Time(int tiempo_juego);

void LcdM_Show_Processing_Time(int tiempo_calculos);

/**
 * Pre: La region es conforme a la matriz
 *  1  2
 *  3  4
 *  Post: Muestra la pantalla con el zoom indicado
 * */
void LcdM_Show_Board_Zoom(char tablero[][8], int region);

void Init_LcdManager();

// La pantalla estará dividida en dos y en la derecha podrá toca aqui o pulsa boton derecho para finalizar
// y en la izquierda lo mismo pero para saltar
void LcdM_Show_Skip();

void LcdM_Show_Calibrar(int num_pantalla);


#endif //_LCDMANAGER_H
