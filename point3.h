#ifndef INC_POINT3_H
#define INC_POINT3_H

class point3 {
public:
	double x, y, z;


	const point3 operator-(const point3 a) const
	{
		point3 n;
		n.x = x - a.x;
		n.y = y - a.y;
		n.z = z - a.z;

		return n;
	}
};

#endif