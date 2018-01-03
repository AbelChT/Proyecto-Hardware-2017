/*********************************************************************************************
* File��	tp.c
*********************************************************************************************/

/*--- include files ---*/
#include <string.h>
#include "44b.h"
#include "44blib.h"
#include "tp.h"

/*--- global  variables ---*/
/* Todas han de ser volatile debido a que se modifican o desde la rutina de interrupcion
 * o desde funciones que se llaman desde esta */
volatile static unsigned int Xmax = 0;
volatile static unsigned int Ymax = 0;
volatile static unsigned int Xmin = 2000;
volatile static unsigned int Ymin = 2000;

volatile static unsigned int x = 0;
volatile static unsigned int y = 0;
volatile static int pulsa = 0;
volatile static int calibraciones = 0;


/**
 * Función auxiliar que modifica Xmax e Ymax si se introduce una x o una y superior a la Xmax e Ymax actual
 * */
void ajustar_x_y() {
    if (x > Xmax) {
        Xmax = x;
    } else if (x < Xmin) {
        Xmin = x;
    }
    if (y > Ymax) {
        Ymax = y;
    } else if (y < Ymin) {
        Ymin = y;
    }
}

/**
 * Función auxiliar que calibra la pantalla las 8 primeras veces que se le invoca
 * */
int calibrar_tp() {
    if (calibraciones < 8) {
        ajustar_x_y();
        calibraciones += 1;
        return 1;
    } else {
        return 0;
    }
}

/*********************************************************************************************
* name:		TSInt
* func:		TouchScreen interrupt handler function
* para:		none
* ret:		none
* modify:
* comment:		
********************************************************************************************/
void TSInt(void) __attribute__((interrupt("IRQ")));

void TSInt(void) {
    int i;

    // X position Read
    rPDATE = 0x68;
    rADCCON = 0x1 << 2;            // AIN1

    //DelayTime(5000);                // O0 delay to set up the next channel
    DelayTime(25000);                // O2 delay to set up the next channel
    x = 0;
    for (i = 0; i < 5; i++) {
        rADCCON |= 0x1;                // Start X-position A/D conversion
        while (rADCCON & 0x1);        // Check if Enable_start is low
        while (!(rADCCON & 0x40));    // Check ECFLG
        x += (0x3ff & rADCDAT);
    }
    // read X-position average value
    x = x / 5;


    // Y position Read
    rPDATE = 0x98;
    rADCCON = 0x0 << 2;                    // AIN0

    //DelayTime(5000);                // O0 delay to set up the next channel
    DelayTime(25000);                // O2 delay to set up the next channel
    y = 0;
    for (i = 0; i < 5; i++) {
        rADCCON |= 0x1;             // Start Y-position conversion
        while (rADCCON & 0x1);     // Check if Enable_start is low
        while (!(rADCCON & 0x40)); // Check ECFLG
        y += (0x3ff & rADCDAT);
    }
    // read Y-position average value
    y = y / 5;


    if ((calibrar_tp() == 1) || ((x >= Xmin) && (x <= Xmax) && (y >= Ymin) && (y <= Ymax))) {
        // Ya esta la pantalla calibrada y la posicion es valida o no esta calibrada
        pulsa = 1;
    }

    rPDATE = 0xb8;                  // should be enabled
    // DelayTime(100000);   //ParaO0             // delay to set up the next channel
    DelayTime(500000);     //ParaO2           // delay to set up the next channel
    rI_ISPC = BIT_EINT2;            // clear pending_bit
}


/**
 * Pre: Se ha inicializado el touch screen
 * Post: Obtiene la coordenada x de la última pulsación (el sistema de referencia es el que utiliza el lcd
 * para dibujar los píxeles)
 */
int getX(void) {
    int temp = x - Xmin;
    int dif = Xmax - Xmin;

    return (320 * temp) / dif;
}


/**
 * Pre: Se ha inicializado el touch screen
 * Post: Obtiene la coordenada y de la última pulsación (el sistema de referencia es el que utiliza el lcd
 * para dibujar los píxeles)
 */
int getY(void) {
    int temp = Ymax - y;
    int dif = Ymax - Ymin;

    return ((240 * temp) / dif);
}


/**
 * Pre: Se ha inicializado el touch screen
 * Post: Devuelve 1 si se ha pulsado el touch screen desde la última vez que se invocó a esta función
 */
int haPulsado(void) {
    if (pulsa == 0) {
        return 0;
    } else {
        pulsa = 0;
        return 1;
    }
}


/**
 * Pre: ---
 * Post: Inicializa el touch screen
 */
void TS_init(void) {
    rI_ISPC |= BIT_EINT2;            // clear pending_bit
    rPUPE = 0x0;                     // Pull up
    rPDATE = 0xb8;                   // should be enabled	
    DelayTime(500);

    rEXTINT |= 0x200;                // falling edge trigger

    rCLKCON = 0x7ff8;                // enable clock
    rADCPSR = 0x1;//0x4;             // A/D prescaler
    pISR_EINT2 = (int) TSInt;       // set interrupt handler

    rI_ISPC |= BIT_EINT2;            // clear pending_bit
    rINTMSK = rINTMSK & ~(BIT_GLOBAL | BIT_EINT2);// rINTMSK &
}


/**
 * Pre: Se ha inicializado el touch screen
 * Post: Finaliza el touch screen
 */
void TS_close(void) {
    /* Mask interrupt */
    rINTMSK |= BIT_GLOBAL | BIT_EINT2;
    pISR_EINT2 = (int) NULL;
}
