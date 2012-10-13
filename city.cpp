#include "city.h"

#include <windows.h>	// Header File For Windows

#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
//#include <gl\glaux.h>											// Header File For The GLaux Library

#include "graphics.h"											// Header File For NeHeGL

#include <math.h>


#include <fstream>
#include <algorithm>

#include "const.h"


bool city::Crossroad(int nr)
{
	return p[nr].r.size() > 2;
}


int city::Neigh(int from, int notTo)
{
	int now;
	for (int i = 0; i < p[from].r.size(); i++)
	{
		now = p[from].r[i].to;
		if (now != notTo)
			return now;
	}

	return -1;
}


point city::SidePoint(int prev, int now, int next, double laneNr)
{	
	const double sideC = -(0.5 + laneNr);

	if (prev == next)
		return p[now].pos;

	line a = SideLine(next, now, sideC);
	line b = SideLine(now, prev, sideC);

	point res;

	if (!Parallel(a, b))
		res = Colide(a, b);
	else
	{
		//return Point(10000, 100000);

		//straight road
		point s = p[now].pos - p[prev].pos;
		s.Norm();
		s.Rotate();

		res = p[now].pos - (s*sideC);

	}

	return res;
}

void city::MakeRoad(int nr, int begin)
{
	std::vector<int> road;

	road.push_back(begin);

	int prev = begin;
	int temp;

	while (nr != -1)
	{
		road.push_back(nr);
		if (!Crossroad(nr))
			used[nr] = 1;	
		else
			break;

		temp = nr;
		nr = Neigh(nr, prev);
		prev = temp;
	}
	
	roads.push_back(road);

}

void city::MakeRoads()
{
	used = new bool[p.size()];
	roads.clear();

	int i, j;

	for (i = 0; i < p.size(); i++)
		used[i] = 0;

	int now;
	for (i = 0; i < p.size(); i++)
		if (Crossroad(i))
		{
			used[i] = 1;
			for (j = 0; j < p[i].r.size(); j++)
			{
				now = p[i].r[j].to;
				if (!used[now])
				{
					if (!Crossroad(now))
						used[now] = 1;
					MakeRoad(now, i);
				}
			}
		}
		
	delete []used;
}

void DrawPoint(point a, double h = 0)
{
	glVertex3d(a.x, h, a.y);
}

line city::SideLine(int from, int to, double side)
{
	line a;

	point s = p[to].pos - p[from].pos;
	s.Norm();
	s.Rotate();
	s *= side;
	
	//line
	point pa, pb;
	pa = p[from].pos + s;
	pb = p[to].pos + s;
	a.Set(pa, pb);

	return a;
	//MAYBE CHANGE WITH SOMETHING MORE MATHEMATICAL!!!
}


void city::DrawCrossroads()
{
	//crossroads	
	glBindTexture(GL_TEXTURE_2D, tCrossroad);

	for (int i = 0; i < p.size(); i++)
		if (Crossroad(i))
		{
			glBegin(GL_POLYGON);
			
			glNormal3d(0, 1, 0);

			for (int j = 0; j < p[i].r.size(); j++)
			{
				int from = p[i].r[j].to;
				point a = p[from].GetRoadTo(i)->side[0][0];

				glTexCoord2d(a.x, a.y);
				DrawPoint(a);
			}

			glEnd();
		}	
}

void city::DrawRoads()
{
	//roads
	glBindTexture(GL_TEXTURE_2D, tRoad);

	glBegin(GL_QUADS);
	glNormal3d(0, 1, 0);	

	for (int i = 0; i < roads.size(); i++)
	{				
		double tPos[2];
		tPos[0] = tPos[1] = 0;

		for (int j = 0; j+1 < roads[i].size(); j++)
		{
			int now, next;
			now = roads[i][j];
			next = roads[i][j+1];
			
			road *rTo, *rFrom;
			
			rTo = p[now].GetRoadTo(next);
			rFrom = p[next].GetRoadTo(now);
			point *nowS, *nextS;//sides
			nowS = rFrom->side[0];
			nextS = rTo->side[0];

			//length
			double tNow[2];
			tNow[0] = ::Length(nowS[0], nextS[1]);
			tNow[1] = ::Length(nowS[1], nextS[0]);
			
			tPos[1] = tPos[0] + GetProgress(nowS[1], nowS[0], nextS[1]);
						
			
			glTexCoord2d(rFrom->lanes, tPos[0]);          DrawPoint(nowS[0]);
			glTexCoord2d(rTo->lanes, tPos[0] + tNow[0]);  DrawPoint(nextS[1]);
			glTexCoord2d(-rTo->lanes, tPos[1] + tNow[1]); DrawPoint(nextS[0]);
			glTexCoord2d(-rFrom->lanes, tPos[1]);         DrawPoint(nowS[1]);			

			
			tPos[0] += tNow[0];
			tPos[1] += tNow[1];
		}			
	}

	glEnd();
}

