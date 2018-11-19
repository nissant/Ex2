/*
Authors			- Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name	- Ex2
Description		- This program impelments an application testing environment. Tests are run in parallel using threads and results are
written in text output file once all threads have terminated.
*/



#include "AppTest_Thread.h"
#define _CRT_SECURE_NO_WARNINGS

/*
Function runTestThreads
------------------------
Description – The function runs a process of the program with the given arguments and updates test struct with the result of the run
Parameters	– lpParam - used to get test data to be exuceted
Returns		– 0 for success, -1 for failure
*/
DWORD WINAPI runProc(LPVOID lpParam)
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	int					res;
	int path_flag;
	char  path_str[MAX_LINE_LEN];			// Used to extract the path of program that is going to run in the proccess
	TCHAR command[MAX_LINE_LEN];			// App full command line string with arguments
	TCHAR app_wdirectory[MAX_LINE_LEN];		// App working directory path for generated output .txt files

	test_app *test = (test_app*)lpParam;	// Get pointer to test data to execute in this thread
	
	path_flag = ExtractPath(test->app_cmd_line, path_str);			// Get folder path
	swprintf(app_wdirectory, MAX_LINE_LEN, L"%hs", path_str);		// Convert path string to TCHAR
	swprintf(command, MAX_LINE_LEN, L"%hs", test->app_cmd_line);	// Convert cmd string to TCHAR
	
	retVal = CreateProcessSimple(command, app_wdirectory ,&procinfo, path_flag);

	if (retVal == 0)
	{
		printf("Thread_Error - Couldn't create process, error code %d\n", GetLastError());
		return 1;
	}

	waitcode = WaitForSingleObject(procinfo.hProcess, TIMEOUT_IN_MILLISECONDS); // Waiting for the process to end

	if (waitcode == WAIT_TIMEOUT) // Process is still alive
	{
		strcpy(test->app_test_results, "Timed out");
		return 0;
	}
	
	if (waitcode == WAIT_FAILED) //status check failed running
	{
		printf("Thread_Error - Couldn't check process status (wait code), error code %d\n", GetLastError());
		return 1;		
	}
	if (waitcode == WAIT_OBJECT_0) //proccess ended. need to check results
	{
		GetExitCodeProcess(procinfo.hProcess, &exitcode);
		if (exitcode != 0)
		{
			char code[MAX_LINE_LEN];
			_itoa(exitcode, code, 10);
			strcpy(test->app_test_results, "Crashed ");
			strcat(test->app_test_results, code);
			return 0;
		}
		else     // program exit code is 0. need to compare results
		{
			res = CompareResults(test, path_str,path_flag); // need to update
			if (res == 0)
			{
				strcpy(test->app_test_results, "Succeeded");
				return 0;
			}
			if (res == 1)
			{
				strcpy(test->app_test_results, "Failed");
				return 0;
			}
			if (res == -1)
			{
				return 1; // failed comparing the files.
			}
		}
	}
}

/*
Function CompareResults
------------------------
Description – The function compares two text files.
Parameters	– *test - includes test data. used to get relevant paths, program name, etc. *path_str includes the path to the exe program (if supplied)
				path_not_valid indicates if there is a path in the path_str string or isn't.
Returns		– 1 for NOT equal files, 0 for equal files, -1 for problem with execution.
*/
int CompareResults(test_app *test, char *path_str, int path_not_valid)
{
	// the function comapres two text files.
	char path_to_result[MAX_LINE_LEN], tmp_str[MAX_LINE_LEN], *pointer1 ,*pointer2; // variables to create string with the path of the actual results
	FILE *fp_expected, *fp_actual;
	char expected, actual;

	
	if (!path_not_valid) // if there is a path to the results file
	{
		strcpy(tmp_str, test->app_cmd_line); // copy the test cmd_line to tmp string.
		pointer1 = strrchr(tmp_str, '\\');   // find the last \ of the path
		pointer1++;							// put pointer one to the begining of program name.
		pointer2 = strchr(tmp_str, '.');	// find the '.' of <program name>.exe
		*pointer2 = '\0';				// replace '.' with '\0' to copy later the program name.

		strcpy(path_to_result, path_str);    // put the path in to path_to_result variable
		strcat(path_to_result, "\\");
		strcat(path_to_result, pointer1);
		strcat(path_to_result, ".txt");
	}
	else
	{
		strcpy(tmp_str, test->app_cmd_line); // copy the test cmd_line to tmp string.
		pointer2 = strchr(tmp_str, '.');	// find the '.' of <program name>.exe
		*pointer2 = '\0';				// replace '.' with '\0' to copy later the program name.
		strcpy(path_to_result, tmp_str);
		strcat(path_to_result, ".txt");
	}

	fp_expected = fopen(test->app_exp_results_path, "r");
	fp_actual = fopen(path_to_result, "r");

	if (fp_expected == NULL || fp_actual == NULL)
	{
		printf("Thread_Error - error openining expected and/or actual test results files\n");
		return (-1);
	}	
	expected = getc(fp_expected);
	actual = getc(fp_actual);
	while (expected != EOF && actual != EOF)
	{
		if (expected == actual)
		{
			expected = getc(fp_expected);
			actual = getc(fp_actual);
			continue;
		}
			
		else
		{
			fclose(fp_expected);
			fclose(fp_actual);
			return 1; // files are NOT equal
		}
			
	}
	if (expected == actual)
	{
		fclose(fp_expected);
		fclose(fp_actual);
		return 0;	//files are equal
	}
		
	else
	{
		fclose(fp_expected);
		fclose(fp_actual);
		return 1; // files are NOT equal
	}
}

/*
Function CreateProcessSimple
------------------------
Description – The function runs a proccess (summons the CreaProcess function)
Parameters	– CommandLine - this is the command line for running the proccess. app_wdirectory - use parent directory if needed. 
				ProccessInfoPtr - used to write proccess information by the proccess itself. 
				path_flag - tells the program to use parent directory or different one (if path_flag ==1) 
Returns		– returns the return value from CreateProcess method.
*/
BOOL CreateProcessSimple(LPTSTR CommandLine, LPTSTR app_wdirectory, PROCESS_INFORMATION *ProcessInfoPtr, int path_flag)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */
	if (path_flag == 0)
	{
		return CreateProcess(NULL,	/*  No module name (use command line). */
			CommandLine,			/*  Command line. */
			NULL,					/*  Process handle not inheritable. */
			NULL,					/*  Thread handle not inheritable. */
			FALSE,					/*  Set handle inheritance to FALSE. */
			NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
			NULL,					/*  Use parent's environment block. */
			app_wdirectory,					/*  Use the supplied directory. */
			&startinfo,				/*  Pointer to STARTUPINFO structure. */
			ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
		);
	}
	else
	{
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

}

/*
Function ExtractPath
------------------------
Description – The function checks if the command line was supllied with full path or just program name
Parameters	– src is the command line. dst is the destination string that the path of the program is being copied to.
Returns		– returns 1 if the command line contains only program name (without path). returns 0 if the command line includes path to program.exe
*/
int ExtractPath(char *src, char *dst)
{
	char tmp_str[MAX_LINE_LEN], *tmp_p;
	strcpy(tmp_str, src);
	tmp_p = strrchr(tmp_str, '\\');
	if (tmp_p == NULL)
	{
		return 1;
	}

	else
	{
		*tmp_p = '\0';
		strcpy(dst, tmp_str);
		return 0;
	}

}


