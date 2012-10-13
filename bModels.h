#ifndef INC_BMODELS_H
#define INC_BMODELS_H

#include "model.h"


class bModel {
public:
	double length;

	model m;

};


class bModels {
public:
	int n;
	bModel *bM;

	void Load(char *filename);

};


#endif