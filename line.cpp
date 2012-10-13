#include "line.h"


bool Parallel(line a, line b)
{
	return ((a.vert && b.vert) || ((a.k == b.k) && !a.vert && !b.vert));
}

point Colide(line a, line b)
{

	if (Parallel(a, b))
		return Point(0, 0);//parallel lines
	
	//a.k != b.k
	double x, y;
	
	//check for vertical lines
	if (a.vert)
	{
		x = a.x;
		y = b.Value(x);
	}
	else if (b.vert)
	{
		x = b.x;
		y = a.Value(x);
	}
	else
	{
		x = (b.b - a.b) / (a.k - b.k);
		y = a.Value(x);
	}

	return Point(x, y);
}