void city::DrawSidewalks()
{
	//sidewalks
	
	//{from}{from H}{to}{to H}
	int param[3][4] = {{0, 0, 0, 1}, {0, 1, 1, 1}, {1, 0, 1, 1}};
	
	//glBindTexture(GL_TEXTURE_2D, tSidewalk);


	
	for (int i = 0; i < roads.size(); i++)
		for (int k = 0; k < 2; k++)//which side
		{
			double tPos[2];
			tPos[0] = tPos[1] = 0;

			for (int j = 0; j+1 < roads[i].size(); j++)
			{
				int now, next;
				now = roads[i][j];
				next = roads[i][j+1];
					
				road *rTo, *rFrom;
			
				rTo = p[now].GetRoadTo(next);
				rFrom = p[next].GetRoadTo(now);
				point nowS[2], nextS[2];//sides

				nowS[0] = rFrom->side[0][k];
				nowS[1] = rFrom->side[1][k];
					
				nextS[0] = rTo->side[0][!k];
				nextS[1] = rTo->side[1][!k];			

				//length
				point a = p[next].pos - p[now].pos;
				double tNow[2];
				tNow[0] = ::Length(nowS[0], nextS[0]);
				tNow[1] = ::Length(nowS[1], nextS[1]);

				tPos[1] = tPos[0] + GetProgress(nowS[1], nowS[0], nextS[0]);

				for (int l = 0; l < 3; l++)
				{
					int *par = param[l];

					if (l == 1)
						glBindTexture(GL_TEXTURE_2D, tSidewalk);
					else
						glBindTexture(GL_TEXTURE_2D, tSidewalkSide);
					
					glBegin(GL_QUADS);
					glNormal3d(0, 1, 0);
					
					glTexCoord2d(1, tPos[par[0]]);				   DrawPoint(nowS[par[0]], par[1] * 0.1);
					glTexCoord2d(1, tPos[par[0]] + tNow[par[0]]);  DrawPoint(nextS[par[0]], par[1] * 0.1);
					glTexCoord2d(0, tPos[par[2]] + tNow[par[2]]);  DrawPoint(nextS[par[2]], par[3] * 0.1);
					glTexCoord2d(0, tPos[par[2]]);				   DrawPoint(nowS[par[2]], par[3] * 0.1);			
				
					glEnd();
				
				}
					
				tPos[0] += tNow[0];
				tPos[1] += tNow[1];
			}		
		}		
	
}


void city::DrawCity()
{		
	glColor3d(1, 1, 1);

	DrawCrossroads();
	
	MakeRoads();
	DrawRoads();
	DrawSidewalks();	
}

void city::Clear()
{
	p.clear();
}

bool city::LAvailable(int lightNr, int roadNr)
{
	return l.Available(lightNr, roadNr);
}

bool city::OpenRoads(const char *mapFilename)
{
	std::ifstream f(mapFilename);

	int i, j;

	int pNr;//points' number
	f >> pNr;

	int rNr;

	vertex *temp;

	road tempR;
	for (i = 0; i < pNr; i++)
	{
		temp = new vertex;

		f >> temp->pos.x >> temp->pos.y;
		f >> rNr;

		for (j = 0; j < rNr; j++)
		{
			f >> tempR.to;
			tempR.lanes = 3;
			tempR.v = new vehicleList[tempR.lanes];
			temp->r.push_back(tempR);
		}
		
		p.push_back(*temp);
		delete temp;
	}
	
	f.close();

	return 0;
}

point city::GetPointOnRoad(double progress, int from, int to, int lane)
{

	point pV = p[to].pos - p[from].pos;
	pV.Norm();
	point a = p[from].pos + pV * progress;

	//push to right road part
	pV.Rotate();

	a += pV * (lane + 0.5);

	return a;
}



