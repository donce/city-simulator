#ifndef INC_LIGHTS_H
#define INC_LIGHTS_H



//--------------------------------------------------------------------
//----------------------------LIGHT-----------------------------------
//--------------------------------------------------------------------
class light {
private :
	//0 point
	double left;
        
	void Change(bool &b);
        
	int PairNr(int n);

	bool green;
        
public :
	
	light();

	double cTime[2];//colors: red, green
	int r[2][2];//road pairs

	void Clean();

	bool Available(int n);

	void Progress(double time);


};
//---------------------------------------------------------------


//--------------------------------------------------------------------
//----------------------------LIGHTS----------------------------------
//--------------------------------------------------------------------
class lights {
  private : 
          
    void Read(const char *filename);
          
    int n;//vertexes nr
    int lNr;//lights nr
    
    
  public :      

    bool *is;

    int *iNr;//index to light nr   
    light *l;
  
    int Lights();
    
    int Index(int lightNr);
       
    void Create(int nr, const char *filename);
    void Progress(double time);
    bool Available(int lightNr, int roadNr);

    //void RandomParameters();
};
//---------------------------------------------------------------
















#endif
