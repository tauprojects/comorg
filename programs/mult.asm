		add $sp, $zero, $imm, 2048 			# $sp = 2048
		lw $a0, $zero, $imm, 1024			# $a0 = Mem[1024] = x
		lw $a1, $zero, $imm, 1025			# $a1 = Mem[1025] = y
		add $t0, $zero, $zero, 0			# $t0 = 0 = result
		add $t1, $a1, $zero, 0				# $t1 = temp_y
		sra $t3, $a0, $imm, 31				# $t3 = LSB(x)
		sra $t4, $a`, $imm, 31				# $t4 = LSB(y)
		sub $t5, $t4, $t3, 0				# $t5 = LSB(x)-LSB(y) ($t5=0 if x, y have same sign,	 $t5=-1 if x>0, y<0,	$t5=1 if x<0, y>0)
		ble $imm, $t1, $zero, loop_minus	# jump to loop_minus if temp_y<=0
loop_plus:
		beq $imm, $t1, $zero, check_sign	# jump to check_sign if temp_y=0
		add $t0, $t0, $a0, 0				# result = result + x
		sub $t1, $t1, $imm, 1				# temp_y = temp_y - 1
		beq $imm, $zero, $zero, loop_plus	# back to beginning of loop_plus
loop_minus:
		beq $imm, $t1, $zero, check_sign	# jump to check_sign if temp_y=0
		add $t0, $t0, $a0, 0				# result = result + x
		add $t1, $t1, $imm, 1				# temp_y = temp_y + 1
		beq $imm, $zero, $zero, loop_minus	# back to beginning of loop_plus
check_sign:
		bgt $imm, $zero, $a1, negate		# jump to negate if y<0 (we need to reverse sign of res iff y<0)
finish:
		sw $t0, $zero, $imm, 1026			# Mem[1026]	 = res
		halt								# exit mult.asm
negate:
		sub $t0, $zero, $t0, 0 				# res = -res
		beq $imm, $zero, $zero, finish			# jump to finish
		
		

/*
*	based on note from forum:
*	אפשר להתחיל מרגיסטר מאופס, 
*	ולסכום את הנכפל עם עצמו מספר פעמים ללא הזזות. 
*	כמה פעמים צריך לסכום אותו?
*	פשוט מחסירים '1'
*	כל פעם מהכופל אם הוא חיובי,
*	או מוסיפים '1'
*	אם הוא שלילי
*	(עד שמגיעים לאפס).
*	כמובן שבכל מקרה צריך לוודא שהסימן מתאים. 
*	כלומר אם הסימנים הפוכים,
*	התוצאה שלילית,
*	ואם הם זהים, 
*	התוצאה חיובית.
*/
