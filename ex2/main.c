/*
Authors - Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name - Ex2
Description - This program impelments an application testing tool. Test are run in parallel using threads
*/

// Includes --------------------------------------------------------------------
#include "TestEnvironment.h"


#define _CRT_SECURE_NO_WARNINGS
#define ERROR_MESSAGE "An error occurred during execution, couldn't complete the task!\n"

int main(int argc, char *argv[]) {

	// Check that minimal cmd line args are present - 
	if (argc < 3) {									
		printf("Not enough input arguments!");
		exit(EXIT_FAILURE);
	}

	if (createAppTestList(argv[2]) != 0) {			
		printf("An error occurred during test list creation, couldn't complete the task!\n");
		exit(EXIT_FAILURE);
	}


	


	if (createTestResults(argv[3]) != 0) {
		printf("An error occurred during results file creation, couldn't complete the task!\n");
		// TODO - Free resources
		exit(EXIT_FAILURE);
	}

}
