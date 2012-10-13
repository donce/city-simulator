#include "buildings.h"

#include <Windows.h>

#include <gl\gl.h>
#include <gl\glu.h>	



building::building()
{
	bM = 0;
}


void buildings::LoadModels(char *filename)
{
	bM.Load(filename);

	/*
	std::ifstream f(filename);

	std::string name;
	double length;

	int n;
	f >> n;

	model *tempM;

	for (int i = 0; i < n; i++)
	{
		f >> length;
		f.ignore();
		f >> name;
				
		tempM = new model;

		std::string s = "buildings/" + name + "/";

		tempM->Open(s.c_str());
		bModels.push_back(*tempM);
		delete tempM;
	}
	

	f.close();
	*/
}


void buildings::LoadBuildings(char *filename)
{
	std::ifstream f(filename);
	
	int nr;
	f >> nr;
	building temp;
	int mNr;//model nr
	for (int i = 0; i < nr; i++)
	{
		f >> temp.r[0] >> temp.r[1] >> temp.d >> mNr;
		temp.bM = &bM.bM[mNr];
		b.push_back(temp);
	}

	f.close();
}

int buildings::Load(char *filenameModels,char *filenameBuildings)
{

	
	LoadBuildings(filenameBuildings);
	
	return 0;
}

void buildings::Clear()
{
	b.clear();
}

/*
void buildings::Draw()
{
	for (int i = 0; i < b.size(); i++)
	{
		int ra, rb;
		ra = b[i].r[0];
		rb = b[i].r[1];

		point pa, pb, p;
		
		pa = c->p[ra].pos;		
		pb = c->p[rb].pos;

		double l = b[i].d / Length(pa, pb);

		p = pa + ((pb - pa)*l);

		point side;
		side = pb - pa;
		
		side.Norm();
		side *= c->LanesNr(ra, rb);

		side.Rotate();
		p += side;
		
		double k = Angle(pa, pb);


		//glTranslated(p.x, 0, p.y);
		//glRotated(k, 0, -1, 0);

		bModels[b[i].mNr].Draw(p, k);
		
		//glRotated(k, 0, 1, 0);
		//glTranslated(-p.x, 0, -p.y);

	}
}
*/