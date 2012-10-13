#ifndef INC_CITY_H
#define INC_CITY_H

#include "common.h"

#include <list>
#include <vector>


#include "line.h"


#include "vertex.h"
//#include "path.h"
#include "lights.h"

#include "buildings.h"

#include "routes.h"
//class routes;


class sign {
public:
	point p;
	double k;

	int nr;
};


class city {
		
public:
	
	int selectedBuilding;


	std::vector<vertex> p;	
	buildings b;
	std::vector<sign> signs;
	
	bool Init();
	void Clear();

	bool Load();

	
	void Generate();
	
	void Progress(double time, bool editor);
	
	
	void Draw();
		
	
	bool Crossroad(int nr);


	//lights
	bool LAvailable(int lightNr, int roadNr);


	point GetPointOnRoad(double progress, int from, int to, int lane);

		
	point SidePoint(int prev, int now, int next, double laneNr);	
	int LanesNr(int from, int to);
	
	rect CityRect();

	//adding/removing vehicles in vLists
	void AddVList(int nr, int from, int to, int lane);
	void RemVList(int nr, int from, int to, int lane);
	
	//for vehicle
	double GetRoadProgress(int from, int to, point pos);
	double DistanceToCrossroad(int from, int to, point pos);

	double Length(int from, int to);
	
	//path Path(int pointsL[2]);
	std::list<int> Path(int fromB, int toB);
	

	//for editor
	int AddVertex(point pos);
	int ClosestVertex(point pos);
	void AddRoad(int from, int to);

	int highlightVertex;
	
	bool drawSBuilding;
	//point sampleBuilding;
	building sampleBuilding;


	void DrawForEditor();

	//editor models
	model mFlag, mMarker;

	
	//void DrawSampleBuilding();
	void SetSampleBuilding(point pos);	
	void BuildSampleBuilding();

	bool drawRoutes;
	void DrawRoutes();

	
	routes r;
	
	//background


	//saving/loading
	void SaveCity();
	void LoadCity();
	

	void DrawBuildings(bool color = 0);
	
private:
	//rendering

	//buidlings
	void DrawBuilding(building *now);
	point GetBuildingPos(building *now);
	double buildingLinesProgress;


	void DrawSigns();

	void DrawCity();

	void DrawCrossroads();
	void DrawRoads();
	void DrawSidewalks();

	
	//GLuint glRoads, glCrossroads;
	GLuint glCity;

	GLuint tRoad, tCrossroad, tSidewalk, tSidewalkSide;

	model mSigns[5];
	
	void DrawBackground();
	GLuint tGround, tWater;
	
	bool LoadMap(const char *mapFilename);
	void LoadLights(const char *filename);

	void MakeRoad(int nr, int begin);	
	std::vector<std::vector<int>> roads;//between crossroads			
	void MakeRoads();	
	line SideLine(int from, int to, double side);//private
	bool *used;//change it to function's parameter


	lights l;	

	int Neigh(int from, int notTo);
	
	//for sorting
	void SortRoad(int nr);
	void SortRoads();
	//for generating
	void GenerateSides();


	//for path
	void AddFakePoints(int fromB, int toB);
	void city::RemoveFakePoints(int toB);


	//open
	bool OpenRoads(const char *mapFilename);


	//buildings
	void SetAvailBuildingProgress(building *bNow);
	
	double GetBuildingPosNoCollision(double progr, building *bCheck, bool left, bool right);
	void BuildingRoadBounds(double bounds[2], int from, int to);



};
//-----------------------------------------------------------------


#endif