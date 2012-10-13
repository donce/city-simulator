#ifndef INC_MODEL_H
#define INC_MODEL_H


#include <windows.h>	// Header File For Windows

#include <gl\gl.h>	// Header File For The OpenGL32 Library
#include <gl\glu.h>	// Header File For The GLu32 Library

#include <stdio.h>

#include "common.h"

#include "tex.h"

typedef float vec3_t[3];

//MD2 header 
struct md2_header_t
{
public:
	int ident;
	int version;

	int skinwidth;
	int skinheight;

	int framesize;

	int num_skins;
	int num_vertices;
	int num_st;
	int num_tris;
	int num_glcmds;
	int num_frames;

	int offset_skins;
	int offset_st;
	int offset_tris;
	int offset_frames;
	int offset_glcmds;
	int offset_end;
};

//Texture name 
struct md2_skin_t
{
	char name[64];
};

//Texture coords 
struct md2_texCoord_t
{
	short s;
	short t;
};

//Triangle info 
struct md2_triangle_t
{
	unsigned short vertex[3];
	unsigned short st[3];
};

//Compressed vertex
struct md2_vertex_t
{
	unsigned char v[3];
	unsigned char normalIndex;
};

//model frame
struct md2_frame_t
{
	vec3_t scale;
	vec3_t translate;
	char name[16];
	struct md2_vertex_t *verts;
};

//GL command packet
struct md2_glcmd_t
{
	float s;
	float t;
	int index;
};

//MD2 model structure


//Table of precalculated normals 
/*
vec3_t anorms_table[162] = {
#include "anorms.h"
};
*/

class model {
private:	

	struct md2_header_t header;
	struct md2_skin_t *skins;
	struct md2_texCoord_t *texcoords;
	struct md2_triangle_t *triangles;
	struct md2_frame_t *frames;
	int *glcmds;
	
	GLuint tex_id;

	
	GLuint list;

	double frameNr;
	//float interp;
	


	int Openmodel(const char *filename);
	int OpenTexture(const char *filename);

	void RenderFrameItpWithGLCmds();
	void RenderFrame(int n);

	//void Strech(double k);//GAL BLOGAI VEIKIA IR TIK 0 FRAME
	
	void Render();

	unsigned long time;

	void Progress();


public:
	
	
	int Open(const char *modelFilename, const char *TexFilename);
	int Open(const char *path);
	
	void Draw();

	void Draw(double x, double y, double z, double k = 0);
	
	void Draw(point p, double k = 0);
	void Draw(point3 p, double k = 0);
	
};



#endif