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


//Error Messages Declarations
#define ERR_MSG_INVALID_ARGS_NUM "Invalid Number Of Arguments - Expected: 6 , Actual: "
#define INVALID_ARG "Invalid Argument"
#define ERR_MSG_OPEN_FILE "Error While Opening File: "

//Command String Declatraions
#define VAL1  "op1"
#define VAL2  "op2"
#define VAL3	"op3"

//Enum Message Operation Code Declarations
typedef enum USER_MAIN_CMD {
	op1, op2, op3, invalid
} USER_MAIN_CMD;

//Function Declarations
int parMainCmd(char* cmd);


//Main function
int main(int argc, char *argv[]) {

	//File descriptor of the input\output files
	int inputFd;

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

	//Optional Translator
//	cmd = parMainCmd(argv[1]);

}


//Parse the main command argument
int parMainCmd(char* cmd) {
	if (strcmp(cmd, VAL1) == 0) {
		return op1;
	} else if (strcmp(cmd, VAL2) == 0) {
		return op2;
	} else if (strcmp(cmd, VAL3) == 0) {
		return op3;
	} else {
		return invalid;
	}
}





