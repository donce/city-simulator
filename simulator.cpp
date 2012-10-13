#include "simulator.h"

#include "const.h"
#include <fstream>


#include "vModels.h"


void simulator::Progress(double time, bool editor) 
{	
	if (!editor)
	{
		v.Progress(time);
		c.r.Progress(time);
	}

	c.Progress(time, editor);
}


bool simulator::Init()
{	
	if (c.Init())
		return 1;
	

	//vM

	v.Init(&c);

	c.r.Init(c_file_routes, &v);

	return 0;
}

void simulator::Deinit()
{
	//if () DeinitSim();
	v.Clear();
	c.Clear();
}

void simulator::InitSim()
{

}

void simulator::DeinitSim()
{
	v.Clear();
}

int simulator::GetVNr()
{
	using namespace std;
	ifstream f(c_file_simulation);

	int vNr;
	f >> vNr;

	f.close();

	return vNr;
}



bool simulator::LoadMap()
{
	c.Load();
	
	return 0;
}


void simulator::Draw()
{
	c.Draw();
	v.Draw();
}