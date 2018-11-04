/*
Authors - Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name - Ex2
Description - This program impelments an application testing environment
*/

#include "Test.h"

#define _CRT_SECURE_NO_WARNINGS
#define ERROR_MESSAGE "An error occurred during execution, couldn't complete the task!\n"

void main(int argc, char *argv[]) {

	// Check that minimal cmd line args are present - 
	if (argc < 3) {
		printf(ERROR_MESSAGE);
		exit(1);
	}

	// Set test results output file for writing
	FILE *fp_results = fopen(argv[2], "w");
	if (fp_results == NULL)							// Handle errors
	{
		printf(ERROR_MESSAGE);						// At this point error can't be written to file...
		exit(1);
	}

	// Set test input file for reading
	FILE *fp_test = fopen(argv[1], "r");
	if (fp_test == NULL)							// Handle errors
	{
		printf(ERROR_MESSAGE);						// At this point error can't be written to file...
		exit(1);
	}

}
