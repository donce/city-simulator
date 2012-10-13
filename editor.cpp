#include "editor.h"

#include "common.h"

void editor::ChangeMode(int newMode)
{
	prev = c->highlightVertex = -1;
	mode = newMode;

	c->drawSBuilding = (mode == 3);
	c->drawRoutes = (mode == 4);
}


void editor::Init(city *ct)
{
	c = ct;
	mode = 0;
	prev = -1;
}


void editor::Progress(point3 pos, double time)
{
	pressedDown += time;

	if (mode == 3)
	{
		point p(pos.x, pos.z);
		c->SetSampleBuilding(p);
		//c->sampleBuilding = p;

	}

}


void editor::MouseLeftDown(point3 pos)
{
	pressedDown = 0;
}


void editor::MouseLeftUp(point3 pos)
{
	if (pressedDown < 200)
	{

		if (mode == 2)
		{
			point now;
			double height;

			now.x = pos.x;
			now.y = pos.z;
			height = pos.y;

			int nowNr = c->ClosestVertex(now);


			if (prev != -1)
			{
				if (prev == nowNr)
					prev = c->highlightVertex = -1;
				else
				{
					if (nowNr != -1)
					{
						c->AddRoad(prev, nowNr);
						prev = c->highlightVertex = -1;
					}
					else
					{
						//make new vertex
						int newP = c->AddVertex(now);
						c->AddRoad(prev, newP);
					
						prev = c->highlightVertex = newP;
					}
				
					c->Generate();
				}

			}
			else
			{
				if (nowNr != -1)
					prev = c->highlightVertex = nowNr;
			}
		}
		else if (mode == 3)
		{
			//build a building
			c->BuildSampleBuilding();
		}
	}
}