/*********************************************************************************************
* Fichero:	button.c
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "button_rebotes.h"
#include "44b.h"


/*--- variables globales del m�dulo ---*/
/* int_count la utilizamos para sacar un n�mero por el 8led.
  Cuando se pulsa un bot�n sumamos y con el otro restamos. �A veces hay rebotes! */

#define ESTADO_BOTON_ESPERANDO 0
#define ESTADO_BOTON_PULSADO 1
#define ESTADO_BOTON_MANTENIDO 2
#define ESTADO_BOTON_REBOTES_INICIALES 3
#define ESTADO_BOTON_REBOTES_FINALES 4

//volatile typedef enum {ESPERANDO, PULSADO, MANTENIDO, REBOTES_INICIALES, REBOTES_FINALES} estados_boton;


volatile static int estado_actual;
volatile static int boton_en_gestion;
volatile static int boton_pulsado;
volatile static int interrupciones_mantenido;

/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void Botones_ISR(void) __attribute__((interrupt("IRQ")));

void gestor_anti_rebotes(void);

void timer0_inicializar(void);

/*--- codigo de funciones ---*/
void Botones_ISR(void) {
    /* Identificar la interrupcion (hay dos pulsadores)*/
    int which_int = rEXTINTPND;

    switch (which_int) {
        case 4:
            boton_pulsado = BOTON_PULSADO_IZQUIERDA;
            boton_en_gestion = BOTON_PULSADO_IZQUIERDA;
            break;
        case 8:
            boton_pulsado = BOTON_PULSADO_DERECHA;
            boton_en_gestion = BOTON_PULSADO_DERECHA;
    }

    estado_actual = ESTADO_BOTON_PULSADO;

    gestor_anti_rebotes();


    /* Finalizar ISR */
    rEXTINTPND = 0xf;                // borra los bits en EXTINTPND
    rI_ISPC |= BIT_EINT4567;        // borra el bit pendiente en INTPND


}

void Botones_anti_inicializar(void) {
    /* Configuracion del controlador de interrupciones. Estos registros est�n definidos en 44b.h */
    pISR_EINT4567 = (int) Botones_ISR;

    rPCONG = 0xffff;                // Establece la funcion de los pines (EINT0-7)
    rPUPG = 0x0;                  // Habilita el "pull up" del puerto
    rEXTINT = rEXTINT | 0x22222222;   // Configura las lineas de int. como de flanco de bajada

    /* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
    rI_ISPC |= (BIT_EINT4567);
    rEXTINTPND = 0xf;

    rINTMSK = rINTMSK & ~(BIT_GLOBAL | BIT_EINT4567);

    // inicializa el timer que se encargará de gestionar los rebotes

    estado_actual = ESTADO_BOTON_ESPERANDO;
    boton_pulsado = BOTON_NO_PULSADO;
    boton_en_gestion = BOTON_NO_PULSADO;

    timer0_inicializar();
}


/* declaraci�n de funci�n que es rutina de servicio de interrupci�n
 * https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html */
void timer0_ISR(void) __attribute__((interrupt("IRQ")));

/*--- codigo de las funciones ---*/
void timer0_ISR(void) {
    gestor_anti_rebotes();

    /* borrar bit en I_ISPC para desactivar la solicitud de interrupci�n*/
    rI_ISPC |= BIT_TIMER0; // BIT_TIMER2 est� definido en 44b.h y pone un uno en el bit 13 que correponde al Timer2
}

void timer0_inicializar(void) {
    /* Configuraion controlador de interrupciones */
    //rINTMOD = 0x00; // Configura las linas como de tipo IRQ  8????
    //rINTCON = 0x0; // Habilita int. vectorizadas y la linea IRQ (FIQ no)

    /* Establece la rutina de servicio para TIMER2 */
    pISR_TIMER0 = (unsigned) timer0_ISR;

    /* Configura el Timer0 */
    rTCFG0 = rTCFG0 | 0x255; // factor de preescalado maximo, para aumentar el retardo de los pulsos
    rTCFG1 = (rTCFG1 & ~(0xf)) |
             0x4; // selecciona la entrada del mux que proporciona el reloj. La 00 corresponde a un divisor de 1/2.
    rTCNTB0 = TIEMPO_RETARDO_REBOTES;
    rTCMPB0 = 200; // valor de comparaci�n
}

void gestor_anti_rebotes(void) {
    switch (estado_actual) {
        case ESTADO_BOTON_PULSADO:
            // Se inicializa el timer
            rINTMSK = rINTMSK & ~(BIT_GLOBAL |
                                  BIT_TIMER0); // Emascara todas las lineas excepto Timer2 y el bit global (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 est�n definidos en 44b.h)
            //rTCNTB0 = TIEMPO_RETARDO_REBOTES; Desde la vez anterior se mantiene
            rTCON = (rTCON & ~(0xf)) | 0x2;
            rTCON = (rTCON & ~(0xf)) | 0x09;
            rINTMSK = rINTMSK | (BIT_EINT4567);
            estado_actual = ESTADO_BOTON_REBOTES_INICIALES;
            interrupciones_mantenido = 0;
            break;

        case ESTADO_BOTON_REBOTES_INICIALES:
            rTCNTB0 = TIEMPO_ENCUESTA_PERIODICA;
            rTCON = (rTCON & ~(0xf)) | 0x2;
            rTCON = (rTCON & ~(0xf)) | 0x09;
            estado_actual = ESTADO_BOTON_MANTENIDO;
            break;

        case ESTADO_BOTON_MANTENIDO:
            if (((boton_en_gestion == BOTON_PULSADO_DERECHA) && ((rPDATG & (0x1 << 7)) != 0))
                || ((boton_en_gestion == BOTON_PULSADO_IZQUIERDA) && ((rPDATG & (0x1 << 6)) !=
                                                                      0))) { // Si alguna de las posiciones es igual a 0 implica que esta pulsado
                rTCNTB0 = TIEMPO_RETARDO_REBOTES;
                rTCON = (rTCON & ~(0xf)) | 0x2;
                rTCON = (rTCON & ~(0xf)) | 0x09;
                estado_actual = ESTADO_BOTON_REBOTES_FINALES;

            } else if (interrupciones_mantenido == TIEMPO_MANTENIDO_PULSACION) {
                interrupciones_mantenido = 0;
                boton_pulsado = boton_en_gestion;
            } else {
                ++interrupciones_mantenido;
            }
            break;

        case ESTADO_BOTON_REBOTES_FINALES:
            rEXTINTPND = 0xf;                // borra los bits en EXTINTPND
            rI_ISPC |= BIT_EINT4567;        // borra el bit pendiente en INTPND
            rINTMSK = rINTMSK & ~(BIT_EINT4567);
            rINTMSK = rINTMSK | (BIT_TIMER0);

            //Elimino interrupciones timer
            rTCON = rTCON & ~(0xf);
            estado_actual = ESTADO_BOTON_ESPERANDO;
            boton_en_gestion = BOTON_NO_PULSADO;
            break;

        case ESTADO_BOTON_ESPERANDO:
            //Vacio
            break;
    }
}

// Devuelve 1 solo la primera vez
int estado_botones(void) {
    int devolver = boton_pulsado;
    boton_pulsado = BOTON_NO_PULSADO;
    return devolver;
}

