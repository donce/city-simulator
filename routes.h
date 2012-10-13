#ifndef INC_ROUTES_H
#define INC_ROUTES_H

//#include <list>
#include <vector>

//#include "vehicles.h"
class vehicles;



class route {
public:
	
	//std::list<int> bL;
	int bL[2];//buildings in route

	int vM;

	double now;
	double interv;

	void Progress(double time);
	
	vehicles *v;
private:

	void LaunchVehicle();

};

class routes {
public:
	std::vector<route> r;



	void Init(const char *filename, vehicles *v0);
	//void Init(const char *filename);


	void Progress(double time);

private:

	void Read(const char *filename);
	
	vehicles *v;
};


#endif