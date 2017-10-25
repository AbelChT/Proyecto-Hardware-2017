.text
#        ENTRY            		/*  mark the first instruction to call */
.global	start
.global ficha_valida_arm
.global ficha_valida_thumb
.global patron_volteo_arm
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

.extern test_patron_volteo_arm
.extern ficha_valida_a_usar /* En este puntero a funci�n se almacena el fichavalida que hay que usar*/
        ldr         r5, = test_patron_volteo_arm
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
.equ LOG2_DIM, 3 /* Debido a que DIM es potencia de 2 podemos evitar una multiplicaci�n */
.equ CASILLA_VACIA, 0

ficha_valida_arm:
# Al no usarse ningun registro adicional no hace falta salvar nada

# si ((f < DIM) && (0 <= f) && (c < DIM) && (0 <= c) && (tablero[f][c] != CASILLA_VACIA))

        cmp r1, #DIM  /* cmp f, DIM*/
        cmplo r2, #DIM  /* cmp c, DIM*/

#  Debido a que un numero negativo tiene el bit de mas peso a 1, si lo interpretamos
#  sin signo obtendremos un número muy alto (siempre mayor a #DIM).
#  Entonces nos podemos aprovechar de ello y comparar si
#  ambos son mayores a #DIM e interpretar la comparación sin signo.
#  Ademas en este caso, al pasarnos dos carácteres, la opción de comparar
#  con #0 para ver si son menores sería más costosa, debido a que antes se
#  tendría que extender el signo y ya despues proceder a la comparación.

        bhs		ficha_valida_arm_else /* salta si !(c < DIM) o !(f < DIM) o (c<0) o (f<0)*/

# Como #DIM es una potencia de 2, una multiplicacion por #DIM equivale a un desplazamiento de log2(#DIM) bits
        add r1, r2, r1, LSL #LOG2_DIM /*r4 = posicion del vector([f][c]) con respecto a tablero -> *tablero[f][c] = r4 + *tablero */
        ldrb r0, [r0, r1] /*r0 = tablero[f][c] para en el caso de que entre al if ya tener el resultado*/
        cmp r0 , #CASILLA_VACIA /* cmp tablero[f][c], CASILLA_VACIA */
        beq ficha_valida_arm_else /* entra si (tablero[f][c] != CASILLA_VACIA)*/

ficha_valida_arm_if:
        mov r1, #1 /*r1=1*/
        str r1, [r3] /* *posicion_valida = 1 */
        b ficha_valida_arm_return /* r0 sera igual a tablero[f][c] */

ficha_valida_arm_else:
        mov r0, #0 /* r0 sera igual a CASILLA_VACIA */
        str r0, [r3] /* *posicion_valida = 0 */

