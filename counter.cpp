#include "counter.h"

#include <Windows.h>

counter::counter()
{
	counterFreq = counterLast = 0;
}


double counter::GetCount()
{
	LARGE_INTEGER lI;
	QueryPerformanceCounter(&lI);

	double now = double(lI.QuadPart - counterLast) / counterFreq * 1000;
	counterLast = lI.QuadPart;

	return now;
}


void counter::Init()
{
	LARGE_INTEGER lI;
	QueryPerformanceFrequency(&lI);
	counterFreq = lI.QuadPart;

	GetCount();
}
