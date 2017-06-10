/*
 * asm.c
 *
 *  Created on: May 17, 2017
 *      Author: user
 */

// run configurations: "test.txt" "D:\C Software Project\fib.asm"

#include "asm.h"

/*
 * All labels in the .asm will be added as nodes to a linked list. Each node will contain
 * the label itself, and its original address (line number) in the .asm file.
 */
typedef struct node {
	char* label;
	int address;
	struct node *next;
} node;

struct node *head = NULL;

FILE *fp_asm;
FILE *fp_memin;
FILE *fp_test_mem;

char op_name[][10] = { "add", "sub", "and", "or", "sll", "sra", "srl", "beq", "bgt", "ble", "bne", "jal", "lw", "sw", "lhi", "halt" }; // all of the names of the instructions.

char reg_name[][10] = { "$zero", "$imm", "$v0", "$a0", "$a1", "$t0", "$t1", "$t2", "$t3", "$s0", "$s1", "$s2", "$gp", "$sp", "$fp", "$ra" }; // names of registers.


/*
 * removespaces takes a string as paramater, and strips it of spaces (if there are any).
 */
void removespaces(char* source)
{
	char *write = source, *read = source;
	do {
	   // Skip space and tab if we haven't yet
	   if (write != source || (*read != ' ' && *read != '\t')) {
	       *(write++) = *read;
	   }
	} while (*(read++));
}

/*
 * func returns 1 iff the line is empty
 */
int check_empty(char *line){
  while (*line != '\0') {
	if (!isspace(*line))
	  return 0;
	line++;
  }
  return 1;
}


/*
 * clean_and_convert_to_lowercase converts its string parameter into lowercase string.
 */
void clean_and_convert_to_lowercase(char *word){
	int i;
	for(i=0; word[i]; i++){
		word[i] = tolower(word[i]);
	}
	removespaces(word);
}

node* create_node(char *label, int address){
	struct node *new_node = (struct node*) malloc(sizeof(struct node));
	new_node->label = malloc(strlen(label)+1);
	strcpy(new_node->label, label);
	new_node->address = address;
	new_node->next = NULL;
	return new_node;
}

/*
 * insert each label node into the label list.
 */
void insert_to_label_list(node *new_node) {
	new_node->next = head;
	head = new_node;
}


/*
 * create an array of strings of length MEM_SIZE, and put '00000000' in each cell.
 */
void init_mem_array(char** mem_array){
	int i;
	for (i=0; i<MEM_SIZE; i++){
		mem_array[i] = (char*)malloc(9*sizeof(char));
		strcpy(mem_array[i], "00000000");
	}
}

/*
 * func goes over the file, line by line.
 * if it finds a label, it stores it as a node in a linked list
 * (at the beginning the linked list is empty).
 */
void create_label_list (FILE* asm_file){ //stores all labels and their addresses in a linked list
	char *label;
	int address = 0;
	char line[500];
	//int offset = 0;
	while(1){
		fgets(line, LINE_SIZE, asm_file);
		if(check_empty(line)==0){ //meaning this is not an empty line
			break;
		}
	}
	//fgets(line, LINE_SIZE, asm_file);
	do{
		if(check_empty(line)==1){ //meaning this is an empty line
			continue;
		}
		char* label_end = strstr(line, ":");
		if (label_end){ // if the sentence contains ':', label_end is assigned a pointer to that place, else NULL.
			label = strtok(line, ":"); // save only the beginning of the string, up until the ":"
			clean_and_convert_to_lowercase(label);
			insert_to_label_list(create_node(label, address));
			//insert_to_label_list(create_node(label, address-offset));
			//offset++;
			address--;
			}
		address++;
	} while (fgets(line, LINE_SIZE, asm_file));
	fseek(asm_file, 0, SEEK_SET); //reset pointer to the beginning of file, for second iteration of file.
}

/*
 * search for a node in the label list, according to the label. if label is found,
 * return value is its address.
 */
int search_label_list(char *imm){
   struct node* current = head;
   while (current != NULL){
	   if(strcmp((current->label), imm)==0){
		   return current->address;
	   }
	   current = current->next;
   }
   return NULL;
}

/*
 * function takes the first word in each .asm line (the operator),
 * and converts it to its proper HEX value, which will be the return value of this function.
 */
char* process_first_word (char *operation){
	char *first = malloc(5);
	int op_num, i;
	clean_and_convert_to_lowercase(operation);
	for(i=0; i<BIT_SIZE; i++){
		if(strcmp(operation, op_name[i])==0){
			op_num = i;
		}
	}
	sprintf(first, "%01X", op_num);
	return first;
}

/*
 * function takes the second, third or fourth word in each .asm line (the registers),
 * and converts it to its proper HEX value, which will be the return value of this function.
 */
char* process_registers (char *reg){
	char *second = malloc(5);
	int reg_num, i;
	clean_and_convert_to_lowercase(reg);
	for(i=0; i<BIT_SIZE; i++){
		if(!strcmp(reg, reg_name[i])){
			reg_num = i;
		}
	}
	sprintf(second, "%01X", reg_num);
	return second;
}

/*
 * takes in a string and convert it into its decimal value.
 */
