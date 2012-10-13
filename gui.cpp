#include "gui.h"

#include "const.h"

static font f;
static GLuint tTable, tTableBorder, tTableButton;


bool Inside(rect r, point p)
{
		return (p.x >= r.p[0].x && p.x <= r.p[1].x && 
				p.y >= r.p[0].y && p.y <= r.p[1].y);
}


void DrawRect(rect r)
{
	glBegin(GL_QUADS);	

		glVertex2d(r.p[0].x, r.p[0].y);
		glVertex2d(r.p[1].x, r.p[0].y);
		glVertex2d(r.p[1].x, r.p[1].y);
		glVertex2d(r.p[0].x, r.p[1].y);

	glEnd();
}

void DrawTexRect(rect r)
{
	glBegin(GL_QUADS);	

		glTexCoord2d(0, 1);
		glVertex2d(r.p[0].x, r.p[0].y);
		
		glTexCoord2d(1, 1);
		glVertex2d(r.p[1].x, r.p[0].y);
		
		glTexCoord2d(1, 0);
		glVertex2d(r.p[1].x, r.p[1].y);
		
		glTexCoord2d(0, 0);
		glVertex2d(r.p[0].x, r.p[1].y);

	glEnd();
}

//----------------------------------------------------
//label
void label::Draw()
{
	glColor3f(0.0f, 0.0f, 0.0f);

	glRasterPos2d(p.x, p.y);
	f.glPrint(title.c_str());

	glColor3f(1.0f, 1.0f, 1.0f);
}

/*
void label::Press(point p)
{
	if (Inside(r, p))
		PressCall();
}
*/
//----------------------------------------------------
//button

button::button(std::string t, rect a)
{
	title = t;
	r = a;

	PressCall = 0;
	tex = 0;
}

void button::Draw()
{
	//frame
	
	glColor3f(1.0f, 1.0f, 1.0f);
	//rect re(0.1, 0.1, 0.3, 0.3);
	DrawTexRect(r);

	//title
	if (!tex)
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos2d(r.p[0].x, r.p[1].y);
		f.glPrint(title.c_str());

		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

bool button::Press(point p)
{
	if (Inside(r, p))
	{
		//Beep(500, 300);
		if (PressCall)
			PressCall();
	
		return 1;
	}

	return 0;
}


//----------------------------------------------------
//table

void glTranslate(point p)
{
	glTranslated(p.x, p.y, 0);
}

table::table(rect _r, bool v)
{
	title = "";
	r = _r;
	visible = v;
}

void table::Draw()
{

	if (!visible)
		return;
	
	rect tr = TopRect();
	
	glColor3f(1.0f, 1.0f, 1.0f);
	

	//frame
	glBindTexture(GL_TEXTURE_2D, tTable);
	glBegin(GL_QUADS);

	glTexCoord2d(0, 0);	glVertex2d(r.p[0].x, r.p[0].y);
	glTexCoord2d(1, 0);	glVertex2d(r.p[1].x, r.p[0].y);
	glTexCoord2d(1, 1);	glVertex2d(r.p[1].x, r.p[1].y);
	glTexCoord2d(0, 1);	glVertex2d(r.p[0].x, r.p[1].y);
	
	glEnd();
	
	if (title.length())
	{
		//top(border)
		glBindTexture(GL_TEXTURE_2D, tTableBorder);
		glBegin(GL_QUADS);
	
		glTexCoord2d(0, 0); glVertex2d(tr.p[0].x, tr.p[0].y);
		glTexCoord2d(1, 0);	glVertex2d(tr.p[1].x, tr.p[0].y);
		glTexCoord2d(1, 1);	glVertex2d(tr.p[1].x, tr.p[1].y);
		glTexCoord2d(0, 1);	glVertex2d(tr.p[0].x, tr.p[1].y);

		glEnd();

		//title
		glBindTexture(GL_TEXTURE_2D, 0);
		glRasterPos2d(r.p[0].x, r.p[0].y);
		f.glPrint(title.c_str());	
	}


	glTranslate(r.p[0]);

	
	//buttons
	for (int i = 0; i < b.size(); i++)
	{
		if (b[i]->tex)
			glBindTexture(GL_TEXTURE_2D, b[i]->tex);
		else
			glBindTexture(GL_TEXTURE_2D, tTableButton);
		
		b[i]->Draw();
	}

	//labels	
	glBindTexture(GL_TEXTURE_2D, 0);	
	for (int i = 0; i < l.size(); i++)
		l[i]->Draw();


	glTranslate(-r.p[0]);
}


void table::Add(label &a)
{
	l.push_back(&a);
}

void table::Add(button &a)
{
	b.push_back(&a);	
}

/*
void table::Show()
{
	visible = 1;
}

void table::Hide()
{
	visible = 0;
}
*/

void table::ToggleVisibility(int _visible)
{
	if (_visible == -1)
		_visible = !visible;


	if (_visible != visible)
	{
		//change
		visible = _visible;
	}
}


int table::Press(point p)
{
	if (!visible)
		return 0;
	
	rect tr = TopRect();

	if (Inside(tr, p))
	{
		//Beep(800, 200);

		return 2;
	}
	else if (Inside(r, p))
	{
		//Beep(500, 300);

		//check all items

		//buttons

		bool pressed = 0;
		for (int i = 0; (i < b.size()) && (!pressed); i++)
			if (b[i]->Press(p-r.p[0]))
				pressed = 1;			

		return 1;

	}

	return 0;
}


rect table::TopRect()
{
	rect tr;//title's rect

	tr.p[0].x = r.p[0].x;
	tr.p[1].x = r.p[1].x;

	tr.p[0].y = r.p[0].y-c_border_height;
	tr.p[1].y = r.p[0].y;

	return tr;	
}


void table::MoveTable(point d)
{
	r += d;

}


/*
bool table::Visible()
{
	return visible;
}
*/
//----------------------------------------------------
//gui

void gui::Draw()
{
	if (!visible)
		return;

	//tables
	
	std::list<table*>::reverse_iterator it;
	for (it = t.rbegin(); it != t.rend(); it++)
		(*it)->Draw();
		

}


void gui::Init(HDC &hDC, std::string &tableTex, std::string &tableBorderTex, 
	           std::string &tableButtonTex)
{
	tTable = LoadTexture(tableTex);
	tTableBorder = LoadTexture(tableBorderTex);
	tTableButton = LoadTexture(tableButtonTex);


	f.Init(hDC);

	dragging = 0;
	visible = 1;
}

void gui::Clear()
{
	f.Clear();
}

void gui::Add(table &a)
{
	t.push_back(&a);
}

bool gui::Press(point p)
{
	if (!visible)
		return 0;
	
	p.x /= screen.y;
	p.y /= screen.y;


	bool pressed = 0;
	int res;

	std::list<table*>::iterator it;

	for (it = t.begin(); it != t.end(); it++)
	//for (int i = 0; (i < t.size()) && (!pressed); i++)
	{
		res = (*it)->Press(p);
		if (res)
		{

			//bring to front
			t.push_front(*it);
			t.erase(it);

			pressed = 1;

			if (res == 2)
				dragging = 1;//start dragging

			return 1;
		}
	}

	return 0;
}


void gui::ChangeScreen(point p)
{
	screen = p;
}



void gui::Move(point d)
{
	d.x /= screen.y;
	d.y /= screen.y;

	if (dragging)
		t.front()->MoveTable(d);
}


void gui::Release()
{
	dragging = 0;
}