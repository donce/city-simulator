#include "uiControl.h"

std::string *tCS;//tc string

std::string *sView;
table *tTimeControl, *tView, *tSettings, *tStatistics, *tTest, *tMainMenu, *tEditorActions, *tSimulatorActions, *tQuit;

std::string *sAvSpeed, *sMaxSpeed, *sLimitsSpeed;
std::string *sNumberOfVehicles, *sUsedSegments;

ui *u;


void SendUI(ui *sUI)
{
	u = sUI;
}

//statistics
//speeds
double avSpeedNow = 0, avSpeedAll = 0;







void UpdateTCS()
{
	sprintf(buf, "Time: %0.2f%%", u->tC);
	*tCS = buf;
}

bool frozen = 0;
double frozenAt;

void Freeze()
{
	frozen = 1;
	frozenAt = u->tC;	
	u->tC = 0;
}

void Unfreeze()
{
	frozen = 0;
	u->tC = frozenAt;
}

void TimeFreeze()
{
	if (!frozen)
	{
		Freeze();
		*tCS = "Time speed: frozen";
	}
	else
	{
		Unfreeze();
		UpdateTCS();
	}
}

void TimeNormal()
{
	if (frozen)
		Unfreeze();

	u->tC = 1;
	UpdateTCS();
}

void MultiplyTime(double k)
{
	if (frozen)
		Unfreeze();

	u->tC *= k;
	UpdateTCS();
}


void TimeDec1()
{
	MultiplyTime(1/1.2);
}

void TimeDec2()
{
	MultiplyTime(1.0/5);
}
void TimeInc1()
{
	MultiplyTime(1.2);
}
void TimeInc2()
{
	MultiplyTime(5);
}




void Explore()
{
	*sView = "Exploring";
	u->trackCar = 0;
}

void TrackCar()
{
	/*
	u->trackCar = 1;
	u->trackNr = rand() % s.v.v.size();

	sprintf(buf, "Tracking: %d", trackNr);
	*sView = buf;
	*/
}

void ToggleTrackAutoRotation()
{
	u->trackAutoRotation ^= 1;
}

void ToggleShaders()
{
	u->shaders ^= 1;
}

void MakeSound()
{
	Beep(300, 300);
	Beep(500, 300);
	Beep(800, 300);
}

void ToggleTimeControl()
{
	tTimeControl->ToggleVisibility();
}

void ToggleView()
{
	tView->ToggleVisibility();
}

void ToggleSettings()
{
	tSettings->ToggleVisibility();
}

void ToggleQuit()
{
	//Beep(500, 500);
	tQuit->ToggleVisibility();

}

void ToggleStatistics()
{
	tStatistics->ToggleVisibility();
}

void ToggleTest()
{
	tTest->ToggleVisibility();
}


void ModeEditor()
{
	u->ChangeMode(0);

}

void ModeSimulator()
{
	u->ChangeMode(1);

}



void EditorVertexes()
{
	u->ChangeEditorMode(1);
}

void EditorRoads()
{
	u->ChangeEditorMode(2);
}

void EditorBuildings()
{
	u->ChangeEditorMode(3);
}

void EditorRoutes()
{
	u->ChangeEditorMode(4);
}

void SaveCity()
{
	u->SaveCity();
}


void ToggleMainMenu()
{
	tMainMenu->ToggleVisibility();
}


void QuitProgram()
{
	u->QuitProgram();
}
