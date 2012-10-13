
#ifndef INC_LINE_H
#define INC_LINE_H

#include "common.h"

class line {
public:
	double k, b;
	
	bool vert;
	double x;

	line()
	{
		vert = 0;
		k = b = 0;
	}

	void Set(point pa, point pb)
	{
		//k
		if (pb.x - pa.x != 0)
		{
			vert = 0;
			k = (pb.y - pa.y) / (pb.x - pa.x);
			
			//b
			b = ((pa.y * pb.x) - (pb.y * pa.x)) / (pb.x - pa.x);

		}
		else
		{
			//vertical
			vert = 1;
			x = pa.x;
		}
	}

	double Value(double x)
	{
		return k*x + b;
	}
};

point Colide(line a, line b);


bool Parallel(line a, line b);

#endif