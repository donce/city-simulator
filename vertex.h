#ifndef INC_VERTEX_H
#define INC_VERTEX_H

#include "road.h"
#include <vector>

class vertex {
private:
	
	int Turning(int nr, int d);

public:
	point pos;

	//roads(neighbours)
	std::vector<road> r;


	road *GetRoadTo(int to);

	int ToRight(int nr);
	int ToLeft(int nr);
	
	int GetRoadID(int nr);//public only for random road
};

#endif