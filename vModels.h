#ifndef INC_VMODELS_H
#define INC_VMODELS_H


#include <vector>
#include <map>
#include <string>
#include "model.h"

class vModel {
public:

	double maxSpeed;
	double a;//acceleration
	double b;//breakes

	double l;//length(half lenght actually)

	
	model m;

};

class vModels {
public:
	//std::vector<vModel> vM;
	vModel *vM;

	int GetN();

	void Load(char *filename);
	int GetID(std::string name);

	
private:
	std::map<std::string, int> id;	
	int n;

};

#endif