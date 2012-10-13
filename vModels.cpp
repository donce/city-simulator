#include "vModels.h"

#include <fstream>


void vModels::Load(char *filename)
{
	std::ifstream f(filename);

	f >> n;
	vM = new vModel[n];

	std::string s, path;
	
	for (int i = 0; i < n; i++)
	{
		f >> s >> vM[i].a >> vM[i].b >> vM[i].maxSpeed >> vM[i].l;

		id[s] = i;
		path = "vehicles/" + s + "/";
		vM[i].m.Open(path.c_str());
	}

	f.close();
}

int vModels::GetID(std::string name)
{
	return id[name];
}

int vModels::GetN()
{
	return n;
}
