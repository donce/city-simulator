#include "ui.h"

#include "const.h"

#include "simulator.h"


//-----------------------------------------------------------------------------


void ui::ChangeMode(bool newMode)
{
	if (newMode == mode)
		return;

	mode = newMode;
	
	tEditorActions->ToggleVisibility(!mode);
	tSimulatorActions->ToggleVisibility(mode);
	//tMainMenu->ToggleVisibility(

	if (mode)
	{
		//turn to simulator
		s->InitSim();		

	}
	else
	{
		//turn to editor
		s->DeinitSim();

	}

}


void ui::ChangeScreen(int x, int y)
{		
	point p(x, y);
	g.ChangeScreen(p);
}


void ui::InitGUI()
{
	escapePressed = keys[VK_ESCAPE];


	table *t;
	button *b;
	label *l;
	
		
	rect r(0.05, 0.1, 0.45, 0.5);	
	rect br(0.05, 0.05, 0.2, 0.1);
	

	
	
	//-------------------------------------
	//main menu
	t = new table(rect(0.3, 0.3, 0.6, 0.6), 0);
	tMainMenu = t;
	t->title = "Main menu";
	
	rect mMenuButton(0.01, 0.05, 0.21, 0.09);

	//save
	b = new button("Save", mMenuButton);
	//b->PressCall = &ToggleSettings;
	t->Add(*b);

	

	//settings
	mMenuButton += Point(0, 0.05);
	b = new button("Settings", mMenuButton);
	b->PressCall = &ToggleSettings;
	t->Add(*b);
	
	//quit
	mMenuButton += Point(0, 0.05);
	b = new button("Quit", mMenuButton);
	b->PressCall = &::QuitProgram;//&ToggleQuit;
	t->Add(*b);

	g.Add(*t);
	//-------------------------------------
	/*
	t = new table(rect(0.25, 0.3, 0.65, 0.6), 1);
	t->title = "Confirmation";
	
	l = new label("Are you sure you want to quit?", point(0.01, 0.01));
	t->Add(*l);
	
	b = new button("Yes", rect(0.01, 0.11, 0.09, 0.19));
	t->Add(*b);

	b = new button("No", rect(0.11, 0.11, 0.19, 0.19));
	t->Add(*b);
	
	tQuit = t;
	g.Add(*t);
	*/
	//-------------------------------------
	//selecting mode
	rect rMainBar(0, 0, 0.2, 0.1);

	rect rBarButton(0.01, 0.01, 0.09, 0.09);

	t = new table(rMainBar);
	//t->title = "Menu";
	
	b = new button("Edit", rBarButton);
	b->tex = LoadTexture("data/tex/gui/buttons/editor.bmp");
	b->PressCall = ModeEditor;
	t->Add(*b);

	rBarButton += Point(0.1, 0);
	
	b = new button("Simulate", rBarButton);
	b->tex = LoadTexture("data/tex/gui/buttons/simulator.bmp");
	b->PressCall = ModeSimulator;
	t->Add(*b);

	g.Add(*t);
	//-------------------------------------
	rect rActions(0.2, 0, 1, 0.1);
	//editor actions		

	rBarButton = rect(0.01, 0.01, 0.09, 0.09);		

	t = new table(rActions, 1);
	
	b = new button("Vertexes", rBarButton);//and lights
	b->PressCall = EditorVertexes;
	t->Add(*b);
		
	rBarButton += Point(0.1, 0);
	b = new button("Roads", rBarButton);
	b->tex = LoadTexture("data/tex/gui/buttons/roads.bmp");
	b->PressCall = EditorRoads;
	t->Add(*b);
	
	rBarButton += Point(0.1, 0);
	b = new button("Buildings", rBarButton);
	b->tex = LoadTexture("data/tex/gui/buttons/buildings.bmp");
	b->PressCall = EditorBuildings;
	t->Add(*b);
	
	rBarButton += Point(0.1, 0);
	b = new button("Routes", rBarButton);
	b->tex = LoadTexture("data/tex/gui/buttons/routes.bmp");
	b->PressCall = EditorRoutes;
	t->Add(*b);

	tEditorActions = t;
	g.Add(*t);
	//-------------------------------------
	//simulator actions	
	t = new table(rActions, 0);

	rBarButton = rect(0.01, 0.01, 0.09, 0.09);
	
	
	//time
	l = new label("Time 05:01", Point(0.01, 0.05));
	t->Add(*l);

	//time control
	b = new button("Time control", rBarButton);
	b->PressCall = &ToggleTimeControl;
	t->Add(*b);
	
	//view
	rBarButton += Point(0.1, 0);
	b = new button("View", rBarButton);
	b->PressCall = &ToggleView;
	t->Add(*b);

	//statistics
	rBarButton += Point(0.1, 0);
	b = new button("Statistics", rBarButton);
	b->PressCall = &ToggleStatistics;
	t->Add(*b);

	//test
	rBarButton += Point(0.1, 0);
	b = new button("Test", rBarButton);
	b->PressCall = &ToggleTest;
	t->Add(*b);
	
	tSimulatorActions = t;
	g.Add(*t);	
	//-------------------------------------
	//time control
	r += Point(0.2, 0.1);
	
	rect rTimeControlTable(0, 0, 0.31, 0.11);
	rTimeControlTable += Point(0.1, 0.1);

	rect rDec2(0.01, 0.01, 0.05, 0.05);
	rect rDec1(0.06, 0.01, 0.10, 0.05);
	
	rect rNormal(0.11, 0.01, 0.20, 0.05);

	rect rInc1(0.21, 0.01, 0.25, 0.05);
	rect rInc2(0.26, 0.01, 0.30, 0.05);
	
	rect rFreeze(0.01, 0.06, 0.10, 0.10);

	//time table
	t = new table(rTimeControlTable, 0);	
	t->title = "Time control";
	//decrease 2
	b = new button("<<", rDec2);
	b->PressCall = TimeDec2;
	t->Add(*b);
	//decrease 1
	b = new button("<", rDec1);
	b->PressCall = TimeDec1;
	t->Add(*b);
	//normal
	b = new button("Normal", rNormal);
	b->PressCall = TimeNormal;
	t->Add(*b);
	//increase 1
	b = new button(">", rInc1);
	b->PressCall = TimeInc1;
	t->Add(*b);
	//increase 2
	b = new button(">>", rInc2);
	b->PressCall = TimeInc2;
	t->Add(*b);

	//freeze
	b = new button("Freeze", rFreeze);
	b->PressCall = TimeFreeze;
	t->Add(*b);

	//tC
	l = new label("EMPTY", Point(0.11, 0.10)); 
	tCS = &l->title;
	t->Add(*l);	
	
	tTimeControl = t;
	g.Add(*t);		
	//-------------------------------------
	//view
	rect rView(0, 0, 0.6, 0.11);
	rView += Point(0.1, 0.1);
	rect rExplore(0.01, 0.01, 0.20, 0.05);
	rect rTrack(0.21, 0.01, 0.40, 0.05);
	rect rTrackAutoRotation(0.21, 0.06, 0.40, 0.10);

	t = new table(rView, 0);
	t->title = "View";
	//Explore
	b = new button("Explore", rExplore);
	b->PressCall = Explore;
	t->Add(*b);
	//Track
	b = new button("Track vehicle", rTrack);
	b->PressCall = TrackCar;
	t->Add(*b);
	//Auto rotation
	b = new button("Auto rotation", rTrackAutoRotation);
	b->PressCall = ToggleTrackAutoRotation;
	t->Add(*b);
	//ID
	l = new label("Exploring", Point(0.41, 0.05));
	sView = &l->title;
	t->Add(*l);
	
	tView = t;
	g.Add(*t);
	//-------------------------------------
	//settings
	rect rShaders(0.01, 0.01, 0.15, 0.05);

	t = new table(r, 0);
	t->title = "Settings";
	//Shaders
	b = new button("Shaders", rShaders);
	b->PressCall = ToggleShaders;
	t->Add(*b);

	g.Add(*t);
	tSettings = t;
	//-------------------------------------
	//statistics
	rect rStatistics(0, 0, 0.7, 0.7);
	rStatistics += Point(0.1, 0.1);

	t = new table(rStatistics, 0);
	t->title = "Statistics";
	//cars
	l = new label("Vehicles", Point(0.01, 0.05));
	t->Add(*l);
	//cars' number
	//sprintf(buf, "Vehicles' number: %d", s.v.v.size());
	l = new label("empty", Point(0.06, 0.10));
	sNumberOfVehicles = &l->title;
	t->Add(*l);
	//stopped cars
	l = new label("Stopped vehicles: 20.00%%", Point(0.06, 0.15));
	t->Add(*l);
	//cars' speed
	l = new label("Vehicles' speed", Point(0.06, 0.20));
	t->Add(*l);
	//avarage speed
	l = new label("Average speed: 12.00(20.00)", Point(0.11, 0.25));
	sAvSpeed = &l->title;
	t->Add(*l);
	//maximum speed
	l = new label("Maximum speed: 30.00%%(35.00%%)", Point(0.11, 0.30));
	sMaxSpeed = &l->title;
	t->Add(*l);
	//limits limit
	l = new label("Limits' speed: 80.00%%(85.00%%)", Point(0.11, 0.35));
	sLimitsSpeed = &l->title;
	t->Add(*l);

	//roads
	l = new label("Roads", Point(0.01, 0.40));
	t->Add(*l);
	//points
	sprintf(buf, "Points: %d", s->c.p.size());
	l = new label(buf, Point(0.06, 0.45));
	t->Add(*l);
	//segments
	sprintf(buf, "Segments: %d", 123);
	l = new label(buf, Point(0.06, 0.50));
	t->Add(*l);
	//used segments
	l = new label("Used segments: 20.00%(21.00%)", Point(0.06, 0.55));
	sUsedSegments = &l->title;
	t->Add(*l);
			
	tStatistics = t;	
	g.Add(*t);
	//-------------------------------------
	//test
	t = new table(r, 0);
	t->title = "Test";
	//Beep
	b = new button("Test beep", br);
	//b->PressCall = MakeSound;
	b->PressCall = ::SaveCity;

	t->Add(*b);	
	
	tTest = t;
	g.Add(*t);
	//-------------------------------------


	UpdateTCS();

}

