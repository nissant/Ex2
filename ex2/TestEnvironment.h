/*
Authors			- Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name	- Ex2
Description		- This program impelments an application testing environment. Tests are run in parallel using threads and results are
					written in text output file once all threads have terminated.
*/

#ifndef TESTENVIRONMENT_H
#define TESTENVIRONMENT_H
// Includes --------------------------------------------------------------------
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "AppTest_Thread.h"



// Constants -------------------------------------------------------------------


#define MAX_LINE_LEN 100
#define MAX_RESULTS_LINE_LEN 15

#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))


typedef struct {
	HANDLE test_thread_handles;						// Test thread data
	DWORD test_thread_id;							// Test thread data
	char app_path[MAX_LINE_LEN];					// String of size MAX_LINE_LEN holding app full path
	char app_args[MAX_LINE_LEN];					// String of size MAX_LINE_LEN holding app complete argument string
	char app_exp_results_path[MAX_LINE_LEN];		// String of size MAX_LINE_LEN holding app expected resuts full path
	char app_test_results[MAX_RESULTS_LINE_LEN];	// String of size MAX_RESULTS_LINE_LEN holding test results: 
													// "Succeeded", "Timed Out", "Failed", "Crashed -xx", "NULL" for test not finished
	struct test_app *next_test;
} test_app;

// Function Declarations -------------------------------------------------------

void runTests(test_app *test_list_ptr);
char *trimwhitespace(char *str);
int createAppTestList(char *tst_file_path, test_app **test_list_ptr);
int createTestResults(char *report_file_path[]);
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, test_app *tst_ptr, LPDWORD p_thread_id);
void ClearTestList(test_app *tst_lst);

#endif // !APPTEST_H_INCLUDE