int city::LanesNr(int from, int to)
{
	int end = p[from].r.size();

	for (int i = 0; i < end; i++)
		if (p[from].r[i].to == to)
			return p[from].r[i].lanes;
		
	while(1)
	{
		Beep(500, 1000);
	}

	return -10000;
}

bool city::Init()
{
	b.LoadModels(c_file_building_models);

	
	mSigns[0].Open("signs/pole");
	mSigns[1].Open("signs/light");
	mSigns[2].Open("sings/noentry");
	mSigns[3].Open("signs/main");
	mSigns[4].Open("signs/giveaway");	
		
	mFlag.Open("others/pointer");
	mMarker.Open("others/marker");

	
	std::string str = c_dir_tex_main_path;
	tRoad = LoadTexture(str + "road.bmp");
	tCrossroad = LoadTexture(str + "crossroad.bmp");
	tGround = LoadTexture(str + "ground.bmp");
	tWater = LoadTexture(str + "water.bmp");
	tSidewalk = LoadTexture(str + "sidewalk.bmp");
	tSidewalkSide = LoadTexture(str + "sidewalk_side.bmp");

	highlightVertex = -1;

	return 0;
}

bool city::LoadMap(const char *mapFilename)
{
	OpenRoads(mapFilename);


	return 0;
}

void city::LoadLights(const char *filename)
{
	l.Create(p.size(), filename);
	
    std::ifstream f(c_file_parameters);
	
	for (int i = 0; i < l.Lights(); i++)
		f >> l.l[i].cTime[0] >> l.l[i].cTime[1];
		
	f.close();	

	for (int i = 0; i < l.Lights(); i++)
		l.l[i].Clean();

}

bool city::Load()
{
	LoadMap(c_file_map);
	b.LoadBuildings(c_file_buildings);
	LoadLights(c_file_lights);
	
	Generate();

	return 0;
}

void city::Generate()
{
	SortRoads();
	GenerateSides();
	
	signs.clear();
	//put signs
	sign *sNow;
	for (int i = 0; i < p.size(); i++)
		if (l.is[i])
		{
			for (int j = 0; j < p[i].r.size(); j++)
			{
				sNow = new sign;

				int from = p[i].r[j].to;
				//to i
				sNow->nr = 1;
				sNow->p = p[from].GetRoadTo(i)->side[0][0];
				sNow->k = p[from].GetRoadTo(i)->k;

				signs.push_back(*sNow);
				delete sNow;
			}

		}

	//generate meshes
		
	//glIsList
	glDeleteLists(glCity, 1);//don't need, because glNewList replaces old?
	glCity = glGenLists(1);
	glNewList(glCity, GL_COMPILE);
	DrawCity();
	glEndList();
}

void city::Progress(double time, bool editor)
{
	if (!editor)
	{
		l.Progress(time);
	}
	else
	{
		buildingLinesProgress += time;
		buildingLinesProgress -= 1000 * int(buildingLinesProgress / 1000);//modf(buildingLinesProgress + time, 1000);
	}
}

rect city::CityRect()
{
	rect r;
	r.p[0] = r.p[1] = p[0].pos;

	for (int i = 1; i < p.size(); i++)
	{
		r.p[0].x = min(r.p[0].x, p[i].pos.x);
		r.p[0].y = min(r.p[0].y, p[i].pos.y);

		r.p[1].x = max(r.p[1].x, p[i].pos.x);
		r.p[1].y = max(r.p[1].y, p[i].pos.y);
	}

	return r;
}

double atan2(point p)
{
	return atan2(p.y, p.x);
}

void city::SortRoad(int nr)
{
	//compareNr = nr;

	//calculate angles
	for (int i = 0; i < p[nr].r.size(); i++)
	{
		int to = p[nr].r[i].to;
		p[nr].r[i].k = atan2(p[to].pos - p[nr].pos) * (180/c_pi);
	}

	int i, j;

	int minNr;
	double minV;//value
	
	double nowV;

	road temp;

	for (i = 0; i < p[nr].r.size(); i++)
	{
		//find min
		minNr = i;
		minV = p[nr].r[i].k;
		for (j = i+1; j < p[nr].r.size(); j++)
		{
			nowV = p[nr].r[j].k;
			if (nowV < minV)
			{
				minNr = j;
				minV = nowV;
			}
		}
		
		//swap
		temp = p[nr].r[i];
		p[nr].r[i] = p[nr].r[minNr];
		p[nr].r[minNr] = temp;	
	}
}

