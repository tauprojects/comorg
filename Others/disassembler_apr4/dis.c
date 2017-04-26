#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "project.h"

// main memory
int mem[MEM_SIZE];

// files
FILE *fp_memin, *fp_asm;

// opcode names
char op_name[][10] = { "add", "sub", "and", "or", "sll", "sra", "srl", "beq", "bgt", "ble", "bne", "jal", "lw", "sw", "lhi", "halt" };

// register names
char reg_name[][10] = { "$zero", "$imm", "$v0", "$a0", "$a1", "$t0", "$t1", "$t2", "$t3", "$s0", "$s1", "$s2", "$gp", "$sp", "$fp", "$ra" };

int main(int argc, char *argv[])
{
	int i, last;
	int inst, op, rd, rs, rt, imm, PC;

	// check that we have 2 cmd line parameters
	if (argc != 3) {
		printf("usage: dis memin.txt program.asm\n");
		exit(1);
	}


	// open files
	fp_memin = fopen(argv[1], "rt");
	fp_asm = fopen(argv[2], "wt");
	if (!fp_memin || !fp_asm) {
		printf("ERROR: couldn't open files\n");
		exit(1);
	}

	// zero memory
	memset(mem, 0, MEM_SIZE * sizeof(int));

	// read memin.txt into mem
	i = 0;
	while (!feof(fp_memin)) {
		if (fscanf(fp_memin, "%08X\n", &mem[i]) != 1)
			break;
		i++;
	}

	// find last non-zero memory entry
	last = MEM_SIZE - 1;
	while (last >= 0 && mem[last] == 0)
		last--;

	// decode instructions
	PC = 0;
	while (PC <= last) {
		// fetch next instruction
		inst = mem[PC];

		// decode instruction
		op = sbs(inst, 31, 28);
		rd = sbs(inst, 27, 24);
		rs = sbs(inst, 23, 20);
		rt = sbs(inst, 19, 16);
		imm = sbs(inst, 15, 0);

		// print non zero contents
		if (inst != 0) {
			if (op == 0 && rd == 0)
				fprintf(fp_asm, "\t.word %d %d\n", PC, inst);
			else
				fprintf(fp_asm, "\t%-4s %-5s, %-5s, %-5s, 0x%04x\t# %04x: %08X\n", op_name[op], reg_name[rd], reg_name[rs], reg_name[rt], imm, PC, inst);
		}
		PC++;
	}

	// close files
	fclose(fp_memin);
	fclose(fp_asm);
	
	return 0;
}
