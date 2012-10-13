#ifndef INC_TEX_H
#define INC_TEX_H

#include <windows.h>// Header File For Windows

#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library

#include <stdio.h>//gal reikia
#include "bmp.h"

#include <string>


AUX_RGBImageRec *LoadBMP(const char *Filename);//Loads A Bitmap Image
GLuint LoadTexture(const char *filename);
GLuint LoadTexture(std::string &filename);


#endif