void city::SortRoads()
{
	for (int i = 0; i < p.size(); i++)
		SortRoad(i);
}

void city::GenerateSides()
{
	for (int i = 0; i < p.size(); i++)
		for (int j = 0; j < p[i].r.size(); j++)
		{
			point *s;
			//s[0] = p[i].r[j].side[0];

			int lanes = p[i].r[j].lanes;
			//i-now-next
			int now = p[i].r[j].to;

			for (int k = 0; k < 2; k++)
			{
				s = p[i].r[j].side[k];
				if (p[now].r.size() < 2)
				{
					//deadend
					point a;//road side
					a = p[now].pos - p[i].pos;
					a.Norm();
					a.Rotate();

					a *= lanes + k;

					s[0] = p[now].pos + a;
					s[1] = p[now].pos - a;
				}
				else
				{				
					//not deadend
					int next;

					next = p[now].ToRight(i);				
					s[0] = SidePoint(i, now, next, lanes - 0.5 + k);

					next = p[now].ToLeft(i);
					s[1] = SidePoint(next, now, i, lanes - 0.5 + k);
				} 
			}
		}
}


void city::AddVList(int nr, int from, int to, int lane)
{
	p[from].GetRoadTo(to)->v[lane].Add(nr);
}

void city::RemVList(int nr, int from, int to, int lane)
{	
	p[from].GetRoadTo(to)->v[lane].Rem(nr);
}


bool Foo(point a, point b)
{
	double cosA = (a.x * b.x + a.y * b.y) / (a.Length() + b.Length());
	return cosA < 0;
}


double city::GetRoadProgress(int from, int to, point pos)
{
	return GetProgress(pos, p[from].pos, p[to].pos);
	/*
	point pFrom = p[from].pos;
	point pTo = p[to].pos;

	point v = pTo - pFrom;
	//v.Norm();
	v.Rotate();

	point a = pFrom;
	point b = pFrom + v;

	double d = DistancePointToLine(pos, a, b);

	//double lA, lB, lC;
	//lA = Length(pTo - pFrom);

	if (Foo(pTo - pFrom, pos - pFrom))
		d *= -1;

	return d;
	*/
}


double city::DistanceToCrossroad(int from, int to, point pos)
{
	point pFrom = p[from].pos;
	point pTo = p[to].pos;
	
	point v = pTo - pFrom;

	point *sides = p[from].GetRoadTo(to)->side[0];

	point pos2 = pos + v;

	line a, b;
	a.Set(sides[0], sides[1]);
	b.Set(pos, pos2);

	point c = Colide(a, b);
	
	point l = c - pos;


	return l.Length();
}

double city::Length(int from, int to)
{
	return ::Length(p[to].pos, p[from].pos);
}


void city::AddFakePoints(int fromB, int toB)
{
	int begin, end;
	begin = p.size();
	end = p.size() + 1;


	//add fake points
	vertex *vNew;
	int *rP;				
	road *rTemp;

	//from
	vNew = new vertex;
	rTemp = new road;

	rP = b.b[fromB].r;//from
	
	vNew->pos = (p[rP[0]].pos + p[rP[1]].pos) / 2;

	//roads from it
	rTemp->to = rP[0];
	vNew->r.push_back(*rTemp);
	rTemp->to = rP[1];
	vNew->r.push_back(*rTemp);
			
		
	p.push_back(*vNew);
	delete rTemp;
	delete vNew;


	//to	
	vNew = new vertex;
	rTemp = new road;
	
	rP = b.b[toB].r;//to

	vNew->pos = (p[rP[0]].pos + p[rP[1]].pos) / 2;
		
	p.push_back(*vNew);

	//roads to it
	rTemp->to = end;

	p[rP[0]].r.push_back(*rTemp);
	p[rP[1]].r.push_back(*rTemp);
		
	delete rTemp;
	delete vNew;
}

