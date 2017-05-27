/*
 Created By: Matan Gizunterman
 TAU Username: gizunterman
 ID number: 303157804
 Most of the code was with the help of StackOverflow Issues, and Tutorials Point C Programming Guide
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h> 
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// main memory
#define MEM_SIZE (1 << 16)
#define NUM_REGS  16


//Error Messages Declarations
#define ERR_MSG_INVALID_ARGS_NUM "Invalid Number Of Arguments - Expected: 6 , Actual: "
#define INVALID_ARG "Invalid Argument"
#define ERR_MSG_OPEN_FILE "Error While Opening File: "


// main memory
int mem[MEM_SIZE];
//Registers
int reg_list[NUM_REGS];

// opcode names
char op_name[][10] = { "add", "sub", "and", "or", "sll", "sra", "srl", "beq", "bgt", "ble", "bne", "jal", "lw", "sw", "lhi", "halt" };

// register names
char reg_name[][10] = { "$zero", "$imm", "$v0", "$a0", "$a1", "$t0", "$t1", "$t2", "$t3", "$s0", "$s1", "$s2", "$gp", "$sp", "$fp", "$ra" };


// files
FILE *fp_memin, *fp_memout,*fp_regout, *fp_trace,*fp_count;

//Function Declarations
int sbs(int x, int msb, int lsb);
int sb(int x, int bit);
void printTrace();
void printRegout();
void instExec();
void gracfullyExit();


void printInst(op, rd, rs, rt , imm){
	printf("op is: %d  %s\n",op, op_name[op]);
	printf("rd is: %d  %s\n",rd , reg_name[rd]);
	printf("rs is: %d  %s\n",rs,  reg_name[rs]);
	printf("rt is: %d  %s\n",rt, reg_name[rt]);
	printf("imm is: 0x%04x\n",imm);

}


//Main function
int main(int argc, char *argv[]) {
	//Variables declarations
	int i, last, cnt=0;
	int inst, op, rd, rs, rt, imm, PC;

	//Checking if number of arguments is valid
	if (argc < 6) {
		printf(ERR_MSG_INVALID_ARGS_NUM);
		printf("%d\n", argc - 1);
		exit(-1);
	} else if (argc > 6) {
		printf(ERR_MSG_INVALID_ARGS_NUM);
		printf("%d\n", argc - 1);
		exit(-1);
	}

	//Memory map input file path
	char* meminPath = argv[1];

	//Memory Map output file path
	char* memoutPath = argv[2];

	//Registers program output file path
	char* regoutPath = argv[3];

	//Trace output file path
	char* tracePath = argv[4];

	//Operations count file path
	char* countPath = argv[5];

	// opening files
	fp_memin = fopen(meminPath, "rt");
	fp_memout = fopen(memoutPath, "wt");
	fp_regout = fopen(regoutPath, "wt");
	fp_trace = fopen(tracePath, "wt");
	fp_count = fopen(countPath, "wt");
	if (!fp_memin || !fp_memout || !fp_regout ||!fp_trace ||!fp_count) {
		printf("ERROR: couldn't open files\n");
		exit(-1);
	}

	// zero memory
	memset(mem, 0, MEM_SIZE * sizeof(int));

	//Zero registers
	memset(reg_list, 0, NUM_REGS * sizeof(char));

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

			if (op == 0 && rd == 0){
				fprintf(fp_memout, "\t.word %d %d\n", PC, inst);

			}
			else {
				cnt++;
				printInst(op,rd,rs,rt,imm);
				instExec(op,rd,rs,rt,imm);
				printTrace(PC, inst);


			}

		}
		fprintf(fp_memout,"%08X\n", inst);
		PC++;
	}



	//Printing count.txt - the number of instructions
	fprintf(fp_count,"%d\n", cnt);


	 gracfullyExit();
	 return 0;

}
void instExec(int op, int rd, int rs, int rt, int imm, int PC) {
	unsigned int temp ;
	switch (op) {
	case 0:		 //$add
		reg_list[rd] = reg_list[rs] + reg_list[rt];
		break;

	case 1:		//$sub
		reg_list[rd] = reg_list[rs] - reg_list[rt];
		break;

	case 2:		//$and
		reg_list[rd] = reg_list[rs] & reg_list[rt];
		break;

	case 3:		//$or
		reg_list[rd] = reg_list[rs] | reg_list[rt];
		break;

	case 4:		//$sll
		reg_list[rd] = reg_list[rs] << reg_list[rt];
		break;

	case 5:		//$sra

		reg_list[rd] = reg_list[rs] >> reg_list[rt];
		reg_list[rd] = temp;
		break;

	case 6:		//$srl
		reg_list[rd] = reg_list[rs] >> reg_list[rt];
		break;

	case 7:		//$beq
		if(reg_list[rs] == reg_list[rt]){

			PC = reg_list[rd]; //if (R[rs] == R[rt]) pc = R[rd][low bits 15:0]
		}
		break;

	case 8:		//$bgt
		if(reg_list[rs] > reg_list[rt]){
			PC = reg_list[rd]; //if (R[rs] > R[rt]) pc = R[rd][low bits 15:0]
		}
		break;

	case 9:		//$ble
		if(reg_list[rs] <= reg_list[rt]){
			PC = reg_list[rd]; //if (R[rs] <= R[rt]) pc = R[rd] [low bits 15:0]
		}
		break;

	case 10:	//$bne
		if(reg_list[rs] != reg_list[rt]){
			PC = reg_list[rd]; //if (R[rs] != R[rt]) pc = R[rd] [low bits 15:0]
		}
		break;

	case 11:	//$jal
		reg_list[15] = PC + 1; //(next instruction address),
		//PC = reg_list[rd][15:0];
		break;

	case 12:	//$lw
		reg_list[rd] = mem[reg_list[rs]+reg_list[rt]];
		break;

	case 13:	//$sw
		mem[reg_list[rs]+reg_list[rt]] = reg_list[rd];
		break;

	case 14:	//$lhi
		//R[rd][bits 31:16] = R[rs][low bits 15:0]
		break;

	case 15:	//$halt
		gracfullyExit();
		exit(-1);
		break;
	}

}
void printTrace(int PC, int inst){
	fprintf(fp_trace, "%08x %08X ",PC ,inst);
	int i;
	for(i=0;i<NUM_REGS;i++){
		fprintf(fp_trace, "%08x " ,reg_list[i]);
	}
	fprintf(fp_trace, "\n");
}
void printRegout(){
	int i;
	for(i=2;i<NUM_REGS;i++){
		fprintf(fp_regout, "%08x\n" ,reg_list[i]);
	}
}


// extract single bit
int sb(int x, int bit)
{
	return (x >> bit) & 1;
}

// extract multiple bits
int sbs(int x, int msb, int lsb)
{
	if (msb == 31 && lsb == 0)
		return x;
	return (x >> lsb) & ((1 << (msb - lsb + 1)) - 1);
}


void gracfullyExit(){
	//Printing
	 printRegout();
	// close files
	fclose(fp_trace);
	fclose(fp_regout);
	fclose(fp_count);
	fclose(fp_memin);
	fclose(fp_memout);
	puts("Exited Gracefully");
}

