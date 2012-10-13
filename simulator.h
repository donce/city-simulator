#ifndef INC_SIMULATOR_H
#define INC_SIMULATOR_H

#include "vehicles.h"
#include "city.h"




class simulator {
public :
	
	city c;
	vehicles v;
	
	bool Init();
	void Deinit();

	void InitSim();
	void DeinitSim();

	bool LoadMap();
	
	void Progress(double time, bool editor);

	void Draw();

private:
	int GetVNr();
	

	obstacle RoadAvail(int from, int to, double start);//for VehicleObstacle(int nr);
	
};


#endif