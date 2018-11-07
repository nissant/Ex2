/*
Authors			- Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name	- Ex2
Description		- This program impelments an application testing environment. Tests are run in parallel using threads and results are
written in text output file once all threads have terminated.
*/

// Includes --------------------------------------------------------------------
#include "TestEnvironment.h"

#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char *argv[]) {

	// Check that minimal cmd line args are present - 
	if (argc < 3) {									
		printf("Not enough input arguments!");
		exit(EXIT_FAILURE);
	}
	// Create test list
	test_app *test_list = NULL;
	if (createAppTestList(argv[2],&test_list) != 0) {
		printf("An error occurred during test list creation, couldn't complete the task!\n");
		ClearTestList(test_list);
		exit(EXIT_FAILURE);
	}

	// Open test threads and call thread function
	runTests(test_list);

	// Wait for test threads to receive exit command and terminate
	/* TODO
		if (waitForTestThreads() != 0) {

		}
		wait_code = WaitForSingleObject(p_thread_handles[0], INFINITE);
		if (WAIT_OBJECT_0 != wait_code)
		{
			printf("Error when waiting");
			return ERROR_CODE;
		}
	*/

	// Create Test results file // TODO
	if (createTestResults(argv[3]) != 0) {
		printf("An error occurred during results file creation, couldn't complete the task!\n");
		ClearTestList(test_list);
		exit(EXIT_FAILURE);
	}

	ClearTestList(test_list);
	exit(EXIT_SUCCESS);
}