void ui::Init(simulator *sim, HDC &hDC)
{
	s = sim;
	e.Init(&s->c);

	tC = 1;
	mode = 0;
	InitGUI();


	//camera
	cDist = 6;
	camP = s->c.CityRect().Center();
	camA.x = 45;
	camA.y = 0;

	std::string s = c_dir_tex_main_path;

	g.Init(hDC, s + "gui/table.bmp",
				s + "gui/table_border.bmp",
				s + "gui/table_button.bmp");
	
	trackCar = 0;
	trackAutoRotation = 0;

	shaders = 0;

	quitProgram = 0;
}



void ui::MouseLeftDown()
{	
	if (g.Press(mp))
		return;

	mb[0] = 1;

	if (mode == 0)
	{
		e.MouseLeftDown(pointer);
	}
}


void ui::MouseLeftUp()
{
	mb[0] = 0;

	if (mode == 0)
	{
		e.MouseLeftUp(pointer);
	}

	g.Release();
}

void ui::MouseRightDown()
{
	mb[1] = 1;
}

void ui::MouseRightUp()
{
	mb[1] = 0;

}

void ui::MouseMove(int nx, int ny)
{
	point now(nx, ny);
	
	point d = now - mp;

	g.Move(d);

	if (mb[0])
		RotateCamera(d);

	if (mb[1])
	{
		if (!trackCar)
			MoveCamera(d);
	}
		
	mp = now;
}

