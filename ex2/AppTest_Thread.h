/*
Authors			- Eli Slavutsky (308882992) & Nisan Tagar (302344031)
Project Name	- Ex2
Description		- This program impelments an application testing environment. Tests are run in parallel using threads and results are
					written in text output file once all threads have terminated.
				- This module handles the thread function and application under test process management  
*/

#include "TestEnvironment.h"

#ifndef APPTEST_THREAD_H
#define APPTEST_THREAD_H
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS // need this for itoa func.

#define TIMEOUT_IN_MILLISECONDS 10000

// Function Declarations -------------------------------------------------------
DWORD WINAPI runProc(LPVOID lpParam);
BOOL CreateProcessSimple(LPTSTR CommandLine, LPTSTR app_wdirectory, PROCESS_INFORMATION *ProcessInfoPtr, int path_flag);
int CompareResults(test_app *test, char *path_str, int path_flag);
int ExtractPath(char *src, char *dst);
void closeProcHandle(PROCESS_INFORMATION procinfo);

#endif