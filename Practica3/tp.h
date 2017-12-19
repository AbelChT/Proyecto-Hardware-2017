/*********************************************************************************************
* File��	tp.H
* Author:	embest	
* Desc��	Touch Screen define file
* History:	
*********************************************************************************************/

#ifndef __TP_H__
#define __TP_H__

void TS_init(void);
void TSInt(void);
void TS_close(void);
int getX(void);
int getY(void);
int haPulsado(void);

#endif /*__TP_H__*/
