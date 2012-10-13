#include "vertex.h"

int vertex::GetRoadID(int nr)
{
	for (int i = 0; i < r.size(); i++)
		if (r[i].to == nr)
			return i;


	return -1;//no road to there
}

//nr - from
//d - roads to turn(change)
int vertex::Turning(int nr, int d)
{
	int id = GetRoadID(nr);

	int turnId = (id + d + r.size()) % r.size();

	return r[turnId].to;
}

int vertex::ToRight(int nr)
{
	return Turning(nr, -1);
}

int vertex::ToLeft(int nr)
{
	return Turning(nr, 1);

}

road *vertex::GetRoadTo(int to)
{
	return &r[GetRoadID(to)];
}
