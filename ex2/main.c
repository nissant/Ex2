/*
Authors			- Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name	- Ex2
Description		- This program impelments an application testing environment. Tests are run in parallel using threads and results are
written in text output file once all threads have terminated.
*/

// Includes --------------------------------------------------------------------

#include "TestEnvironment.h"

int main(int argc, char *argv[]) {

	int *test_counter;
	HANDLE *thread_handles;
	DWORD wait_code;
	DWORD exit_code;
	BOOL ret_val;

	// Check that minimal cmd line args are present - 
	if (argc < 3) {									
		printf("Not enough input arguments!");
		exit(EXIT_FAILURE);
	}

	// Create test list
	test_app *test_list = NULL;
	if (createAppTestList(argv[1],&test_list,test_counter) != 0) {
		printf("An error occurred during test list creation, couldn't complete the task!\n");
		ClearTestList(test_list);
		exit(EXIT_FAILURE);
	}
	//
	thread_handles = (HANDLE*)malloc(sizeof(HANDLE)*(*test_counter));
	// Open test threads and call thread function
	if (runTests(test_list,thread_handles) != 0) {
		printf("An error occurred when creating thread, couldn't complete the task!\n");
		ClearTestList(test_list);
		exit(EXIT_FAILURE);
	}


	// Wait
	wait_code = WaitForMultipleObjects((DWORD)test_counter,thread_handles, true ,INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting\n");
		return ERROR_CODE;
	}

	// Create Test results file // TODO
	if (createTestResults(argv[3], test_list) != 0) {
		printf("An error occurred during results file creation, couldn't complete the task!\n");
		ClearTestList(test_list);
		exit(EXIT_FAILURE);
	}

	ClearTestList(test_list);
	exit(EXIT_SUCCESS);
}
