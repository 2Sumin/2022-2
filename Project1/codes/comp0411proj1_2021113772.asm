# ------------------------------------------------------------------------------------
# [KNU COMP0411 Computer Architecture] Skeleton code for the 1st project (calculator)
# ------------------------------------------------------------------------------------


.data
.align 2
get_input: .asciz "> " # print b/f getting input
input_string: .space 100 # saving input
enter_string: .string "\n" # printing b/f next input


.text	
# main
main:
	# a/f executing "test" function, go back to x1
	jal x1, test #functionality test, Do not modify!!
	
	#----TODO-------------------------------------------------------------
	#1. read a string from the console
	#2. perform arithmetic operations
	#3. print a string to the console to show the computation result
	#----------------------------------------------------------------------

	# li instructin: li [destination_register], [expression] (load immediate)
	# a0: address of input buffer
	# a1: max num of characters to read
	# a7: call num. readstring(8)
	
	# if you want to break the input_loop, just input enter
	input_loop:
	# initialize
	li a5, 0
	li a6, 0
	li t6, -1 # for beraking loop ( when operator is not in the input )
	
	# 1-1) Show prompt in the console. - print string (4)
	la a0, get_input # print ">"
	li a7, 4
	ecall	
	
	# 1-2) Type an equation into the console. - get string (8)
	la a0, input_string
	li a1, 100
	li a7, 8
	ecall
	la t0, input_string # load input_string's address into t0
	
	# 1-3) assign operation's ascii value into s1~s4 
	li s1, 43 # 43: + (add)
	li s2, 45 # 45 : - (sub)
	li s3, 42 # 42 : * (mul)
	li s4, 47 # 47 : / (div)
	
	# 1-4) convert string into ascii code, and find operation's index
	li t1, 0 # i(t1) = 0
	
	do:
	add t4, t1, t0 # add i(t1) to base address(t0) of input_string
	lb t4, (t4) # load a char : ith element of input_string

	beq t4, s1, goto_add
	beq t4, s2, goto_sub
	beq t4, s3, goto_mul
	beq t4, s4, goto_div
	
	beqz t4, goto_exit # if last char in input_string, break do loop
	
	addi t1, t1, 1 # i++
	beq zero, zero, do 
	
	
	# index of the operation is in t2 & 
	# save the operation code into register a1
	goto_add: #a0 = 0
	addi t2, t1, 0 # save index of operator in t2
	addi t6, zero, 0 # save operator's number (0) in a1 register for calc
	addi t1, t1, 1 # i++
	beq zero, zero, do
	
	goto_sub: #a0 = 1
	addi t2, t1, 0 # save index of operator in t2
	addi t6, zero, 1 # save operator's number (1) in a1 register for calc
	addi t1, t1, 1 # i++
	beq zero, zero, do
	
	goto_mul: #a0 = 2
	addi t2, t1, 0 # save index of operator in t2
	addi t6, zero, 2 # save operator's number (2) in a1 register for calc
	addi t1, t1, 1 # i++
	beq zero, zero, do
	
	goto_div: #a0 = 3
	addi t2, t1, 0 # save index of operator in t2
	addi t6, zero, 3 # save operator's number (3) in t6 register for calc
	addi t1, t1, 1 # i++
	beq zero, zero, do
	
	goto_exit: # end of the string , last index is saved in t1
	# if no input, then exit the input loop
	li t3, -1
	beq t6, t3, input_loop_done
	# else, save the last index
	addi t1, t1, -2
	
	# 1-5) assign first operand in (a6), and second operand in (a5)
	
	###########################################################
	# t0 : address of input_string
	# t1 : index of last string
	# t2 : index of operator
	# t4 : char of input_string[i]
	# t5 : address of input_string[i] 
	# t6 : number of operator ( 0 / 1 / 2 / 3 )
	# a6 : tmp reg for a2
	# a5 : tmp reg for a3
	# -------------- from now on -------------
	# s0 : index i
	# s2 : index j (for ending loop if j == t2 )
	# -- to check if it is number or not, use s1, s3
	# if the char is between 0 and 9 -> it is a number,
	# else, it is not a number !!
	# s1 : for ascii ( number 0 )
	# s3 : for ascii ( number 9 )
	###########################################################
	
	# -----------------------put 1st operand into a6 
	# checking index
	li s0, 0
	li s2, 0
	# checking number in ascii (number?)
	li s1, 48
	li s3, 58
	# checking limit (10) for loop (multiply 10)
	li a3, 10
	
	get_first_operand:
	# load char into t4
	add t5, t0, s0		
	lb t4, 0(t5)	
		
	# if ( j == index of op ) goto second operand (1st operand done)
	beq s2, t2, get_second_operand
	# else j++
	addi s2, s2, 1
	
	# convert input_string[i] into ascii code number (input_string[i]- 48 ('0') )
	# t4 = t4 - s1
	not s4, s1
	addi s4, s4, 1 	
	add t4, t4, s4
	
	# initialize -> for (int j(s6) = 0 ; j < 10 (a3) ; j++)
	li s6, 0 # j
	li a2, 0 # save sum 				
	
	mul_ten1:
	# First, multiply 10 to the sum before (a2 = a6 * 10)
	# multiplying 10 is same as adding 10 times 
	add a2, a2, a6 # a6 : original sum, final a2 : a6 * 10
	addi s6, s6, 1	# j++ (for counting 10 times)
	# if ( it is added less than 10 times ), continue until adding 10 times.
	blt s6, a3, mul_ten1
	
	# Second, add t4 as a last digit	
	add a6, a2, t4		
	
	# i++ (for next string idx)
	addi s0, s0, 1	
	beq zero, zero, get_first_operand
	
	## -----------------------put 2nd operand into a5  
	
	# initialize 
	li a3, 10
	
	get_second_operand:
	# i++ 
	# (for reading next digit. place here b/c need to start from next idx of operator)
	addi s0, s0, 1	
		
	# load char 
	add t5, s0, t0		
	lb t4, 0(t5)	
		
	# if ( input_string[i] is not number ), second operand is done. now start calc.
	blt t4, s1, st_calc	
	bge t4, s3, st_calc	
	
	# convert input_string[i] into ascii code number (input_string[i]- 48 ('0') )
	# t4 = t4 - s1
	not s4, s1
	addi s4, s4, 1 	
	add t4, t4, s4
	
	# initialize
	li a2, 0		
	li s6, 0		
	
	mul_ten2:
	# First, multiply 10 to the sum before (a2 = a5 * 10)
	# multiplying 10 is same as adding 10 times 
	add a2, a2, a5
	addi s6, s6, 1	# j++ (for counting 10 times)
	# if ( it is added less than 10 times ), continue until adding 10 times.
	blt s6, a3, mul_ten2	
	# Second, add t4 as a last digit		
	add a5, a2, t4		
	
	beq zero, zero, get_second_operand
	
	#------------------------------------------------------------------------------------------------------
	# ready for calc function
	st_calc:
	addi a1, t6, 0
	addi a2, a6, 0
	addi a3, a5, 0
	
	# 2. perform arithmetic operations
	jal ra, calc
		
	# 3. print a string to the console to show the computation result
	
	addi t1, a0, 0 # save operation result (a0) into t1
	
	# print 1st operand
	li a7, 1
	addi a0, a2, 0 # print a2 (first operand)
	ecall
	
	# print operator
	addi t0, zero, 0 # t0 = 0
	bne a1, t0, sub_op2
	li a7, 11
	li a0, 43 # print "+"
	ecall
	beq zero, zero, exitt
	
	sub_op2:
	addi t0, t0, 1 # t0 = 1
	bne a1, t0, mul_op2
	li a7, 11
	li a0, 45 # print "-"
	ecall
	beq zero, zero, exitt
	
	mul_op2:
	addi t0, t0, 1 # t0 = 2
	bne a1, t0, div_op2
	li a7, 11
	li a0, 42 # print "*"
	ecall
	beq zero, zero, exitt
	
	div_op2:
	li a7, 11
	li a0, 47 # print "/"
	ecall
	
	exitt:
	
	# print 2nd operand
	li a7, 1
	addi a0, a3, 0	 # print a3 (second operand)	
	ecall
	
	# print "="
	li a7, 11
	li a0, 61
	ecall
	
	# print result
	li a7, 1
	addi a0, t1, 0	# print t1 (original a0, result)	
	ecall
	
	# if division, need to show remainder
	addi a5, zero, 3
	bne a1, a5, all_done
	
	# print ","
	li a7, 11
	li a0, 44
	ecall
	
	# print remainder
	li a7, 1
	addi a0, a4, 0	# print a4 (original a0, result)	
	ecall
	
	all_done:
	
	# print enter
	la a0, enter_string
	li a7, 4
	ecall
	
	beq zero, zero, input_loop
	
	input_loop_done:	
	# Exit (93) with code 0
        li a0, 0
        li a7, 93
        ecall
        ebreak


