#ifndef INC_RECT_H
#define INC_RECT_H

#include "point.h"

class rect {
public:
	point p[2];
	
	rect();
	rect(point a, point b);
	rect(double x1, double y1, double x2, double y2);
	
	rect &operator+=(const point &a);

	point Size();

	point Center();

};


#endif