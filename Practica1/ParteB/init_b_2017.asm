.text
#        ENTRY            		/*  mark the first instruction to call */
.global	start
.global ARM_ficha_valida
.global THUMB_ficha_valida
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
ARM_ficha_valida:
#  saves the working registers
        STMFD   sp!, {r4-r11}

# si ((f <= DIM) && (0 <= f) && (c <= DIM) && (0 <= c) && (tablero[f][c] != CASILLA_VACIA))
# Al ser evaluacion cortocircuitada salto despues de cada comparacion
        mov r5, #8 /*r5 = DIM*/
        mov r4 , #0 /*r4 = 0 , como CASILLA_VACIA=0 entonces r4=CASILLA_VACIA*/
        cmp r1, r5 /*(f <= DIM)*/
        bgt		ARM_ficha_validaElse
        cmp r4, r1 /*(0 <= f)*/
        bgt		ARM_ficha_validaElse
        cmp r2, r5 /*(c <= DIM)*/
        bgt		ARM_ficha_validaElse
        cmp r4, r2 /*(0 <= c)*/
        bgt ARM_ficha_validaElse
        mla r6, r1, r5, r2 /*r6 la posicion del vector([f][c]) (sin multiplicar por 4)*/
        # Obtener r6 = tablero[f][c]
        ldr r7, [r0, r6] /*r7 = tablero[f][c]*/
        cmp r7 , r4 /*(tablero[f][c] != CASILLA_VACIA)*/
        beq ARM_ficha_validaElse
        B ARM_ficha_validaIf

ARM_ficha_validaIf:
# *posicion_valida = 1;
        mov r4, #1 /*r4=1*/
        str r4, [r3]
# ficha = tablero[f][c];
        mov r0, r7 /* result = tablero[f][c] */
        b ARM_ficha_valida_return

ARM_ficha_validaElse:
# *posicion_valida = 0;
        str r4, [r3]
        mov r0, r4 /* result = CASILLA_VACIA*/

ARM_ficha_valida_return:
        # restore the original registers
        LDMFD   sp!, {r4-r11}
        # return to the instruccion that called the rutine and to arm mode
        BX      r14

#################################################################################################################

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
THUMB_ficha_valida:

#################################################################################################################

.data

.end
#        END
