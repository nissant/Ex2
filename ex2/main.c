/*
Authors			- Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name	- Ex2
Description		- This program impelments an application testing environment. Tests are run in parallel using threads and results are
written in text output file once all threads have terminated.
*/

// Includes --------------------------------------------------------------------

#include "TestEnvironment.h"

int main(int argc, char *argv[]) {

	DWORD wait_code;
	DWORD exit_code;
	BOOL ret_val;

	// Check that exactly 2 cmd line args are present
	if (argc < 3) {									
		printf("Not enough input arguments!");
		exit(EXIT_FAILURE);
	}

	if (argc > 3) {
		printf("Too many input arguments!");
		exit(EXIT_FAILURE);
	}

	// Create test linked list
	int test_counter = 0;
	int *test_counter_ptr = &test_counter;
	test_app *test_list = NULL;
	if (createAppTestList(argv[1],&test_list, test_counter_ptr) != 0) {
		printf("An error occurred during test list creation, couldn't complete the task!\n");
		ClearTestList(test_list);
		exit(EXIT_FAILURE);
	}

	// Open all test threads
	HANDLE *thread_handles = NULL;
	thread_handles = (HANDLE*)malloc(sizeof(HANDLE)*(test_counter)); // Created for using WaitForMultipleObjects, malloc success is checked in runTestThreads routine
	if (runTestThreads(test_list,thread_handles) != 0) {
		printf("An error occurred when creating thread, couldn't complete the task!\n");
		ClearTestList(test_list);
		free(thread_handles);
		exit(EXIT_FAILURE);
	}

	// Wait for threads to finish
	wait_code = WaitForMultipleObjects((DWORD)test_counter,thread_handles, true ,INFINITE);
	if (WAIT_OBJECT_0 != wait_code){
		printf("Error while waiting for threads to finish, couldn't complete the task!\n");
		ClearTestList(test_list);
		free(thread_handles);
		exit(EXIT_FAILURE);
	}

	// Check thread exit codes
	if (checkThreads(test_list) != 0) {
		ClearTestList(test_list);
		free(thread_handles);
		exit(EXIT_FAILURE);
	}

	// At this point all test threads have finished successfully - Create Test results file
	if (createResultsFile(argv[2], test_list) != 0) {
		printf("An error occurred during results file creation, couldn't complete the task!\n");
		ClearTestList(test_list);
		free(thread_handles);
		exit(EXIT_FAILURE);
	}

	ClearTestList(test_list);
	free(thread_handles);
	exit(EXIT_SUCCESS);
}
