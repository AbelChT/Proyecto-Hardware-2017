.text
#        ENTRY            		/*  mark the first instruction to call */
.global	start
.global ficha_valida_arm
.global patron_volteo_arm
.global ficha_valida_thumb
start:
.arm /*indicates that we are using the ARM instruction set */
#------standard initial code
# --- Setup interrupt / exception vectors
      B       Reset_Handler
/* In this version we do not use the following handlers */
#######################################################################################################
#-----------Undefined_Handler:
#      B       Undefined_Handler
#----------SWI_Handler:
#      B       SWI_Handler
#----------Prefetch_Handler:
#      B       Prefetch_Handler
#----------Abort_Handler:
#      B       Abort_Handler
#         NOP      /* Reserved vector */
#----------IRQ_Handler:
#      B       IRQ_Handler
#----------FIQ_Handler:
#      B       FIQ_Handler
#######################################################################################################
# Reset Handler:
# the processor starts executing this code after system reset
#######################################################################################################
Reset_Handler:
#
        MOV     sp, #0x4000      /*  set up stack pointer (r13) */

.extern     test_ficha_valida_arm
        ldr         r5, = test_ficha_valida_arm
        mov         lr, pc
        bx          r5


stop:
 		B     	stop    	/*  end of program */


################################################################################
# Funcion ARM ficha_valida:
# :Input
# r0 = char tablero[][DIM]
# r1 = char f
# r2 = char c
# r3 = int *posicion_valida
# :Output
# r0 = result
################################################################################
.equ DIM, 8
.equ CASILLA_VACIA, 0
ficha_valida_arm:
#  saves the working registers
		str r4, [sp,#-4]! /*STMFD   sp!, {r4}*/

# si ((f < DIM) && (0 <= f) && (c < DIM) && (0 <= c) && (tablero[f][c] != CASILLA_VACIA))
# Al ser evaluacion cortocircuitada salto despues de cada comparacion
        cmp r1, #DIM  /* cmp f, DIM*/
        bge		ficha_valida_arm_else /* salta si !(f < DIM)*/
        cmp r1, #0 /* cmp f, #0 */
        blt		ficha_valida_arm_else /* salta si !(0 <= f)*/
        cmp r2, #DIM  /* cmp c, DIM*/
        bge		ficha_valida_arm_else /* salta si !(c < DIM)*/
        cmp r2, #0 /* cmp c, #0 */
        blt ficha_valida_arm_else /* salta si !(0 <= c)*/
        mov r4, #DIM
        mla r4, r1, r4, r2 /*r4 ser� la posicion del vector([f][c]) con respecto a tablero -> *tablero[f][c] = r4 + *tablero */
        ldr r0, [r0, r4] /*r0 = tablero[f][c] para en el caso de que entre al if ya tener el resultado*/
        cmp r0 , #CASILLA_VACIA /* cmp tablero[f][c], CASILLA_VACIA */
        beq ficha_valida_arm_else /* salta si (tablero[f][c] != CASILLA_VACIA)*/

ficha_valida_arm_if:
        mov r4, #1 /*r4=1*/
        str r4, [r3] /* *posicion_valida = 1 */
        b ficha_valida_arm_return /* r0 sera igual a tablero[f][c] */

ficha_valida_arm_else:
        mov r0, #0 /* r0 sera igual a CASILLA_VACIA */
        str r0, [r3] /* *posicion_valida = 0 */

ficha_valida_arm_return:
        # restore the original registers
        LDMFD   sp!, {r4} /*ldr r4,[sp],#4*/
        # return to the instruccion that called the rutine and to arm mode
        BX      r14

################################################################################

################################################################################
# Funcion Thumb ficha_valida:
# :Input
# r0 = char tablero[][DIM]
# r1 = char f
# r2 = char c
# r3 = int *posicion_valida
# :Output
# r0 = result
################################################################################
ficha_valida_thumb:
#  cambio de contexto a thumb
        ADR r7, Tstart + 1 /* Processor starts in ARM state, */
        BX r7 /* small ARM code header used to call Thumb main program. */

.thumb
Tstart:
        MOV r7, #10 /* Set up parameters */
#  saves the working registers
        PUSH {r1-r7}
# si ((f < DIM) && (0 <= f) && (c < DIM) && (0 <= c) && (tablero[f][c] != CASILLA_VACIA))
# Al ser evaluacion cortocircuitada salto despues de cada comparacion
        mov r5, #8 /*r5 = DIM*/
        mov r4 , #0 /*r4 = 0 , como CASILLA_VACIA=0 entonces r4=CASILLA_VACIA*/
        cmp r1, r5 /*(f < DIM)*/
        bge		ficha_validaElse_thumb
        cmp r4, r1 /*(0 <= f)*/
        bgt		ficha_validaElse_thumb
        cmp r2, r5 /*(c < DIM)*/
        bge		ficha_validaElse_thumb
        cmp r4, r2 /*(0 <= c)*/
        bgt 	ficha_validaElse_thumb
        mul r1, r1, r5
        add r1, r1, r2
# Obtener r6 = tablero[f][c]
        ldr r7, [r0, r1] /*r7 = tablero[f][c]*/
        cmp r7 , r4 /*(tablero[f][c] != CASILLA_VACIA)*/
        beq ficha_validaElse_thumb

ficha_validaIf_thumb:
# *posicion_valida = 1;
        mov r4, #1 /*r4=1*/
        str r4, [r3]
# ficha = tablero[f][c];
        mov r0, r7 /* result = tablero[f][c] */
        b ficha_valida_return_thumb

ficha_validaElse_thumb:
# *posicion_valida = 0;
        str r4, [r3]
        mov r0, r4 /* result = CASILLA_VACIA*/

ficha_valida_return_thumb:
# restore the original registers
        POP {r1-r7}
# return to the instruccion that called the rutine and to arm mode
        BX      r14

################################################################################

.arm
################################################################################
# Funcion ARM patron_volteo:
# :Input
# r0 = char tablero[][DIM]
# r1 = int *longitud
# r2 = char FA
# r3 = char CA
# Pila
# char SF
# char SC
# char color
# :Output
# r0 = result
################################################################################
# Constantes

.equ PATRON_ENCONTRADO, 1
.equ NO_HAY_PATRON, 0

patron_volteo_arm:
#  saves the working registers
        STMFD   sp!, {fp}
        add fp, sp , #4
        STMFD   sp!, {r4-r11}
        LDMFD   fp!, {r8-r10}
/*
* r8 = char SF
* r9 = char SC
* r10 = char color
*/
# Codigo
        add r6, r2, r8
        add r7,r3,r9
        mov r4, r0
        mov r5, r1
/*
* r4 = char tablero[][DIM]
* r5 = int *longitud
* r6 = char FA
* r7 = char CA
*/
# LLamada a fichavalida
/* Actualizo FA y CA para pasarlos*/
        mov r1,r6
        mov r2,r7
        sub sp,sp,#4 /* Reservo un entero en la pila para almacenar posicion_valida*/
        mov r3,sp
/* Los registros del 0 al 3 ya están guardados en los registros 4-7 */
        mov r11, lr
/* r11=lr */
        mov lr, pc
        bx =ficha_valida_arm /*para thumb usar bx y descomentar lo de arriba*/
        mov lr, r11 /*Recupero el lr*/
        LDMFD sp!, {r1} /*Recupero posicion_valida */
/*
* r0 = casilla
* r1 = posicion_valida
*/
# Fin de llamada a fichavalida
        cmp  r1, #1
        bne patron_volteo_arm_else
        cmp r0, r10
        beq patron_volteo_arm_else_if
patron_volteo_arm_if:
        ldr r2, [r5] /* r2 = longitud */
        add r2, r2, #1
        str r2, [r5]

# LLamada a patronVolteo
/*No hace falta guardar los registros ya que no se van a usar*/
        STMFD sp!, {r8-r10}
        mov r11, lr
/* r11=lr */
        mov lr, pc
        bx =patron_volteo_arm /*para thumb usar bx y descomentar lo de arriba*/
        mov lr, r11 /*Recupero el lr*/
        add sp, sp ,#12 /*Elimino los parametros de la pila*/
# Fin lLamada a patronVolteo
/*Patron = r0*/
        b patron_volteo_return

patron_volteo_arm_else_if:
        ldr r2, [r5] /* r2 = longitud */
        cmp r2 , #0
        movlt r0, #PATRON_ENCONTRADO
        movge r0, #NO_HAY_PATRON
        b patron_volteo_return
patron_volteo_arm_else:
        mov r0, #NO_HAY_PATRON
# restore the original registers
patron_volteo_return:
        LDMFD   sp!, {r4-r11}
        LDMFD   sp!, {fp}
# return to the instruccion that called the rutine and to arm mode
        BX      r14

#################################################################################################################

.data

.end
#        END
