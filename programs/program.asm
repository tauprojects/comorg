	add  $sp  , $zero, $imm , 0x0800	# 0000: 0D010800
	lw   $a0  , $zero, $imm , 0x0400	# 0001: C3010400
	jal  $imm , $zero, $zero, 0x0005	# 0002: B1000005
	sw   $v0  , $zero, $imm , 0x0401	# 0003: D2010401
	halt $zero, $zero, $zero, 0x0000	# 0004: F0000000
	add  $sp  , $sp  , $imm , 0xfffd	# 0005: 0DD1FFFD
	sw   $s0  , $sp  , $imm , 0x0002	# 0006: D9D10002
	sw   $ra  , $sp  , $imm , 0x0001	# 0007: DFD10001
	sw   $a0  , $sp  , $imm , 0x0000	# 0008: D3D10000
	add  $t0  , $zero, $imm , 0x0001	# 0009: 05010001
	bgt  $imm , $a0  , $t0  , 0x000e	# 000a: 8135000E
	add  $v0  , $a0  , $zero, 0x0000	# 000b: 02300000
	add  $sp  , $sp  , $imm , 0x0003	# 000c: 0DD10003
	beq  $ra  , $zero, $zero, 0x0000	# 000d: 7F000000
	sub  $a0  , $a0  , $imm , 0x0001	# 000e: 13310001
	jal  $imm , $zero, $zero, 0x0005	# 000f: B1000005
	add  $s0  , $v0  , $imm , 0x0000	# 0010: 09210000
	lw   $a0  , $sp  , $imm , 0x0000	# 0011: C3D10000
	sub  $a0  , $a0  , $imm , 0x0002	# 0012: 13310002
	jal  $imm , $zero, $zero, 0x0005	# 0013: B1000005
	add  $v0  , $v0  , $s0  , 0x0000	# 0014: 02290000
	lw   $a0  , $sp  , $imm , 0x0000	# 0015: C3D10000
	lw   $ra  , $sp  , $imm , 0x0001	# 0016: CFD10001
	lw   $s0  , $sp  , $imm , 0x0002	# 0017: C9D10002
	add  $sp  , $sp  , $imm , 0x0003	# 0018: 0DD10003
	beq  $ra  , $zero, $zero, 0x0000	# 0019: 7F000000
	.word 1024 7