void city::RemoveFakePoints(int toB)
{
	//to
	p.pop_back();

	//roads to it
	int nr;
	for (int i = 0; i < 2; i++)
	{
		nr = b.b[toB].r[i];
		p[nr].r.pop_back();
	}

	//from
	p.pop_back();
}

std::list<int> city::Path(int fromB, int toB)
{
	std::list<int> l;
	
	int begin, end;
	begin = p.size();
	end = p.size() + 1;
	
	AddFakePoints(fromB, toB);

	
	bool *used = new bool[p.size()];
	double *d = new double[p.size()];
	int *back = new int[p.size()];

	for (int i = 0; i < p.size(); i++)
	{
		used[i] = 0;
		d[i] = -1;
	}

	used[begin] = 1;
	d[begin] = 0;

	int now = begin;

	while (d[end] < 0)
	{
		used[now] = 1;

		//find new distances from now
		for (int i = 0; i < p[now].r.size(); i++)
		{
			int nrNow = p[now].r[i].to;
			double dNow = d[now] + Length(now, nrNow);
			if ((dNow < d[nrNow]) || (d[nrNow] < 0))
			{
				back[nrNow] = now;
				d[nrNow] = dNow;
			}
		}

		//find shortest unused
		now = -1;
		for (int i = 0; i < p.size(); i++)
 			if (!used[i] && (d[i] > 0) && (now == -1 || d[i] < d[now]))
			{
				now = i;

			}
	}

	//generate path
	now = end;
	while (now != begin)
	{
		l.push_back(now);
		now = back[now];
	}
	l.push_back(begin);

	l.reverse();
	
	RemoveFakePoints(toB);

	int *sideP;

	//beginning
	l.pop_front();
	sideP = b.b[fromB].r;
	if (l.front() == sideP[0])
		l.push_front(sideP[1]);
	else
		l.push_front(sideP[0]);
	//ending
	l.pop_back();
	sideP = b.b[toB].r;
	if (l.back() == sideP[0])
		l.push_back(sideP[1]);
	else
		l.push_back(sideP[0]);

	//add one more for calculating direction
	std::list<int>::iterator it;
	it = l.end();
	it--;
	it--;
	
	l.push_back(p[l.back()].ToRight(*it));
		
	return l;
}


point city::GetBuildingPos(building *now)
{
	int ra, rb;
	ra = now->r[0];
	rb = now->r[1];

	point pa, pb, a;
		
	pa = p[ra].pos;
	pb = p[rb].pos;

	double l = now->d / Length(ra, rb);//Length(pa, pb);

	a = pa + ((pb - pa)*l);

	point side;
	side = pb - pa;
		
	side.Norm();
	side *= LanesNr(ra, rb) + 1;

	side.Rotate();
	a += side;

	return a;
}

void city::DrawBuilding(building *now)
{
	point a = GetBuildingPos(now);
		

	point pa, pb;
	pa = p[now->r[0]].pos;
	pb = p[now->r[1]].pos;
	double k = Angle(pa, pb);
		
	now->bM->m.Draw(a, k);
}

void city::DrawBuildings(bool color)
{
	//buildings
	for (int i = 0; i < b.b.size(); i++)
	{
		if (color)
			glColor3ub(i + 1, 0, 0);
		DrawBuilding(&b.b[i]);
			
		if (selectedBuilding == i && !color)
		{
			//add marker
			mMarker.Draw(GetBuildingPos(&b.b[i]));
		}
	}
}


int city::AddVertex(point pos)
{
	vertex vNew;
	vNew.pos = pos;
	p.push_back(vNew);

	return (p.size() - 1);
}

int city::ClosestVertex(point pos)
{
	if (!p.size())
		return -1;

	int minNr = 0;

	for (int i = 1; i < p.size(); i++)
		if (::Length(p[i].pos, pos) < ::Length(p[minNr].pos, pos))
			minNr = i;

	if (::Length(p[minNr].pos, pos) > C_MAX_LENTH_SELECTING_VERTEX)
		minNr = -1;//not close enough

	return minNr;

}

void city::AddRoad(int from, int to)
{
	if (p[from].GetRoadID(to) != -1)
		return;

	road rNow;

	rNow.lanes = 2;
	rNow.to = to;
	p[from].r.push_back(rNow);

	rNow.to = from;
	p[to].r.push_back(rNow);

}