void ui::MouseWheel(int d)
{
	Zoom(d);
}


void ui::MoveCamera(point p)
{
	double x, y;
	x = p.x * sqrt(cDist+0.1);
	y = p.y * sqrt(cDist+0.1);

	const double c_juda = 0.03;

	//forward/backward
	camP.x -= float(-sin(camA.y*3.14/180) * y * c_juda);
	camP.y -= float( cos(camA.y*3.14/180) * y * c_juda);

	//left/right
	camP.x -= float( cos(camA.y*3.14/180) * x * c_juda);
	camP.y -= float( sin(camA.y*3.14/180) * x * c_juda);

}

void ui::RotateCamera(point p)
{
	camA.x += p.y * c_mouse_speed;
	camA.y += p.x * c_mouse_speed;

	camA.x = min(90, camA.x);
	camA.x = max(0, camA.x);
}

void ui::Zoom(int d)
{	
	cDist += -float(d * c_mouse_wheel_speed);

	cDist = max(1, cDist);
}



bool ui::Progress(double time)
{	

	bool nowEscape = keys[VK_ESCAPE];
	if (nowEscape && !escapePressed)
		ToggleMainMenu();

	escapePressed = nowEscape;
		//return 1;
	
	//parameters
	for (int i = 0; i < 10; i++)
		if (keys['0'+i])
		{
			if (!btn[i])
			{
				par[i] ^= 1;
				btn[i] = 1;
			}
		}
		else
			btn[i] = 0;


	//if (g.visible != par[1])
		//g.visible = par[1];	


	
	s->Progress(time*tC, !mode);
	if (mode == 1)
		UpdateStatistics(time);

	e.Progress(pointer, time);

	if (quitProgram)
		return 1;

	return 0;
}


void ui::UpdateStatistics(double time)
{
	avSpeed.Add(s->v.AverageSpeed(), time);
	double now;
	now = s->v.AverageMaxSpeed()*100;
	avMaxSpeed.Add(now, time);
	

	//average speed
	sprintf(buf, "Average speed: %0.2f(%0.2f)", avSpeed.GetNow(), avSpeed.GetAverage());
	*sAvSpeed = buf;
	//maximum speed

	sprintf(buf, "Average max speed: %0.2f(%0.2f)", avMaxSpeed.GetNow(), avMaxSpeed.GetAverage());
	*sMaxSpeed = buf;

	sprintf(buf, "Number of vehicles: %d", s->v.GetVehiclesNr());
	*sNumberOfVehicles = buf;

	//limits' speed
	//sprintf(buf, "Limits' speed: 30.00%%(35.00%%)");
	//*sLimitsSpeed = buf;
	//used segments
	//sprintf(buf, "Used segments: %0.2f%%(%0.2f%%)", 20.1, 21.2);
	//*sUsedSegments = buf;
}

void ui::ChangeEditorMode(int newMode)
{
	e.ChangeMode(newMode);
}


void ui::SaveCity()
{
	s->c.SaveCity();
}


void ui::QuitProgram()
{
	quitProgram = 1;
}
