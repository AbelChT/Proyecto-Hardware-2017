/*********************************************************************************************
* Fichero:	main.c
* Autor:
* Descrip:	punto de entrada de C
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include <stdint.h>
#include "pila.h"
#include "44blib.h"
#include "44b.h"
#include"timer2.h"

/*--- variables globales ---*/
static uint32_t DSTACK_INIT = 0x0c7ff000;
int DSTACK_LIMIT=64;
struct event* vDSTACK;
int INDEX;
int TAM;


typedef struct event {
	int event;
	int aux;
	int time;
};

void fill(int e, int a, int t, struct event *this){
	(*this).event=e;
	(*this).aux=a;
	(*this).time=t;
}

void init_pila_depuracion(){
	vDSTACK=DSTACK_INIT; //DSTACK;
	//vDSTACK= malloc(DSTACK_LIMIT* sizeof(int));
	INDEX=DSTACK_LIMIT-1;;
	timer2_inicializar();
	timer2_empezar();
}

void push_debug(int ID_evento, int auxData){
	struct event ev;
	fill(ID_evento,auxData,timer2_leer(),&ev);
	vDSTACK[INDEX]=ev;
	INDEX=(INDEX-1)&0x3F;
	TAM+=1;
}

void pop_debug(int *IDevento, int *auxData, int *time){
	if (TAM>0){
		INDEX=(INDEX+1)&0x3F;
		struct event ev=vDSTACK[INDEX];
		*time=vDSTACK[INDEX].time;
		*auxData=vDSTACK[INDEX].aux;
		*IDevento=vDSTACK[INDEX].event;
		TAM-=1;
	}
}
