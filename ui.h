#ifndef INC_UI_H
#define INC_UI_H

#include "gui.h"
#include "editor.h"

#include "average.h"

#include "uiControl.h"

class simulator;

class ui {
public:

	void QuitProgram();
	
	bool *keys;//private

	gui g;

	//camera
	point camP;//camera's position
	point camA;//camera's angle
	float cDist;//camera's distance
	
	double tC;
	
	void Init(simulator *s, HDC &hDC);
	

	void ChangeScreen(int x, int y);
	
	void MouseLeftDown();
	void MouseLeftUp();
	void MouseRightDown();
	void MouseRightUp();
	void MouseMove(int nx, int ny);
	void MouseWheel(int d);
		
	void MoveCamera(point p);
	void RotateCamera(point p);
	void Zoom(int d);


	bool Progress(double time);

	bool mb[2];//mouse buttons
	point mp;//mouse point
	
	point3 pointer;

	
	//track
	bool trackCar;
	int trackNr;
	bool trackAutoRotation;
	
	bool shaders;
	
	void ChangeMode(bool newMode);
	void ChangeEditorMode(int newMode);

	void SaveCity();

private:
	bool mode;//editor, simulator
	
	bool quitProgram;

	editor e;
	simulator *s;
	
	bool par[10];//parameters
	bool btn[10];//buttons

	void InitGUI();

	
	
	//statistics
	average avSpeed, avMaxSpeed;
	void UpdateStatistics(double time);

	bool escapePressed;


};




#endif