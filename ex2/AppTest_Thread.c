/*
Authors			- Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name	- Ex2
Description		- This program impelments an application testing environment. Tests are run in parallel using threads and results are
written in text output file once all threads have terminated.
*/



#include "AppTest_Thread.h"
#define _CRT_SECURE_NO_WARNINGS

DWORD WINAPI runProc(LPVOID lpParam)
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	int					res;
	//TCHAR tmp_str[MAX_LINE_LEN];

	test_app *test = (test_app*)lpParam; // Get pointer to test data to execute in this thread
	
	printf("This thread has access to test with full command: %s\n", test->app_cmd_line);

	//swprintf(tmp_str, MAX_LINE_LEN, L"%hs", test->app_cmd_line); // Convert string to TCHAR
	retVal = CreateProcessSimple(_T("c:\\windows\\notepad.exe"), &procinfo); //"c:\\windows\\notepad.exe"

	if (retVal == 0)
	{
		printf("Couldn't create process, error code %d\n", GetLastError());
	}

	waitcode = WaitForSingleObject(procinfo.hProcess, TIMEOUT_IN_MILLISECONDS); // Waiting for the process to end

	/* Commenting for testing application without thread function

	printf("checking proccess status (wait code)\n"); //debug prints
	switch (waitcode)
	{
	case WAIT_TIMEOUT:
		printf("WAIT_TIMEOUT\n"); break;
	case WAIT_OBJECT_0:
		printf("WAIT_OBJECT_0\n"); break;
	case WAIT_FAILED:
		printf("WAIT_FAILED\n");
	}

	if (waitcode == WAIT_TIMEOUT) // Process is still alive
	{
		printf("Process did not finish before timeout!\n");
		strcpy(test->app_test_results, "Timed out");
		return;
	}
	
	if (waitcode == WAIT_FAILED) //status check failed running
	{
		printf("Couldn't check process status (wait code), error code %d\n", GetLastError());
		return;		//need to check what to do if there is failure 
	}
	if (waitcode == WAIT_OBJECT_0) //proccess ended. need to check results
	{
		GetExitCodeProcess(procinfo.hProcess, &exitcode);
		if (exitcode != 0)
		{
			printf("program crashed. exit code is non zero\n");
			char tmp_str[100];
			_itoa(exitcode, tmp_str, 10);
			strcpy(test->app_test_results, "Crashed ");
			strcat(test->app_test_results, tmp_str);
			return;
		}
		else     // program exit code is 0. need to compare results
		{
			res = CompareResults(test); // need to update
			if (res == 0)
			{
				strcpy(test->app_test_results, "Succeeded");
				return;
			}
			else
			{
				strcpy(test->app_test_results, "Failed");
				return;
			}
		}
	}
	*/
}

int CompareResults(test_app *test)
{
	// the function comapres two text files.
	return 0;
}

BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */

	return CreateProcess(NULL,	/*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}



