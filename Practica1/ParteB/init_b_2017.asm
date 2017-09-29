.text
#        ENTRY            		/*  mark the first instruction to call */
.global	start
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

.extern     reversi8
        ldr         r5, = reversi8
        mov         lr, pc
        bx          r5


stop:
 		B     	stop    	/*  end of program */


################################################################################
# Función ARM ficha_valida:
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
        # Recordad que puede modificar r0, r1, r2 y r3 sin guardarlos previamente
        STMFD   sp!, {r4-r11}

        # Poned el código aquí:
        # r4 = (f <= DIM) && (0 <= f) && (c <= DIM) && (0 <= c) && (tablero[f][c] != CASILLA_VACIA)
        mov r5, #define DIM
        mov r4 , #0
        cmp r1, r5
        cmple r4, r1
        cmple r2, r5
        cmple r4, r2
        bgt ARM_ficha_valida_else
        mla r6,r1, r5, r2 # obtengo en r6 la posicion del vector (sin multiplicar por 4)
        # Obtener r6 = tablero[f][c]

        cmp r7 , r4 # (tablero[f][c] != CASILLA_VACIA) revisar
ARM_ficha_valida_inside_if:
ARM_ficha_valida_inside_else:
        # Si la operacion anterior es 0 entonces
        #*posicion_valida = 1;
        #ficha = tablero[f][c];

        # Si no lo es entonces
        #*posicion_valida = 0;
        #ficha = CASILLA_VACIA;

        # restore the original registers
        LDMFD   sp!, {r4-r11}
        # return to the instruccion that called the rutine and to arm mode
        BX      r14

################################################################################
# Función ARM:
# copia 10 palabras de la dirección indicada en r0 a la indicada por r1
# debería crear un marco de pila con la misma estructura que en el resto de llamadas a funciones
# pero como es un nodo hoja (no llama a ninguna función) vamos a hacer un marco simplificado:
# sólo guardamos los registros que utiliza y que no tiene permiso para alterar
################################################################################
ARM_copy_10:
        #  saves the working registers
        # Recordad que puede modificar r0, r1, r2 y r3 sin guardarlos previamente
        STMFD   sp!, {r4-r11}

        # Poned el código aquí: sólo hacen falta dos instrucciones

        # restore the original registers
        LDMFD   sp!, {r4-r11}
        # return to the instruccion that called the rutine and to arm mode
        BX      r14


#################################################################################################################
.data



.end
#        END