#----------------------------------
#name: calc
#func: performs arithmetic operation
#x11(a1): arithmetic operation (0: addition, 1:  subtraction, 2:  multiplication, 3: division)
#x12(a2): the first operand
#x13(a3): the second operand
#x10(a0): return value
#x14(a4): return value (remainder for division operation)
#----------------------------------
calc:
	#TODO	
	# perform arithmetic operations
	
	# get register
	addi sp, sp, -56			#adjust stack pointer
	sw t0, 0(sp)				#push register values to stack
	sw t1, 8(sp)
	sw t2, 16(sp)
	sw t3, 24(sp)
	sw t4, 32(sp)
	sw t5, 40(sp)
	sw t6, 48(sp)
	
	# initialize 
	li t0, 0
	li t1, 0
	li t2, 0
	li t3, 0
	li t4, 0
	li t5, 0
	li t6, 0

	# [detect operator -> goto sub]
	addi t0, zero, 0 # switch 
	bne a1, t0, sub_op
	
	# perform operation
	# 1) add
	add_op:
	add a0, a2, a3
	beq zero, zero, done
	
	# 2) sub
	sub_op:
	# [detect operator -> goto mul]
	addi t0, zero, 1
	bne a1, t0, mul_op
	
	#get 2's complement
	not t3, a3 # change 0 <-> 1
	addi t3, t3, 1 #t3 = -a3
	add a0, a2, t3
	
	beq zero, zero, done
	
	# 3) mul
	mul_op:
	# [detect operator -> goto div]
	addi t0, zero, 2
	bne a1, t0, div_op
	
	addi t6, a3, 0 # t6 = a3
	addi t0, a2, 0 # t0 = a2
	
	li t4, 32 # t4 = 32 -> for loop limit ( i < 32 )
	li t5, 0 # t5 = 0 -> for loop counter (i++)
	li t3, 1 # t3 = 00000001  - for masking
	li s7, 0 # for saving result
	
	Loop: 
	# for (int i = 0; i < 32; i++)
	beq t5, t4, Loop_exit
	addi t5, t5, 1 # t5++
	
	# if (t6's lsb ==1_odd) add, else test_lsb(just shift) -> odd or even
	and t1, t3, t6 # mask (only save last bit)
	beq t1, zero, test_lsb # (if lsb == 0) -> don't add, just shift
	# if (lsb == 1) -> add, and then shift
	add s7, s7, t0
	
	test_lsb:
	srli t6, t6, 1
	slli t0, t0, 1
	beq zero, zero, Loop # goto Loop
	
	Loop_exit:
	addi a0, s7, 0 
	beq zero, zero, done
	
	# 4) div
	div_op:
	li t4, 17 # t4 = 17 -> for loop limit ( i < 17 )
	li t5, 0 # t5 = 0 -> for loop counter (i++)
	add t6, t6, a2 # t6 = a2 (for saing result)
	slli t0, a3, 16 # for adding multiplier in Loop (add t0 every loop)
	
	# put -a3 into t0
	not t1, t0
	addi t0, t1, 1
	
	Loop2: 
	# for (int i=0; i<17; i++)
	beq t5, t4, Loop2_exit
	addi t5, t5, 1 # t5++
	
	add t2, t6, t0 # t2 = t6 + t0
	# if (t2's msb ==1) copy t2 into t6, else test_lsb(just shift) -> plus or minus 
	blt t2, zero, test_lsb2 # if t2 < 0 -> don't copy, just shift
	
	# if (msb == 1) -> copy, and then shift
	addi t6, t2, 0
	slli t6, t6, 1
	addi t6, t6, 1
	beq zero, zero, Loop2
	
	# if msb == 0 just shift
	test_lsb2:
	slli t6, t6, 1
	beq zero, zero, Loop2 # goto Loop
	
	Loop2_exit:
	# shift right the left of the remainder
	
	# remainder (a4)
	addi t4, t6, 0
	srli a4, t4, 17
	
	# quotient (a0)
	li t5, 0
	slli a0, t6, 16
	srli t5, a0, 16
	addi a0, t5, 0
	
	# go back to x1 (b/f calling function calc)
	done:
	lw t0, 0(sp)				#pop register values from stack
	lw t1, 8(sp)
	lw t2, 16(sp)
	lw t3, 24(sp)
	lw t4, 32(sp)
	lw t5, 40(sp)
	lw t6, 48(sp)
	addi sp, sp, 56	
	jalr x0, 0(x1)

.include "common.asm"