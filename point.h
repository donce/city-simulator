#ifndef INC_POINT_H
#define INC_POINT_H



class point {
public:
	double x, y;
	
	point &operator-=(const point &a);
	point &operator-=(const double &a);
	point &operator+=(const point &a);
	point &operator+=(const double &a);

	const point operator+(const point &p) const;
	const point operator-(const point &p) const;

	
	point &operator*=(const double &k);
	const point operator*(const double &d) const;
	

	point &operator/=(const point &p);
	point &operator/=(const double &d);
	const point operator/(const double &d) const;
	
	
	point operator-();

	point(double cx, double cy);
	point();

	void Norm();

	double Length();
	
	void Rotate();

};


point Point(double x, double y);
double sqr(double n);

double Angle(point t);
double Angle(point a, point b);
double Length(point a, point b);


double DistancePointToLine(point p, point a, point b);

double CosBetween(point a, point b);

double GetProgress(point p, point a, point b);
bool OnRight(point p, point a, point b);
bool PointProjectionInsideSegment(point p, point a, point b);


#endif