void city::DrawRoutes()
{
	bool shaders = glIsEnabled(GL_LIGHTING);

	if (shaders)
		glDisable(GL_LIGHTING);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBegin(GL_LINES);
	for (int i = 0; i < r.r.size(); i++)
	{
		double pr = buildingLinesProgress / 1000;
		point pa, pb;
		pa = GetBuildingPos(&b.b[r.r[i].bL[0]]);
		pb = GetBuildingPos(&b.b[r.r[i].bL[1]]);

		point v = pb - pa;

		double l = v.Length();

		v.Norm();

		point now = pa;

		bool color = (pr > 0.5);
		if (pr > 0.5)
			pr -= 0.5;

		now = pa;

		bool first = 1;

		while (l > 0)
		{
			//color
			if (color)
				glColor3f(1, 1, 1);
			else
				glColor3f(1, 0, 0);
			color ^= 1;
				
			glVertex3f(now.x, 1, now.y);

			if (first)
			{
				first = 0;
				now += v * pr * 2;
				l -= pr*2;
			}
			else if (l <= 1)
			{				
				now = pb;
				l = -1;
			}
			else
			{
				now += v;
				l -= 1;
			}

			glVertex3f(now.x, 1, now.y);				
		}

		/*	
		if (color)
			glColor3f(1, 1, 1);
		else
			glColor3f(1, 0, 0);
			
		glVertex3f(now.x, 1, now.y);
		glVertex3f(now.x, 1, now.y);
		*/

	}
	glEnd();
		
	glColor3f(1, 1, 1);
	if (shaders)
		glEnable(GL_LIGHTING);

}

void city::DrawForEditor()
{
	if (highlightVertex != -1)	
		mFlag.Draw(p[highlightVertex].pos);
	
	if (drawSBuilding && sampleBuilding.bM != 0)//drawSBuilding
		DrawBuilding(&sampleBuilding);

	if (drawRoutes)
		DrawRoutes();

}

void city::Draw()
{		
	glCallList(glCity);
	
	DrawBuildings();

	DrawSigns();

	if (1)
		DrawForEditor();

	DrawBackground();
}


void city::DrawSigns()
{	
	sign *ts;//temp sign
	for (int i = 0; i < signs.size(); i++)
	{
		ts = &signs[i];
		glTranslated(ts->p.x, 0,  ts->p.y);
		glRotated(ts->k + 90, 0, -1, 0);

		mSigns[0].Draw();

		glTranslated(0, 0.42, 0);
		mSigns[ts->nr].Draw();
		glTranslated(0, -0.42, 0);
		
		glRotated(ts->k + 90, 0, 1, 0);
		glTranslated(-ts->p.x, 0, -ts->p.y);
	}	
}


void city::DrawBackground()
{
	//ground
	glBindTexture(GL_TEXTURE_2D, tGround);
		
	const int c_groundSize = 300;
	const float c_groundSqrSize = 1;

	float c_tex = float(c_groundSize)/c_groundSqrSize;
	
	
	rect groundR = CityRect();//optimize this!!!!!
	groundR.p[0] -= 50;
	groundR.p[1] += 50;

	glBegin(GL_QUADS);	

	point gTex = groundR.Size();
	glNormal3d(0, 1, 0);

	glTexCoord2d(0, 0);	glVertex3d(groundR.p[0].x, -0.1, groundR.p[0].y);
	glTexCoord2d(gTex.x, 0);	glVertex3d(groundR.p[1].x, -0.1, groundR.p[0].y);
	glTexCoord2d(gTex.x, gTex.y);	glVertex3d(groundR.p[1].x, -0.1, groundR.p[1].y);
	glTexCoord2d(0, gTex.y);	glVertex3d(groundR.p[0].x, -0.1, groundR.p[1].y);
	
	glEnd();
	
	//water
	glBindTexture(GL_TEXTURE_2D, tWater);	
	glBegin(GL_QUADS);
	glNormal3d(0, 1, 0);

	glTexCoord2f(0, 0);
	glVertex3f(-200, -50, -200);
	glTexCoord2f(1, 0);
	glVertex3f( 200, -50, -200);
	glTexCoord2f(1, 1);
	glVertex3f( 200, -50,  200);
	glTexCoord2f(0, 1);
	glVertex3f(-200, -50,  200);

	glEnd();
}

