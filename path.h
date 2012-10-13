#ifndef INC_PATH_H
#define INC_PATH_H

#include <list>

class path {
public:
	
	std::list<int> p;//points

	int Prev();
	int Now ();
	int Next();

	void Skip();

};


#endif