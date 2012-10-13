#ifndef INC_BUILDINGS_H
#define INC_BUILDINGS_H


#include <fstream>
#include <vector>

//#include "city.h"

#include <map>


//#include "model.h"

#include "bModels.h"


class building {
public:
	int r[2];//road points
	double d;//dist from first
	
	bModel *bM;

	building();
};

class buildings {
public:

	bModels bM;

	std::vector<building> b;

	int Load(char *filenameModels,char *filenameBuildings);
	void LoadModels(char *filename);
	void LoadBuildings(char *filename);

	void Clear();
	
private:

};
	


#endif