double city::GetBuildingPosNoCollision(double progr, building *bCheck, bool left, bool right)
{
	double lCheck = bCheck->bM->length;
	
	double bounds[2];
	BuildingRoadBounds(bounds, bCheck->r[0], bCheck->r[1]);
	
	if (progr - lCheck < bounds[0] ||
		progr + lCheck > bounds[1])
		return 2*(bounds[1] - bounds[0]);
		
	//find colliding
	for (int i = 0; i < b.b.size(); i++)
	{
		building *bNow = &b.b[i];
		double lNow = bNow->bM->length;

		if (bNow->r[0] == bCheck->r[0] && bNow->r[1] == bCheck->r[1])
			if (abs(bNow->d - progr) + 0.01 < lCheck + lNow)
			{
				//collides
				double best;
				
				if (left)
					best = GetBuildingPosNoCollision(bNow->d - lNow - lCheck, bCheck, 1, 0);	
		
				if (right)
				{
					double now = GetBuildingPosNoCollision(bNow->d + lNow + lCheck, bCheck, 0, 1);
					if (!left || (abs(now - progr) < abs(best - progr)))
						best = now;
				}

				return best;
			}
	}

	return progr;
}

void city::SetAvailBuildingProgress(building *bNow)
{
	bNow->d = GetBuildingPosNoCollision(bNow->d, bNow, 1, 1);
}

void city::BuildingRoadBounds(double bounds[2], int from, int to)
{
	bounds[0] = GetProgress(p[to].GetRoadTo(from)->side[1][1], p[from].pos, p[to].pos);
	bounds[1] = GetProgress(p[from].GetRoadTo(to)->side[1][0], p[from].pos, p[to].pos);
}

void city::SetSampleBuilding(point pos)
{
	double best = 0;
	double now;
	
	sampleBuilding.bM = 0;

	building bNow;//, bBest;
	bNow.bM = &b.bM.bM[0];

	for (int i = 0; i < p.size(); i++)
		for (int j = 0; j < p[i].r.size(); j++)
		{
			int from = i;
			int to = p[i].r[j].to;
						
			if (!OnRight(pos, p[from].pos, p[to].pos))
				Swap(from, to);
			
			//bounds
			double bounds[2];
			BuildingRoadBounds(bounds, from, to);
			
			bNow.r[0] = from;
			bNow.r[1] = to;

			bNow.d = GetProgress(pos, p[from].pos, p[to].pos);
			bNow.d = max(bNow.d, bounds[0] + bNow.bM->length);
			bNow.d = min(bNow.d, bounds[1] - bNow.bM->length);
			SetAvailBuildingProgress(&bNow);			
			
			if (bNow.d >= bounds[0] + bNow.bM->length &&
				bNow.d + bNow.bM->length <= bounds[1])
			{					
				now = ::Length(GetBuildingPos(&bNow), pos);				

				if (now < best || !sampleBuilding.bM)
				{
					best = now;
					sampleBuilding = bNow;
				}
			}
		}

	if (best > 20)
		sampleBuilding.bM = 0;
}


void city::BuildSampleBuilding()
{
	if (sampleBuilding.bM != 0)
		b.b.push_back(sampleBuilding);
}

template <class T>
void WriteBin(std::ofstream &f, T t)
{
	f.write((char*)(&t), sizeof(t));
}


void city::SaveCity()
{
	//std::ofstream f("temp.txt");
	std::ofstream f("temp.txt", std::ios::binary);
	

	//map
	WriteBin(f, p.size());
	for (int i = 0; i < p.size(); i++)
	{
		WriteBin(f, p[i].pos);
		
		WriteBin(f, p[i].r.size());
		for (int j = 0; j < p[i].r.size(); j++)
		{
			WriteBin(f, p[i].r[j].to);
			WriteBin(f, p[i].r[j].lanes);
		}
	}

	//lights?(signs?)

	//parameters?(include this in signs?)
	
	//buildings
	WriteBin(f, b.b.size());
	for (int i = 0; i < b.b.size(); i++)
	{
		f.write((char*)b.b[i].r, sizeof(b.b[i].r)*2);
		WriteBin(f, b.b[i].d);
		//MODEL : CHANGE!
		//WriteBin(f, b.b[i].mNr);

	}


	//routes
	


	f.close();
}

void city::LoadCity()
{

}