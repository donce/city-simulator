#ifndef INC_FONT_H
#define INC_FONT_H

#include <windows.h>

#include <gl\gl.h>		
#include <gl\glu.h>		

#include <stdio.h>

class font {
public:
	void Init(HDC hDC);
	void Clear();

	GLvoid KillFont(GLvoid);
	GLvoid glPrint(const char *fmt, ...);

private:
	
	GLuint	base;// Base Display List For The Font Set

	
	GLvoid BuildFont(HDC hDC);
};

//font f;



#endif