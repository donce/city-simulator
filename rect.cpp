#include "rect.h"


rect::rect()
{
	p[0] = p[1] = Point(0, 0);
}

rect::rect(point a, point b)
{
	p[0] = a;
	p[1] = b;
}

rect::rect(double x1, double y1, double x2, double y2)
{
	p[0] = Point(x1, y1);
	p[1] = Point(x2, y2);
}


rect &rect::operator+=(const point &a)
{
	p[0] += a;
	p[1] += a;

	return *this;
}

point rect::Size()
{
	point s;
	s = p[1] - p[0];

	return s;
}


point rect::Center()
{
	point c = (p[0] + p[1]) / 2;

	//c /= 2;
	return c;
}
