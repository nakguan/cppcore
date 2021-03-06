#include "stdafx.h"
#include <time.h>

CSafeQueue<int> g_TaskQueue;

int ThreadFunc(void* pContext)
{
	int nID = (int)pContext;
	srand(nID);

	int nItem = -1;
	while( EC_SUCCESS == g_TaskQueue.Pop(nItem) )
	{
		Sleep(rand() % 1000 + 100);	// It takes a few time to process.
		printf("%d ", nItem);
	}
	return 0;
}

int main(int nArgc, char* pszArgs[])
{
	const int nTaskCount = 500;
	const int nWorkerCount = 5;
	HANDLE hThread[nWorkerCount];

	int i;

	// Make some tasks
	for( i = 0; i < nTaskCount; i++ )
		g_TaskQueue.Push(i+1);

	// Create workers
	for( i = 0; i<nWorkerCount; i++ )
		hThread[i] = CreateThread(ThreadFunc, (void*)i);

	// Wait to finish
	for( i = 0; i<nWorkerCount; i++ )
		JoinThread(hThread[i]);

	printf("\n");
    return 0;
}

