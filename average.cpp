#include "average.h"


average::average()
{
	t = 0;
	av = 0;
}

void average::Add(double value, double time)
{
	now = value;
	if (t + time)
		av = ((av * t) + (now * time)) / (t + time);
	t += time;
}

double average::GetAverage()
{
	return av;
}

double average::GetNow()
{
	return now;
}