ficha_valida_arm_return:
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
#  saves the working registers
	str r4, [sp,#-4]! /*STMFD   sp!, {r4}*/
	ADR r4, ficha_valida_thumb_start + 1 /* Processor starts in ARM state, */
	BX r4 /* small ARM code header used to call Thumb main program. */
	# La restauracion del registro se hace en ficha_valida_return_thumb
	NOP

.thumb
ficha_valida_thumb_start:

# si ((f < DIM) && (0 <= f) && (c < DIM) && (0 <= c) && (tablero[f][c] != CASILLA_VACIA))

#  Debido a que un numero negativo tiene el bit de mas peso a 1, si lo interpretamos
#  sin signo obtendremos un número muy alto (siempre mayor a #DIM).
#  Entonces nos podemos aprovechar de ello y comparar si
#  ambos son mayores a #DIM e interpretar la comparación sin signo.
#  Ademas en este caso, al pasarnos dos carácteres, la opción de comparar
#  con #0 para ver si son menores sería más costosa, debido a que antes se
#  tendría que extender el signo y ya despues proceder a la comparación.

        cmp r1, #DIM  /* cmp f, DIM*/
        bhs ficha_valida_else_thumb

        cmp r2, #DIM  /* cmp c, DIM*/
        bhs ficha_valida_else_thumb

        lsl r1, r1, #LOG2_DIM
        add r1, r1, r2 /*r1 = posicion del vector([f][c]) con respecto a tablero -> *tablero[f][c] = r1 + *tablero */

        ldrb r0, [r0, r1] /*r0 = tablero[f][c] para en el caso de que entre al if ya tener el resultado*/
        cmp r0 , #CASILLA_VACIA /* cmp tablero[f][c], CASILLA_VACIA */
        beq ficha_valida_else_thumb /* entra si (tablero[f][c] != CASILLA_VACIA)*/

ficha_valida_if_thumb:
# *posicion_valida = 1;
        mov r1, #1 /*r1=1*/
        str r1, [r3]
        b ficha_valida_return_thumb

ficha_valida_else_thumb:
# *posicion_valida = 0;
        mov r0, #0 /* r0 sera igual a CASILLA_VACIA */
        str r0, [r3] /* *posicion_valida = 0 */

ficha_valida_return_thumb:
        # restore the original registers
        POP {r4} /* Se restaura el registro usado para saltar*/
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
        STMFD   sp!, {r4-r10 ,fp,lr}
        add fp, sp , #36

		# Se cargan bytes para respetar que son 3 caracteres
        ldrb r8,[fp],#4 /* LDMFD   fp!, {r4} */
        ldrb r9,[fp],#4 /* LDMFD   fp!, {r5} */
        ldrb r10,[fp],#4 /* LDMFD   fp!, {r6} */

# Estado del sistema
# r8 = char SF
# r9 = char SC
# r10 = char color
        add r6, r2, r8 /* FA = FA + SF */
        add r7, r3, r9 /* CA = CA + SC */
        mov r4, r0 /* Guardo tablero */
        mov r5, r1 /* Guardo longitud */

# Estado del sistema
# r4 = char tablero[][DIM]
# r5 = int *longitud
# r6 = char FA
# r7 = char CA

# LLamada a fichavalida
        and r1, r6, #0xff /* FA lo paso como segundo parametro, para respetar que es un byte se hace el cast */
        and r2, r7, #0xff /* CA lo paso como tercer parametro, para respetar que es un byte se hace el cast */
        sub sp, sp, #4 /* Reservo un entero en la pila para almacenar posicion_valida */
        mov r3, sp /* paso *posicion_valida como ultimo parametro */

		# Cargamos el ficha valida a usar en este caso
        ldr r12, = ficha_valida_a_usar /* Como ya fp no se necesita se puede usar r12 */
        ldr r12, [r12]
        mov lr, pc /* Guardo el pc */
        bx r12 /* Salto a la función */

        ldr r1,[sp],#4 /*Recupero posicion_valida */

# Estado del sistema
# r0 = casilla
# r1 = posicion_valida

# Fin de llamada a fichavalida
# Se comprueba si se entra en el if, el else if o el else
# si(posicion_valida == 1) && (casilla != color)
# sino si ((posicion_valida == 1) && (casilla == color))
# sino
        cmp  r1, #1 /*cmp posicion_valida, #1*/
        bne patron_volteo_arm_else /*si posicion valida es distinto de 1 siempre entra al else*/
        cmp r0, r10 /*cmp casilla, color*/
        /* En este punto posicion_valida == 1 , entonces si casilla == color
         * entrata al else if y sino entrara al if  */
        beq patron_volteo_arm_else_if

patron_volteo_arm_if:
        ldr r1, [r5] /* r1 = longitud */
        add r1, r1, #1
        str r1, [r5] /* longitud = longitud + 1 */

# LLamada a patron_volteo
# Paso de parámetros patron_volteo por registro
        mov r0, r4 /* Guardo tablero */
        mov r1, r5 /* Paso la direccion de longitud como segundo parametro */
        and r2, r6, #0xff /* FA lo paso como tercer parametro, para respetar que es un byte se hace el cast */
        and r3, r7, #0xff /* CA lo paso como cuarto parametro, para respetar que es un byte se hace el cast */

# No hace falta guardar los registros del 0 al 3 ya que no se van a usar

# Paso los parametros correspondientes por la pila
        strb r10, [sp,#-4]! /*STMFD   sp!, {r4}*/
        strb r9, [sp,#-4]! /*STMFD   sp!, {r4}*/
        strb r8, [sp,#-4]! /*STMFD   sp!, {r4}*/

        ldr r12, = patron_volteo_arm /* Preparo la direccion a saltar */
        mov lr, pc /* Guardo pc */
        bx r12

# Retorno patron volteo
        add sp, sp ,#12 /*Elimino los parametros de la pila*/

# Fin llamada a patron_volteo
# En r0 se almacena el resultado de la última invocación
        b patron_volteo_return

patron_volteo_arm_else_if:
        ldr r2, [r5] /* r2 = longitud */
        cmp r2 , #0 /* cmp longitud, #0*/
        movgt r0, #PATRON_ENCONTRADO /* si la longitud es mayor a 0 se ha encontrado */
        movle r0, #NO_HAY_PATRON /* si la longitud es menor o igual a 0 no */
        b patron_volteo_return

patron_volteo_arm_else:
        mov r0, #NO_HAY_PATRON /* no se ha encontrado patron */

# restore the original registers
# return to the instruccion that called the rutine and to arm mode
patron_volteo_return:
        LDMFD   sp!, {r4-r10,fp,pc} /* Se carga directamente pc para evitar una instruccion mas */

#################################################################################################################

.data

.end
#        END
