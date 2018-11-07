/*
Authors - Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name - Ex2
Description - This program impelments an application testing tool. Test are run in parallel using threads
*/

// Includes --------------------------------------------------------------------
#include "TestEnvironment.h"


// Function Definitions --------------------------------------------------------

/*
Function createAppTestList
------------------------
Description – 
Parameters	– 
Returns		– 0 for success, -1 for failure
*/
int createAppTestList(char *tst_file_path[]) {

	// Set test input file for reading
	FILE *fp_test = fopen(tst_file_path, "r");
	if (fp_test == NULL)							 // Handle errors
	{
		printf("Failed to open input test file for reading\n");	 
		return (-1);
	}

}

/*
Function createTestResults
------------------------
Description –
Parameters	–
Returns		– 0 for success, -1 for failure
*/
int createTestResults(char *report_file_path[]) {

	// Set test results output file for writing
	FILE *fp_results = fopen(report_file_path, "w");
	if (fp_results == NULL)							// Handle errors
	{
		printf("Failed to open test results file for writing\n");
		exit(1);
	}
}

/*
* A simplified API for creating threads.
* Input Arguments:
*   p_start_routine: A pointer to the function to be executed by the thread.
*     This pointer represents the starting address of the thread.
*     The function has to have this specific signature:
*       DWORD WINAPI FunctionName(LPVOID lpParam);
*     With FunctionName being replaced with the function's name.
* Output Arguments:
*   p_thread_id: A pointer to a variable that receives the thread identifier.
*     If this parameter is NULL, the thread identifier is not returned.
* Return:
*   If the function succeeds, the return value is a handle to the new thread.
*   If the function fails, the return value is NULL.
*   To get extended error information, call GetLastError.
* Notes:
*   This function is just a wrapper for CreateThread.
*/
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		NULL,            /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		exit(ERROR_CODE);
	}

	return thread_handle;
}
