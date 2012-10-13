
#include "lights.h"
#include "const.h"

#include <fstream>

//#include <Windows.h>

#include <iostream>


//--------------------------------------------------------------------
//----------------------------LIGHT-----------------------------------
//--------------------------------------------------------------------
light::light()
{
	left = 0;
}

void light::Change(bool &b)
{
   b = !b;     
}


void light::Clean()
{
   green = 1;
   left = cTime[green];
}


int light::PairNr(int n)
{
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        if (r[i][j] == n)
          return i;
          
    return -1;//not found 
}


bool light::Available(int n)
{
   int pNr = PairNr(n);//pair nr
   
   if (pNr == -1)
     return 1;//not him
      
   bool avail = green;//available
   
   if (pNr)
     Change(avail);
     
   if (avail)
   {
      //how much until red
      if (left > c_yellow)
        return 1;//available
   }
   
   return 0;
}

void light::Progress(double time)
{
	left -= time;   

	while (left <= 0)
	{
		Change(green);
		left += cTime[green];
	}   
}
//--------------------------------------------------------------------
//----------------------------LIGHTS----------------------------------
//--------------------------------------------------------------------

int lights::Index(int lightNr)
{
    for (int i = 0; i < lNr; i++)
      if (iNr[i] == lightNr)
        return i;
        
    return 0;   
}    

bool lights::Available(int lightNr, int roadNr)
{
    if (!is[lightNr])
      return 1;
	
    return l[Index(lightNr)].Available(roadNr);     
}


void lights::Read(const char *filename)
{
   //read lights
   std::ifstream f(filename);
         
   f >> lNr;
   
   
   iNr = new int[lNr];
      
   l = new light[lNr];
   
   
   int i, j, k;
   int nr;
   
   for (i = 0; i < lNr; i++)
   {
      f >> nr;
      
      is[nr] = 1;       
      iNr[i] = nr;      
      
      for (j = 0; j < 2; j++)
        for (k = 0; k < 2; k++)
          f >> l[i].r[j][k];          
   }
   
   f.close();        
}


void lights::Create(int nr, const char *filename)
{     
   n = nr;
   
   is = new bool[n];
   
   int i;
   for (i = 0; i < n; i++)
     is[i] = 0;   
     
   Read(filename);
}


void lights::Progress(double time)
{
   for (int i = 0; i < lNr; i++)  //lNr
     l[i].Progress(time);     
}


int lights::Lights()
{
    return lNr;       
}




