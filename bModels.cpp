#include "bModels.h"



void bModels::Load(char *filename)
{	
	std::ifstream f(filename);

	std::string name;
	//double length;

	//int n;
	f >> n;
	bM = new bModel[n];

	//model *tempM;


	for (int i = 0; i < n; i++)
	{
		f >> bM[i].length;
		f.ignore();
		f >> name;
				
		//tempM = new model;

		std::string s = "buildings/" + name + "/";


		bM[i].m.Open(s.c_str());
		//bModels.push_back(*tempM);
		//delete tempM;
	}
	

	f.close();
}