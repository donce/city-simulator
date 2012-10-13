#include "point.h"

#include <math.h>

point::point()
{
	x = y = 0;
}

point::point(double cx, double cy)
{
	x = cx;
	y = cy;
}

const point point::operator+(const point &p) const
{
	point t;
	t.x = x + p.x;
	t.y = y + p.y;
	return t;
}

const point point::operator-(const point &p) const
{
	point t;
	t.x = x - p.x;
	t.y = y - p.y;
	return t;
}

point &point::operator*=(const double &k)
{
	x *= k;
	y *= k;

	return *this;
}

const point point::operator*(const double &d) const
{
	return Point(d*x, d*y);
}

point &point::operator/=(const point &p)
{
	x /= p.x;
	y /= p.y;

	return *this;
}

point &point::operator/=(const double &d)
{
	x /= d;
	y /= d;

	return *this;
}

const point point::operator/(const double &d) const
{
	point p;
	p.x = x / d;
	p.y = y / d;
	return p;
}

point &point::operator+=(const point &a)
{
	x += a.x;
	y += a.y;

	return *this;
}
	
point &point::operator-=(const point &a)
{	
	x -= a.x;
	y -= a.y;

	return *this;
}


point point::operator-()
{
	point p;
	p.x = -x;
	p.y = -y;

	return p;
}


void point::Norm()
{
	double l = Length();

	x /= l;
	y /= l;

}

point &point::operator+=(const double &a)
{
	x += a;
	y += a;

	return *this;
}

point &point::operator-=(const double &a)
{
	return operator+=(-a);
}

double point::Length()
{
	return sqrt(sqr(x) + sqr(y));
}


void point::Rotate()
{
	double a = x;

	x = -y;
	y = a;
}





point Point(double x, double y)
{
	point p(x, y);
	return p;
}

double sqr(double n)
{
	return n*n;
}

double Angle(point t)
{
    return atan2(t.y, t.x) * 180 / 3.1415;
}

double Angle(point a, point b)
{
    return Angle(b - a);
}

double Length(point a, point b)
{
	point p = a - b;
	return p.Length();
}

double DistancePointToLine(point p, point a, point b)
{
	double d;

	point x = p - a;
	point y = b - a;

	double c = CosBetween(x, y);
	double s = sqrt(1 - c*c);

	d = y.Length() * s;
	
	d = abs((b.x - a.x) * (a.y - p.y) - (a.x - p.x) * (b.y - a.y)) /
			   sqrt(sqr(b.x - a.x) + sqr(b.y - a.y));

	return d;
}


double CosBetween(point a, point b)
{
	double c;

	if (a.Length() * b.Length() == 0)
		return 0;

	c = (a.x * b.x + a.y * b.y) / (a.Length() * b.Length());

	return c;
}



double GetProgress(point p, point a, point b)
{
	point x = p - a;
	point y = b - a;

	double pr = CosBetween(x, y) * x.Length();// / Length(a, b);

	return pr;
}


bool OnRight(point p, point a, point b)
{
	point x = b - a;
	x.Rotate();

	return CosBetween(p - a, x) >= 0;
}

bool PointProjectionInsideSegment(point p, point a, point b)
{
	double progress = GetProgress(p, a, b);
	return (0 <= progress && progress <= Length(a, b));
}
