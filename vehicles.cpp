#include "vehicles.h"

#include <math.h>
#include "common.h"
#include "const.h"

#include "city.h"


point vehicle::Direction()
{
   point t;

   t.x = cos(k*3.1415/180);
   t.y = sin(k*3.1415/180);

   return t;
}

void vehicle::Drive(double d)
{
	point kr;
	kr = Direction();
	pos.x += kr.x*d;
	pos.y += kr.y*d;

	progr += d;
}


void vehicle::Accelerate(double time)
{
	speed += vM->a*time;
	speed = min(speed, vM->maxSpeed);
}


void vehicle::Brake(double time)
{
	speed -= vM->b*time;

	if (speed < 0)
		speed = 0;
}


int vehicle::GetRoadLanesNr()
{
	return c->LanesNr(p.Prev(), p.Now());
}


bool vehicle::BetterAtLane(int lane)
{
	if (lane < 0 || lane >= GetRoadLanesNr())
		return 0;

	//obstacle o = v->RoadAvail(progr, p.Prev(), p.Now(), nr);
	obstacle o = v->VehicleObstacle(nr, lane);
	obstacle oB = v->VehicleObstacleBehind(nr, lane);


	if (o.d > obs.d && oB.d > 0)
		return 1;

	return 0;
}

void vehicle::Init(int vNr, city *_c, vehicles *_v, vModel *vModel, int bL[2])
{
	nr = vNr;
	c = _c;
	v = _v;
	vM = vModel;
	
	p.p = c->Path(bL[0], bL[1]);
	
	//clear
	speed = 0;
	k = 0;
	lNr = 0;	
	
	lNr = c->LanesNr(p.Prev(), p.Now()) - 1;
	//------------------------------------------------------------
	//starting pos
	building *bT;
	bT = &c->b.b[bL[0]];
	
	progr = bT->d;

	UpdatePosition();
	UpdateRotation();
	//------------------------------------------------------------
	//targerProgress
	bT = &c->b.b[bL[1]];
	targetProgr = bT->d;

	//check if inverse
	std::list<int>::iterator it;
	it = p.p.end();
	it--;it--;
	
	if (bT->r[0] == *it)
		targetProgr = c->Length(bT->r[0], bT->r[1]) - targetProgr;
	//------------------------------------------------------------
	
	c->AddVList(nr, p.Prev(), p.Now(), lNr);

	changingLaneFrom = -1;		
}

point vehicle::Destination()
{
	point res = c->p[p.Now()].pos;

	if (p.Prev() != p.Next())
		res = c->SidePoint(p.Prev(), p.Now(), p.Next(), lNr);
	else
		res = c->p[p.Now()].pos;//CHANGE THAT IT WOULDN'T BE NEEDED

	return res;
}

	
void vehicle::UpdatePosition()
{
	pos = c->GetPointOnRoad(progr, p.Prev(), p.Now(), lNr);
}

void vehicle::UpdateRotation()
{
    k = Angle(Destination() - pos);
}


bool vehicle::Progress(double time)
{
	if (((p.p.size() == 3) && (progr >= targetProgr)) || (p.p.size() == 2))
	{
		//3 - end, drove over targetProgr
		//2 - jumped over targetProgr to end of road
		RemList();
		return 1;//end
	}

	
	double sNow = speed*(double(time)/1000);
	
	//lanes
	if (changingLaneFrom != -1)
	{
		//continue changing lane
		double now = sNow * 0.5;
		double left = 1 - changingLaneProgr;

		bool end = 0;

		if (now >= left)
		{
			now = left;
			end = 1;
		}

		changingLaneProgr += now;
		
		int rot;
		if (lNr > changingLaneFrom)
			rot = 90;
		else
			rot = -90;
		k += rot;
		Drive(now);
		k -= rot;

		if (end)
		{
			changingLaneFrom = -1;
		}
	}
	else
	{
		//look for better lane
		if (BetterAtLane(lNr - 1))
			ChangeLane(lNr - 1);
		else if (BetterAtLane(lNr + 1))
			ChangeLane(lNr + 1);
	}

	double dist = Length(pos, Destination());

	
	if (dist <= sNow)
	{
		Drive(dist);

		RemList();		
		p.Skip();		
				
		progr = c->GetRoadProgress(p.Prev(), p.Now(), pos);

		AddList();

		if (p.p.size() >= 3)
			UpdateRotation();

		//add speed control in here too
				
		if (speed != 0)
		{
			//reduce avail
			double leftDist = sNow - dist;	
			int leftTime = int((leftDist / (speed)) * 1000);
			
			return Progress(leftTime);
		}
	}
	else
	{
		Drive(sNow);
		
		double left;//distance to obstacle
		left = obs.d;

		if (c->Crossroad(p.Now()) && !c->LAvailable(p.Now(), p.Prev()))
		{
			double distToCrossing = c->DistanceToCrossroad(p.Prev(), p.Now(), pos) - vM->l;
			left = min(left, distToCrossing);
		}
				
		/*
		(0; 1) - Brake
		(1; 2) - nieko
		(2; +inf) - Accelerate
		*/
		const double bestDist = 0.5;
		const double warnDist = 1.5;

		if (left < bestDist)
		{
			//increase distance
			speed = 0;
			Brake(time);
		}
		else if (left < warnDist)
		{
			speed = obs.speed;
		}
		else if (left > bestDist)
		{
			Accelerate(time);
		}

	}
		

	return 0;
}


