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
        STMFD   sp!, {r4}

# si ((f <= DIM) && (0 <= f) && (c <= DIM) && (0 <= c) && (tablero[f][c] != CASILLA_VACIA))
# Al ser evaluacion cortocircuitada salto despues de cada comparacion
        mov r4, #8 /*r4 = DIM*/
        cmp r4, r1 /*(f <= DIM)*/
        tstge r1, r1 /*(0 <= f)*/
        cmpge r4, r2 /*(c <= DIM)*/
        cmpge r2, r2 /*(0 <= c)*/
        blt ARM_ficha_validaElse
        mlage r4, r1, r4, r2 /*r6 la posicion del vector([f][c])*/
        ldrge r0, [r0, r4] /*r0 = tablero[f][c]*/
        teqge r0 , r0 /*(tablero[f][c] != CASILLA_VACIA)*/
ARM_ficha_validaIf:
        movne r4, #1 /* *posicion_valida = 1*/
        bne ARM_ficha_valida_return
ARM_ficha_validaElse:
        mov r0 , #0 /* *posicion_valida = 0*/
        mov r4, r0

ARM_ficha_valida_return:

        str r4, [r3] /* result = tablero[f][c] o 0*/
        # restore the original registers
        LDMFD   sp!, {r4}
        # return to the instruccion that called the rutine and to arm mode
        BX      r14

#################################################################################################################

.data

.end
#        END
