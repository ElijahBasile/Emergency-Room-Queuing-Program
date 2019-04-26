/*
 * main.c
 *
 *  Created on: Mar 15, 2019
 *      Author: Elijah Basile
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "interface.h"


//skeletal function for the code
int main() {
	srand(time(0));

	//required command that makes the scanf and printf work properly
	setvbuf(stdout,  NULL,  _IONBF,  0);

	//run the program
	menu();

	//exit without error
	return 0;
}
