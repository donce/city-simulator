#include "path.h"


int path::Prev()
{
	return p.front();
}

int path::Now ()
{
	std::list<int>::iterator it;
	it = p.begin();
	it++;
	return *it;
}

int path::Next()
{
	std::list<int>::iterator it;
	it = p.begin();
	it++;
	it++;
	return *it;
}

void path::Skip()
{
	p.pop_front();
}
