#ifndef INC_UICONTROL_H
#define INC_UICONTROL_H

#include "ui.h"

class ui;

extern std::string *tCS;//tc string

extern std::string *sView;
extern table *tTimeControl, *tView, *tSettings, *tStatistics, *tTest, *tMainMenu, *tEditorActions, *tSimulatorActions, *tQuit;

extern std::string *sAvSpeed, *sMaxSpeed, *sLimitsSpeed;
extern std::string *sNumberOfVehicles, *sUsedSegments;

void SendUI(ui *sUI);


void ToggleTimeControl();
void ToggleView();
void ToggleSettings();
void ToggleStatistics();
void ToggleTest();

void ToggleQuit();



void UpdateTCS();

//---------------------------------------------------------
//time
void Freeze();
void Unfreeze();
void TimeFreeze();
void TimeNormal();
void MultiplyTime(double k);

void TimeDec1();
void TimeDec2();
void TimeInc1();
void TimeInc2();
//---------------------------------------------------------

void Explore();
void TrackCar();
void ToggleTrackAutoRotation();
//---------------------------------------------------------
//shaders
void ToggleShaders();

//---------------------------------------------------------

void MakeSound();


void ToggleTimeControl();
void ToggleView();
void ToggleSettings();
void ToggleStatistics();
void ToggleTest();

//change main mode
void ModeEditor();
void ModeSimulator();

//change editor mode
void EditorRoads();
void EditorVertexes();
void EditorBuildings();
void EditorRoutes();


void SaveCity();


void ToggleMainMenu();

void QuitProgram();

#endif