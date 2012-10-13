#ifndef INC_ROAD_H
#define INC_ROAD_H

#include "vehicleList.h"
#include "common.h"

class road {
public:
	int to;
	int lanes;

	point side[2][2];//for road generation and signs; road, sidewalk 0 - right, 1 - left
	double k;//angle

	vehicleList *v;
};

#endif