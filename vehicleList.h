#ifndef INC_VEHICLELIST_H
#define INC_VEHICLELIST_H

#include <list>

class vehicleList {
public:
	void Add(int nr);
	void Rem(int nr);
	
	std::list<int> v;
	
private:	
	
	std::list<int>::iterator FindValue(int nr);
};

#endif