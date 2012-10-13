#ifndef INC_EDITOR_H
#define INC_EDITOR_H

#include "point3.h"

#include "city.h"

class editor {
public:
	int mode;
	int prev;

	city *c;

	/*
	mode
	0 - empty(explore)
	1 - vertexes
	2 - roads
	3 - buildings
	4 - routes
	*/

	double pressedDown;

	void Progress(point3 pos, double time);

	
	void Init(city *ct);

	void MouseLeftDown(point3 pos);
	void MouseLeftUp(point3 pos);

	void ChangeMode(int newMode);
};

#endif;