int convert_to_decimal(char *word){
	int result;
	if ((word[0] == '0') && (word[1] == 'x')){ // imm is written in hex
		result = strtol(&word[2], NULL, 16); //convert
	}
	else{
		result = strtol(&word[0], NULL, 10);
	}
	return result;
}

/*
 * function takes the fifth in each .asm line (the imm),
 * and converts it to its proper HEX value, which will be the return value of this function.
 */
char *process_imm (char *imm){
	char *fifth = malloc(8);
	int imm_num = 0;
	clean_and_convert_to_lowercase(imm);
	char first_letter = imm[0];
	if(first_letter>='a' && first_letter<='z'){ //meaning it's a label, not a number
		imm_num = search_label_list(imm);
	}
	else{ //imm represents a number, not a label;
		imm_num = convert_to_decimal(imm);
	}
	if (imm_num<0){
		sprintf(fifth, "%hX", imm_num);
	}
	else{
		sprintf(fifth, "%04X", imm_num);
	}
	return fifth;
}

/*
 * this function processes the .word operations.
 */
void process_dot_word(char *address, char *data, char** mem_array){
	int int_address = convert_to_decimal(address); //address is now decimal.
	int int_data = convert_to_decimal(data); // data is now int, now we need to extend it to 32 bit.
	char *final_data = malloc(8);
	sprintf(final_data, "%08X", int_data); //final_data now stores a 32 bit hex.
	strcpy(mem_array[int_address], final_data);
}


/*
 * this function takes a .asm line, converts it into
 * the proper HEX string and stores it in the mem_array.
 */
int process_single_asm_line_to_array(char *asm_line, int line_number, char** mem_array){
	char *first, *second, *third, *fourth, *fifth;
	if(check_empty(asm_line)==1){
		return line_number;
	}
//	if(strlen(asm_line)<5){
//		return line_number-1;
//	}
	first = strtok(asm_line, " ");
	second = strtok(NULL, "\t ,\t ");
	third = strtok(NULL, "\t ,\t ");
	fourth = strtok(NULL, "\t ,\t ");
	fifth = strtok(NULL, "\t #\t ");

	char* word = strstr(first, ":"); // here we check if the line represents the label, so we should ignore it. .
	if (word){ // this means this is a label, no need to process into the mem_in file.
		return line_number;
	}
	word = strstr(first, ".word");
	if (word){
		process_dot_word(second, third, mem_array);
		return line_number;
	}
	char *op = process_first_word(first);
	char *rd = process_registers(second);
	char *rs = process_registers(third);
	char *rt = process_registers(fourth);
	char *imm = process_imm(fifth);
	sprintf(mem_array[line_number],"%s%s%s%s%s", op, rd, rs, rt, imm);
	line_number++;
	return line_number;
}

/*
 * function takes each line of the asm file, translates it into proper 32bit word and updates the mem_in array.
 */
void iterate_asm_program(FILE *filein, char** mem_array){
	char asm_line[LINE_SIZE]; //line will conatain one line from the .asm file every loop.
	fgets(asm_line, LINE_SIZE, filein);
	int line_number = 0;
	line_number = process_single_asm_line_to_array(asm_line, 0, mem_array);
//	if (line_number<0){
//		line_number = 0;
//	}
//	else{
//		line_number = 1;
//	}
	while (fgets(asm_line, LINE_SIZE, filein)) {
		line_number = process_single_asm_line_to_array(asm_line, line_number, mem_array);
//		line_number++;
	}
}

/*
 * all lines of mem_array will be written into the memin file.
 * first we iterate the mem_array from the end, looking for the first
 * non zero line. this line will then be the last to be written
 * into the memin file.
 */
void write_array_of_lines_to_file(FILE *memin, char** mem_array){
	int i=0;
	int final_line=0;
	for(i=MEM_SIZE-1; i>=0; i--){ //this loop counts total number of lines in file
		if(strcmp(mem_array[i], "00000000")!=0){
			final_line = i;
			break;
		}
	}
	for(i=0; i<final_line+1; ++i){
		fprintf(memin, "%s\n", mem_array[i]);
	}
}

int main(int argc, char *argv[])
{
//	if (argc != 3) {
//		printf("usage: dis memin.txt program.asm\n");
//		exit(1);
//	}
	char **memin_array = (char**)malloc(MEM_SIZE*sizeof(char*));
	fp_memin = fopen(argv[1], "wt"); //memin file is the file to write to.
	fp_asm = fopen(argv[2], "rt"); // asm file is where the assembly program can be found.

	if (!fp_memin) {
//		printf(ERR_MSG_OPEN_FILE);
		puts(argv[1]);
		exit(-1);
	}

	if (!fp_asm) {
//		printf(ERR_MSG_OPEN_FILE);
		puts(argv[2]);
		exit(-1);
	}
	init_mem_array(memin_array); // initiate memin file with 2^16 lines of zeroes.
	create_label_list(fp_asm); //that's a linked list containing all labels and their addresses.
	iterate_asm_program(fp_asm, memin_array); //convert the .asm file into the memin file.
	write_array_of_lines_to_file(fp_memin, memin_array);
	fclose(fp_memin);
	fclose(fp_asm);

}