void vehicle::ChangeLane(int newLane)
{
	RemList();

	changingLaneFrom = lNr;
	changingLaneProgr = 0;
	lNr = newLane;

	AddList();
	//UpdatePosition();
}


void vehicle::AddList()
{
	c->AddVList(nr, p.Prev(), p.Now(), lNr);
}

void vehicle::RemList()
{
	c->RemVList(nr, p.Prev(), p.Now(), lNr);
}


void vehicle::Draw()
{
	double rot;//rotation of changing lanes
	if (changingLaneFrom == -1)
		rot = 0;
	else if (changingLaneFrom < lNr)
		rot = 20;
	else
		rot = -20;

	vM->m.Draw(pos, k + rot + 90);
}


void vehicles::Init(city *ct)
{
	c = ct;
		
	vM.Load(c_file_vehicle_models);
}

void vehicles::Clear()
{
	while (idL.size())
		Remove(idL.front());
}


const int maxD = 200;//maximal distance to obstacle(+/- field of view)

obstacle vehicles::RoadAvail(double start, int from, int to, int lane, bool inFront)
{
	obstacle obs;
	obs.d = maxD + 5;//+ start
	
	std::list<int> *l = &c->p[from].GetRoadTo(to)->v[lane].v;
	 

	std::list<int>::iterator it;
	for (it = l->begin(); it != l->end(); it++)
	{
		double now;
		double nowL = v[*it].vM->l;


		//now = v[*it].progr - nowL;
		//now -= start;
		
		now = v[*it].progr - start;

		if (!inFront)
		{
			now *= -1;
		}

		now += nowL;

		if (now > 0.1)
		{
			now -= 2*nowL;

			if (now < obs.d)
			{
				obs.d = now;
				obs.speed = v[*it].speed;
			}
		}
	}


	return obs;
}

obstacle vehicles::VehicleObstacleBehind(int nr, int lane)
{
	if (lane == -1)
		lane = v[nr].lNr;
	//if lane == v[nr].lNr, then obs.d == 0

	double l = 2*v[nr].vM->l;
		
	obstacle o = RoadAvail(v[nr].progr + l, v[nr].p.Prev(), v[nr].p.Now(), lane, 0);
	o.d -= l;
	
	return o;
}

obstacle vehicles::VehicleObstacle(int nr, int lane)
{
	if (lane == -1)
		lane = v[nr].lNr;
	//progress
	double currentProgr = v[nr].progr;//to be in front
	double startProgr = 0;//for others segments
		
	int from, to;	

	obstacle obs;

	std::list<int>::iterator it;//to

	it = v[nr].p.p.begin();
	to = *it;
	it++;

	while ((startProgr < maxD) && (it != v[nr].p.p.end()))
	{
		from = to;
		to = *it;
		it++;

		obs = RoadAvail(currentProgr + v[nr].vM->l, from, to, lane);
		
		if (obs.d <= maxD)
			return obs;//found first car
		
		double length = c->Length(from, to);
		startProgr += length;
		currentProgr -= length;//minProgr only needed for first segment
	}
	
	obstacle oEmpty;
	oEmpty.d = maxD;
	oEmpty.speed = 0;//can be something stoped in there, driver can't see

	return oEmpty;//no cars in maxD
}

void vehicles::Progress(double time)
{
	for (int i = 0; i < v.size(); i++)
		if (!availID[i])
		{
			v[i].obs = VehicleObstacle(v[i].nr);

			if (v[i].Progress(time))
				Remove(i);
		}	
}


//statistics
double vehicles::AverageSpeed()
{
	double a = 0;

	std::list<int>::iterator it;
	for (it = idL.begin(); it != idL.end(); it++)
		a += v[*it].speed;

	if (v.size())
		a /= v.size();

	return a;
}

double vehicles::AverageMaxSpeed()
{
	double a = 0;

	std::list<int>::iterator it;
	for (it = idL.begin(); it != idL.end(); it++)
		a += v[*it].speed / v[*it].vM->maxSpeed;
	
	if (idL.size())
		a /= idL.size();

	return a;
}

int vehicles::StoppedVehicles()
{
	int count = 0;

	std::list<int>::iterator it;
	for (it = idL.begin(); it != idL.end(); it++)
		if (v[*it].speed < 0.01)
			count++;

	return count;
}


void vehicles::Draw()
{
	std::list<int>::iterator it;
	for (it = idL.begin(); it != idL.end(); it++)
			v[*it].Draw();
}



void vehicles::Add(int vMNr, int bL[2])
{
	vehicle *vNow;
	vNow = new vehicle;

	int id = GetFreeID();

	vNow->Init(id, c, this, &vM.vM[vMNr], bL);

	if (id >= v.size()) 
	{
		v.push_back(*vNow);
	}
	else
	{
		v[id] = *vNow;//already created
	}

	idL.push_back(id);
}

std::list<int>::iterator vehicles::GetIDLTo(int to)
{
	std::list<int>::iterator it;
	for (it = idL.begin(); it != idL.end(); it++)
		if (*it == to)
			return it;

	return idL.end();
}

void vehicles::Remove(int id)
{
	//remove from list

	idL.erase(GetIDLTo(id));

	availID[id] = 1;
}

void vehicles::ReleaseID(int id)
{
	availID[id] = 1;
}

int vehicles::GetFreeID()
{
	for (int i = 0; i < availID.size(); i++)
		if (availID[i])
		{
			availID[i] = 0;
			return i;
		}

	//not found, create new ID
	bool newID = 0;
	availID.push_back(newID);

	return availID.size() - 1;
}


int vehicles::GetVehiclesNr()
{
	return idL.size();
}