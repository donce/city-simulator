#ifndef INC_GUI_H
#define INC_GUI_H

#include "common.h"

#include <vector>
#include <list>

#include <string>


#include <windows.h>

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "font.h"


#include "tex.h"




class button {
public:
	void (*PressCall)();
	GLuint tex;

	bool Press(point p);

	void Draw();
	
	button(std::string t, rect a);
	
private:
	std::string title;
	rect r;

};

class label {
public:
	void Draw();
	//void Press(point p);
	label(std::string t, point a)
	{
		title = t;
		p = a;
	}
	

	std::string title;

private:
	point p;


};

class table {
public:

	void Draw();
	void Add(button &a);
	void Add(label &a);

	int Press(point p);
	
	void MoveTable(point d);

	//void Show();
	//void Hide();
	void ToggleVisibility(int _visible = -1);

	table(rect _r, bool v = 1);

	//bool Visible();

	//font *f;
	
	std::string title;//pri9vate

private:

	bool visible;

	rect r;
	
	rect TopRect();
	
	std::vector<button*> b;
	std::vector<label*> l;
};

class gui {
public:

	bool visible;

	void Draw();

	void Add(table &a);
	
	void Init(HDC &hDC, std::string &tableTex, std::string &tableBorderTex, 
		      std::string &tableButtonTex);

	//void Init(HDC hDC, char *tableTex, char *tableBorderTex, char *tableButtonTex);
	void Clear();
	
	void ChangeScreen(point p);
	
	bool Press(point p);
	void Release();
	void Move(point d);
	
	point screen;
	
	//std::vector<table> t;//private
	std::list<table*> t;

private:

	bool dragging;

	//font f;

};


#endif