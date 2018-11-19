/*
Authors			- Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name	- Ex2
Description		- This program impelments an application testing environment. Tests are run in parallel using threads and results are
written in text output file once all threads have terminated.
*/

// Includes --------------------------------------------------------------------
#include "TestEnvironment.h"
#include "AppTest_Thread.h"

#define ERROR_CODE -1
#define _CRT_SECURE_NO_WARNINGS


// Function Definitions --------------------------------------------------------
/*
Function runTestThreads
------------------------
Description – The function opens the tests threads and returns a list of thread handles
Parameters	– *test_list_ptr pointer to test list, *thread_handles pointer to array of list handles
Returns		– 0 for success, -1 for failure
*/
int runTestThreads(test_app *test_list_ptr, HANDLE *thread_handles) {
	int i = 0;
	HANDLE tmp_thread_handle;
	HANDLE *tmp_thread_handle_ptr = &tmp_thread_handle;

	if (thread_handles == NULL) {
		return -1;
	}

	// Itterate over test list and open test threads
	while (test_list_ptr != NULL) {
		// Open new thread and pass to handler routine the test pointer to data
		if (CreateThreadSimple(runProc, test_list_ptr, &test_list_ptr->test_thread_id, tmp_thread_handle_ptr) != 0){
			// Thread creation failed
			return -1;
		}
		test_list_ptr->test_thread_handle = tmp_thread_handle;
		thread_handles[i] = test_list_ptr->test_thread_handle;
		test_list_ptr = test_list_ptr->next_test;
		i++;
	}
	return 0;
}

/*
Function: checkThreads
------------------------
Description – The function receive pointer to the head of tests list and free's allocated memory
Parameters	– *lst_ptr is a pointer to the head of the tests list.
Returns		– Nothing
*/
int checkThreads(test_app *test_list_ptr) {

	DWORD ExitCode;
	DWORD *lpExitCode = &ExitCode;

	if (test_list_ptr == NULL) {
		return -1;
	}

	// Itterate over test list and check thread exit codes
	while (test_list_ptr != NULL) {
		if (GetExitCodeThread(test_list_ptr->test_thread_handle, lpExitCode)) {
			if (ExitCode != 0) {
				printf("Test thread (for test command line: %s) has failed with exit code: %d \n", test_list_ptr->app_cmd_line, &ExitCode);
			}
		}
		test_list_ptr = test_list_ptr->next_test;
	}
	return 0;
}

/*
Function createAppTestList
------------------------
Description – 
Parameters	– 
Returns		– 0 for success, -1 for failure
*/
int createAppTestList(char *tst_file_path, test_app **lst_ptr,int *test_counter) {

	test_app *tmp_t = NULL;
	char line[MAX_LINE_LEN];
	
	int counter = 0;

	// Set test input file for reading
	FILE *fp_test = fopen(tst_file_path, "r");
	if (fp_test == NULL)							 
	{
		printf("Failed to open input test file for reading\n");	 
		return (-1);
	}

	while (fgets(line, MAX_LINE_LEN, fp_test))	// read line from file
	{
		tmp_t = MakeTestFromLine(line);			// create a test entry from the line
		if (tmp_t == NULL)
			return -1;
		AddTestToList(lst_ptr, tmp_t);			//add test to the test list
		counter++;
	}
	*test_counter = counter;
	fclose(fp_test);
	return 0;

}

/*
Function createResultsFile
------------------------
Description – This function generates a test report in specified path. 
			##The tests are assumed to be executed successfully with results string present!##
Parameters	– *report_file_path - pointer to the file path string, *lst_ptr - pointer the head of test linked list
Returns		– 0 for success, -1 for failure
*/
int createResultsFile(char *report_file_path, test_app *lst_ptr) {

	// Set test results output file for writing
	int test_count = 1;
	FILE *fp_results = fopen(report_file_path, "w");
	if (fp_results == NULL)							// Handle errors
	{
		return (-1);
	}

	while (lst_ptr != NULL) {
		fprintf(fp_results, "test #%d : %s\n", test_count, lst_ptr->app_test_results);
		test_count++;
		lst_ptr = lst_ptr->next_test;
	}
	return 0;
}

