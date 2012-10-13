#include "vehicleList.h"

void vehicleList::Add(int nr) 
{
	v.push_back(nr);
}

void vehicleList::Rem(int nr) 
{
	v.erase(FindValue(nr));
}

std::list<int>::iterator vehicleList::FindValue(int nr) 
{
	std::list<int>::iterator it;
	for (it = v.begin(); it != v.end(); it++)
		if (*it == nr)
			return it;
	

	//no such value
	return v.end();
}
