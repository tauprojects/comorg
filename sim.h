/*
 * asm.h
 *
 *  Created on: May 12, 2017
 *      Author: user
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// main memory
#define MEM_SIZE (1 << 16) //2048
#define NUM_REGS  16       //16 internal registers
#define LINE_SIZE (500)
#define BIT_SIZE (1 << 4)

//Error Messages Declarations
#define ERR_MSG_INVALID_ARGS_NUM "Invalid Number Of Arguments - Expected: 5 , Actual: "
#define INVALID_ARG "Invalid Argument"
#define ERR_MSG_OPEN_FILE "Error While Opening File: "


//Function Declarations
int sbs(int x, int msb, int lsb);
int sb(int x, int bit);
void printTrace();
void printRegout();
void instExec();
void gracfullyExit();
int logicalRightShift(int x, int n);
int arithmeticRightShift(int x, int n);
int logicalLeftShift(int x, int n);
void printMemout();


#endif /* MAIN_H_ */