/*
Function MakeTestFromLine
------------------------
Description – The function gets a string containing test data and creates a test instance.
Parameters	– *line is a string which includes test data.
Returns		– Pointer to new test data entry
*/
test_app *MakeTestFromLine(char *line)
{
	char *tmp_t = line, *location = NULL;
	test_app *new_test;

	new_test = (test_app*)malloc(sizeof(test_app));
	if (NULL == new_test) // Handle malloc fail
	{
		return new_test;
	}

	line = trimwhitespace(line); // Trim leading/trailing whitespace


	// Find and copy application path with command line arguments
	location = strrchr(tmp_t, ' ');
	if (location) { 
		*location = '\0';
		strcpy(new_test->app_cmd_line, tmp_t);
		tmp_t = location + 1;
	}
	
	// Copy application test expected results file path
	strcpy(new_test->app_exp_results_path, tmp_t);

	new_test->next_test = NULL;
	strcpy(new_test->app_test_results, "");

	return new_test;
}

/*
Function AddTestToList
------------------------
Description – The function adds a new test to the app tests linked list
Parameters	– **lst_ptr is pointer to a pointer to the head of the pilot list (Is output if list is empty and just created),
				*new_test is a pointer to the new pilot node that is to be added to the linked list
Returns		– Nothing
*/
void AddTestToList(test_app **lst_ptr, test_app *new_test)
{
	test_app *temp_t = *lst_ptr;
	if (temp_t == NULL)					// if list is empty
	{
		*lst_ptr = new_test;
		return;
	}
	while (temp_t->next_test != NULL)
	{
		temp_t = temp_t->next_test;		//move pointer to the last test in the list
	}
	temp_t->next_test = new_test;
}

/*
Function CreateThreadSimple
------------------------
Description – A simplified API for creating threads. This function is just a wrapper for CreateThread.
Parameters	– p_start_routine: A pointer to the function to be executed by the thread,
			  test_app *tst_ptr: A pointer to the test being passed to the thread as argument,
			  p_thread_id: A pointer to a variable that receives the thread identifier,
*					If this parameter is NULL, the thread identifier is not returned.
			HANDLE *thread_handle_ptr: A pointer to the handle to the new thread. If the function
			fails the return value us NULL
Returns		– 0 for success, -1 for failure
*/
int CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,test_app *tst_ptr,LPDWORD p_thread_id, HANDLE *thread_handle_ptr)
{
	HANDLE thread_handle = NULL;
	*thread_handle_ptr = thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Received null pointer for thread routine");
		return -1;
	}

	if (NULL == p_thread_id)
	{
		printf("Received null pointer for thread ID");
		return -1;
	}

	if (NULL == tst_ptr)
	{
		printf("Received null pointer for routine argument");
		return -1;
	}

	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		tst_ptr,         /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		return -1;
	}
	
	*thread_handle_ptr = thread_handle;
	return 0;
}

/*
Function: trimwhitespace
------------------------
Description – The function receive pointer to a string and trimms the string from white spaces
Parameters	– *str is a pointer to a string to be trimmed.
Returns		– Retrun pointer to trimmed string
*/
char *trimwhitespace(char *str)
{
	char *end;

	// Trim leading space
	while (isspace((unsigned char)*str)) str++;

	if (*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end)) end--;

	// Write new null terminator character
	end[1] = '\0';

	return str;
}

/*
Function: ClearTestList
------------------------
Description – The function receive pointer to the head of tests list and free's allocated memory
Parameters	– *lst_ptr is a pointer to the head of the tests list.
Returns		– Nothing
*/
void ClearTestList(test_app *lst_ptr)
{
	test_app *tmp_t;
	while (lst_ptr != NULL) // while list is not empty
	{
		tmp_t = lst_ptr;
		lst_ptr = lst_ptr->next_test;
		free(tmp_t);
	}
}
