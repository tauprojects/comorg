		add $sp, $zero, $imm, 2048				# $sp = 2048
		add $t0, $zero, $imm, 15				# $t0 = 15 = i (outerloop)
		add $t1, $zero, $imm, 0					# $t1 = 0 = j (innerloop_begin)
		add $t5, $zero, Simm, 16				# $t5 = 16 ($t5 will be used to check whether j had reached 16, meaning we ALREADY HAD 15 iterations of the inner loop)
outerloop:
		beq $imm, $t0, $zero, finish			# if i = 0 than go to finish (i will loop from 15...1, a total of 15 iterations, as needed in bubblesort)
		sub $t0, $to, $imm, 1					# i = i-1
innerloop_begin:
		lw $t2, $imm, $t1, 1024					# $t2 = Mem[1024+j] = A[j]
		lw $t3, $imm, $t1, 1025					# $t3 = Mem[1024+j+1] = A[j+1]
		bgt $imm, $t2, $t3, swap&store			# if A[j]>A[j+1] jump to swap&store, else just increase j and go back to innerloop_begin (no need to store items, they were already stored)
innerloop_end:
		add $t1, $t1, $imm, 1					# j = j+1
		beq $imm, $t1, $t5, outerloop			# if j=16, jump back to outerloop
		beq $imm, $zero, $zero, innerloop_begin	# else we need to continue to another iteration of the innerloop_begin
swap&store: 									# swap&store will only be accessed if A[j]>A[j+1]
		add $t4, $t2, $zero, 0					# temp = A[j]
		add $t2, $t3, $zero, 0					# A[j] = A[j+1]
		add $t3, $t4, $zero, 0					# A[j+1] = temp
		sw $t2, $imm, $t1, 1024					# Mem[1024+j] = A[j]
		sw $t3, $imm, $t1, 1025					# Mem[1024+j+1] = A[j+1]
		beq $imm, $zero, $zero, innerloop_end	#go back to innerloop_end
finish:
		halt									
