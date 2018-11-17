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


	char  path_str[MAX_LINE_LEN];					// Used to extract the path of program that is going to run in the proccess
	TCHAR command[MAX_LINE_LEN];			// App full command line string with arguments
	TCHAR app_wdirectory[MAX_LINE_LEN];		// App working directory path for generated output .txt files

	test_app *test = (test_app*)lpParam;	// Get pointer to test data to execute in this thread
	printf("This thread has access to test with full command: %s\n", test->app_cmd_line);	// Just testing...
	
	ExtractPath(test->app_cmd_line, path_str);
		
	swprintf(app_wdirectory, MAX_LINE_LEN, L"%hs", path_str);	// Convert path string to TCHAR
	swprintf(command, MAX_LINE_LEN, L"%hs", test->app_cmd_line);	// Convert cmd string to TCHAR
	retVal = CreateProcessSimple(command, app_wdirectory ,&procinfo);

	if (retVal == 0)
	{
		printf("Thread_Error - Couldn't create process, error code %d\n", GetLastError());
		return(-1);
	}

	waitcode = WaitForSingleObject(procinfo.hProcess, TIMEOUT_IN_MILLISECONDS); // Waiting for the process to end

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
		return 0;
	}
	
	if (waitcode == WAIT_FAILED) //status check failed running
	{
		printf("Thread_Error - Couldn't check process status (wait code), error code %d\n", GetLastError());
		return -1;		//need to check what to do if there is failure 
	}
	if (waitcode == WAIT_OBJECT_0) //proccess ended. need to check results
	{
		GetExitCodeProcess(procinfo.hProcess, &exitcode);
		if (exitcode != 0)
		{
			printf("program crashed. exit code is non zero\n");
			char code[MAX_LINE_LEN];
			_itoa(exitcode, code, 10);
			strcpy(test->app_test_results, "Crashed ");
			strcat(test->app_test_results, code);
			return 0;
		}
		else     // program exit code is 0. need to compare results
		{
			res = CompareResults(test, path_str); // need to update
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
				return -1; // failed comparing the files.
			}
		}
	}
	return 0;
}

int CompareResults(test_app *test, char *path_str)
{
	// the function comapres two text files.
	char tmp_str[MAX_LINE_LEN], *tmp_str_p, *tmp_str_p2, path_to_result[MAX_LINE_LEN]; // variables to create string with the path of the actual results
	FILE *fp_expected, *fp_actual;
	char expected, actual;
	
	strcpy(tmp_str, test->app_cmd_line); //copy cmd line to tmp_str
	tmp_str_p = strrchr(tmp_str, '\\'); // put pointer to the last slash before program name
	tmp_str_p++;						// put pointer to begining of program name
	tmp_str_p2 = strchr(tmp_str_p, '.');   // find the '.' of ".exe"
	*tmp_str_p2 = '\0';						//replace '.' with '\0'
	strcpy(tmp_str, tmp_str_p); // put only the program name to tmp_str
	strcat(tmp_str, ".txt");   // add .txt to program name
	strcpy(path_to_result, path_str);
	strcat(path_to_result, "\\");  // 
	strcat(path_to_result, tmp_str);	// add <program name>.txt to the path. this is the final path.
	

	fp_expected = fopen(test->app_exp_results_path, "r");
	fp_actual = fopen(path_to_result, "r");

	if (fp_expected == NULL || fp_actual == NULL)
	{
		printf("Thread_Error - error openining expected and/or actual test results files\n");
		fclose(fp_expected);
		fclose(fp_actual);
		return (-1);
	}	
	expected = getc(fp_expected);
	actual = getc(fp_actual);
	while (expected != EOF & actual != EOF)
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

BOOL CreateProcessSimple(LPTSTR CommandLine, LPTSTR app_wdirectory, PROCESS_INFORMATION *ProcessInfoPtr)
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
		app_wdirectory,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}

void ExtractPath(char *src, char *dst)
{
	/*
	char tmp_str[MAX_LINE_LEN], *tmp_p;
	strcpy(tmp_str, src);
	tmp_p = strrchr(tmp_str, '\\');
	if (tmp_p == NULL)
	{
		strcpy(dst, NULL);
		return 1;
	}

	else
	{
		*tmp_p = '\0';
		strcpy(dst, tmp_str);
		return 0;
	}
	*/
	strcpy(dst, NULL);
}


