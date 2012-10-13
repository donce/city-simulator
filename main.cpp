#include <windows.h>

#include <gl\gl.h>
#include <gl\glu.h>

#include "graphics.h"

#include <vector>
#include <math.h>

#include "model.h"

#include "common.h"
#include "const.h"

#include "simulator.h"
//#include "gui.h"
#include "ui.h"

//#include "average.h"


#ifndef CDS_FULLSCREEN											// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4										// Compilers. By Defining It This Way,
#endif															// We Can Avoid Errors

GL_Window*	g_window;
Keys*		g_keys;



simulator s;

ui u;


model mPointer, mEmpty;





void ChangeScreen(int x, int y)
{
	u.ChangeScreen(x, y);
}

//------------------------------------------------------------------


//------------------------------------------------------------------




//bool selectVertex = 0;
//int selectVertexNr;

void MouseLeftDown()
{	
	u.MouseLeftDown();
}

void MouseLeftUp()
{
	u.MouseLeftUp();
}

void MouseRightDown()
{
	u.MouseRightDown();
}

void MouseRightUp()
{
	u.MouseRightUp();
}

void MouseMove(int nx, int ny)
{
	u.MouseMove(nx, ny);
}

void MouseWheel(int d)
{
	u.MouseWheel(d);
}



bool LoadWorld()
{
	//models	
	mPointer.Open("others/pointer");
	mEmpty.Open("others/empty");
		
	return 0;
}


BOOL Initialize(GL_Window* window, Keys* keys)
{
	srand(GetTickCount());
	
	g_window = window;
	g_keys = keys;
	
	u.keys = keys->keyDown;

	//textures
	glEnable(GL_TEXTURE_2D);// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);


	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);// Black Background
	glClearDepth(1.0f);// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);									// The Type Of Depth Testing (Less Or Equal)
	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glShadeModel(GL_SMOOTH);									// Select Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Set Perspective Calculations To Most Accurate
		
	GLfloat LightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};//Ambient Light Values ( NEW )
	GLfloat LightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};//Diffuse Light Values ( NEW )
	GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};//Light Position ( NEW )
	

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);//Setup The Ambient Light	
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);//Setup The Diffuse Light		
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);//Position The Light
	
	glEnable(GL_LIGHT1);//Enable Light One

	glEnable(GL_LIGHTING);//Enable Lighting
	

	LoadWorld();
	s.Init();
	s.LoadMap();

	SendUI(&u);
	u.Init(&s, window->hDC);
	
	//BuildFont(window->hDC);
	
	return 1;												// Return TRUE (Initialization Successful)
}

void Deinitialize(void)										// Any User DeInitialization Goes Here
{
	s.Deinit();
	//u.Clear();
	//u.g.Clear();
	//KillFont();
}

void Progress(double time)
{
	if (u.Progress(time))
		TerminateApplication(g_window);

}

point3 GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;


	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	point3 p;
	p.x = posX;
	p.y = posY;
	p.z = posZ;

	return p;
}

void DrawWorld()
{
	s.Draw();
}

void DrawForEditor()
{
	for (int i = 0; i < s.c.p.size(); i++)
	{
		mEmpty.Draw(s.c.p[i].pos, 0);
	}
	
}


void Setup3dWorld()
{	
	point camA0;
	camA0 = u.camA;
	if (u.trackCar && u.trackAutoRotation)
		camA0.y += s.v.v[u.trackNr].k;
		
	point pos;
	if (u.trackCar)
		pos = s.v.v[u.trackNr].pos;
	else	
		pos = u.camP;
	
	glTranslatef (0.0f, 0.0f, -u.cDist);	

	glRotatef (camA0.x, 1.0f, 0.0f, 0.0f);					
	glRotatef (camA0.y, 0.0f, 1.0f, 0.0f);					
	
	glTranslatef(0, -0.5, 0);//rise up		
	glTranslatef(-pos.x, 0, -pos.y);
	
	if (u.shaders)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);			
}


int GetSelectedBuilding()
{
	Setup3dWorld();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);

	s.c.DrawBuildings(1);
	GLubyte px;
	glReadPixels(int(u.mp.x + 0.5), int(768 - u.mp.y - 0.5), 1, 1, GL_RED, GL_UNSIGNED_BYTE, (GLvoid*)&px);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	
	glColor3f(1, 1, 1);

	return px - 1;
}

void Draw3d()
{
	Setup3dWorld();

	DrawWorld();	
	//pointer
	u.pointer = GetOGLPos(int(u.mp.x), int(u.mp.y));
	mPointer.Draw(u.pointer, 0);
}

void DrawGUI()
{
	glDisable(GL_LIGHTING);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	glColor3f(1, 1, 1);

	
	u.g.Draw();
	
	//mouse
	point dm;//draw mouse
	
	dm = u.mp;
	dm.x /= u.g.screen.y;
	dm.y /= u.g.screen.y;

	glColor3d(0.5, 0.7, 0.5);

	glBegin(GL_TRIANGLES);	
		glVertex2d(dm.x, dm.y);
		glVertex2d(dm.x+0.02, dm.y);
		glVertex2d(dm.x, dm.y+0.02);
	glEnd();

	glColor3d(1.0f, 1.0f, 1.0f);
}

void Draw()
{	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity ();		
	s.c.selectedBuilding = GetSelectedBuilding();
	glFlush ();	

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity ();		
	

	glPushMatrix();	
	Draw3d();
	glPopMatrix();
	

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//gluOrtho2D(0, 1, 1, 0);
	
	double lx = u.g.screen.x / u.g.screen.y;
	glOrtho(0, lx, 1, 0, -1, 1);


	glMatrixMode(GL_MODELVIEW);
	
	DrawGUI();

	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	glFlush ();													// Flush The GL Rendering Pipeline
	
}
