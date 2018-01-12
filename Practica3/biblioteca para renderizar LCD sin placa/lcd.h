/*********************************************************************************************
* Fichero:	LCD.H
* Autor:	
* Descrip:	definicion de madro y declaracion de funciones
* Version:	
*********************************************************************************************/

#ifndef __LCD_H__
#define __LCD_H__



/*--- define macros---*/
#define UINT	unsigned int
#define USHORT	unsigned short
#define UCHAR	unsigned char
#define U32 	unsigned int
#define INT32U  unsigned int
#define INT32	int
#define U16 	unsigned short
#define INT16U  unsigned short
#define INT16	short int
#define S32 	int
#define S16 	short int
#define U8  	unsigned char
#define INT8U 	unsigned char
#define byte	unsigned char
#define INT8 	char
#define	S8  	char


#define SCR_XSIZE 			(320)
#define SCR_YSIZE 			(240)
#define LCD_XSIZE 			(320)
#define LCD_YSIZE 			(240)


#define BLACK 				0xf
#define WHITE 				0x0
#define LIGHTGRAY   		0x5
#define DARKGRAY    		0xa
#define TRANSPARENCY 		0xff

/*--- declaracion de funciones ---*/

void  Lcd_Clr(void);
void  Lcd_Dma_Trans(void);
void  Lcd_Draw_Box(INT16 usLeft, INT16 usTop, INT16 usRight, INT16 usBottom, INT8U ucColor);
void  Lcd_Draw_Line(INT16 usX0, INT16 usY0, INT16 usX1, INT16 usY1, INT8U ucColor, INT16U usWidth);
void  Lcd_Draw_HLine(INT16 usX0, INT16 usX1, INT16 usY0, INT8U ucColor, INT16U usWidth);
void  Lcd_Draw_VLine(INT16 usY0, INT16 usY1, INT16 usX0, INT8U ucColor, INT16U usWidth);
void  Lcd_Anti_Disp(INT16U usX0, INT16U usY0, INT16U usX1, INT16U usY1);
void  Lcd_DisplayChar(INT16U usX0, INT16U usY0, INT8U ucChar);
void  Lcd_DisplayString(INT16U usX0, INT16U usY0, INT8U *pucStr);
void  Lcd_DisplayShort(INT16 sX, INT16 sY, INT16U usInt);
void  Lcd_DspAscII6x8(INT16U usX0, INT16U usY0,INT8U ForeColor, INT8U* pucChar);
void  Lcd_DspAscII8x16(INT16U x0, INT16U y0, INT8U ForeColor, INT8U* s);
void  Lcd_DspHz16(INT16U x0, INT16U y0, INT8U ForeColor, INT8U *s);
void  ReverseLine(INT32U ulHeight, INT32U ulY);
INT8U LCD_GetPixel(INT16U usX, INT16U usY);
void Lcd_Init(void);
void Lcd_Draw_Disks(INT16 usX0, INT16 usY0, INT16 ancho, INT8U ucColor);

void Lcd_Draw_Rectangles(INT16 usX0, INT16 usY0, INT16 ancho, INT16 alto, INT8U ucColor);

void Lcd_Draw_Circle(INT16 usX0, INT16 usY0, INT16 ancho, INT8U ucColor);



#endif /* __LCD_H__ */
