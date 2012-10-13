#include "routes.h"

#include <fstream>

#include "vehicles.h"



void route::Progress(double time)
{
	now += time;
	
	while (now > interv)
	{
		now -= interv;
		LaunchVehicle();
	}
}

void route::LaunchVehicle()
{
	v->Add(vM, bL);
}

void routes::Init(const char *filename, vehicles *v0)
//void routes::Init(const char *filename)
{
	v = v0;
	Read(filename);
}

void routes::Read(const char *filename)
{
	std::ifstream f(filename);
	
	int nR;//routes
	int nB;//buildings

	//int bNr;

	f >> nR;

	route *rT;//temp

	int i, j;
	for (i = 0; i < nR; i++)
	{
		//route
		rT = new route;

		//f >> nB;

		for (j = 0; j < 2; j++)
		{
			f >> rT->bL[j];
			//f >> bNr;
			//rT->bL.push_back(bNr);
		}


		//car
		f >> rT->vM;		

		//interval
		f >> rT->interv;
		rT->now = 0;

		//classes
		//rT->b = b;
		rT->v = v;

		r.push_back(*rT);
		delete rT;
	}

	f.close();
}


void routes::Progress(double time)
{
	for (int i = 0; i < r.size(); i++)
		r[i].Progress(time);
}