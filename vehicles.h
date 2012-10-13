#ifndef INC_VEHICLES_H
#define INC_VEHICLES_H

#include <list>
#include <vector>

#include "common.h"

#include "path.h"
#include "obstacle.h"
#include "vModels.h"

class city;
class vehicles;

class vehicle {
public :	
	double speed;
	
	int nr;//vehicle nr(id, which doesn't mean vector index!)

	vModel *vM;//vehicle model

	double progr;//from road beginning
	double targetProgr;//last segment's

	int lNr;//lane nr

	//double avail;
	obstacle obs;

	path p;//road
	point Destination();//vertex i kuri reikia vaziuoti
	

	point pos;
	double k;

	void Init(int vNr, city *_c, vehicles *_v, vModel *vModel, int bL[2]);

	bool Progress(double time);

	void Draw();

	
private :	

	city *c;
	vehicles *v;


	point Direction();

	void Drive(double d);

	void Brake(double time);
	void Accelerate(double time);


	int GetRoadLanesNr();

	//lists
	void AddList();
	void RemList();

	
	bool BetterAtLane(int nr);
	void ChangeLane(int newLane);

	int changingLaneFrom;
	double changingLaneProgr;

	void UpdatePosition();
	void UpdateRotation();

};



class vehicles {
public:
	std::vector<vehicle> v;	
	vModels vM;

	
	void Add(int vMNr, int bL[2]);


	void Init(city *ct);
	void Clear();

	
	obstacle VehicleObstacle(int nr, int lane = -1);
	obstacle VehicleObstacleBehind(int nr, int lane = -1);
	
	void Progress(double time);
	
	void Draw();

	int GetVehiclesNr();

	//statistics
	double AverageSpeed();
	double AverageMaxSpeed();
	int StoppedVehicles();
	

private:
		


	city *c;
	
	std::vector<bool> availID;
	std::list<int> idL;//list of all available ID's

	std::list<int>::iterator GetIDLTo(int to);

	
	obstacle RoadAvail(double start, int from, int to, int lane, bool inFront = 1);


	void Remove(int id);
	void ReleaseID(int id);

	int GetFreeID();
};

#endif