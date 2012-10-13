#include "model.h"


#include <string.h>//for strcat
#include <math.h>

vec3_t anorms[162] = {
#include "anorms.h"
};


#include "const.h"



void model::Progress()
{
	unsigned long now = GetTickCount();

	double t = double(now - time) / 1000 * c_models_frames_per_second;
	frameNr += t;
	frameNr = fmod(frameNr, header.num_frames - 1);

	time = now;
}


int model::Openmodel(const char *filename)
{
	FILE *fp;
	int i;

	fp = fopen (filename, "rb");
	if (!fp)
	{
		fprintf (stderr, "Error: couldn't open \"%s\"!\n", filename);
		return 1;
	}

	//Read header 
	fread (&header, 1, sizeof (struct md2_header_t), fp);

	if ((header.ident != 844121161) ||
		(header.version != 8))
	{
		//Error!
		fprintf (stderr, "Error: bad version or identifier\n");
		fclose (fp);
		return 1;
	}

	//Memory allocations
	skins = (struct md2_skin_t *)
	malloc (sizeof (struct md2_skin_t) * header.num_skins);
	texcoords = (struct md2_texCoord_t *)
	malloc (sizeof (struct md2_texCoord_t) * header.num_st);
	triangles = (struct md2_triangle_t *)
	malloc (sizeof (struct md2_triangle_t) * header.num_tris);
	frames = (struct md2_frame_t *)
	malloc (sizeof (struct md2_frame_t) * header.num_frames);
	glcmds = (int *)malloc (sizeof (int) * header.num_glcmds);

	//Read model data 
	fseek (fp, header.offset_skins, SEEK_SET);
	fread (skins, sizeof (struct md2_skin_t),
		header.num_skins, fp);

	fseek (fp, header.offset_st, SEEK_SET);
	fread (texcoords, sizeof (struct md2_texCoord_t),
		header.num_st, fp);

	fseek (fp, header.offset_tris, SEEK_SET);
	fread (triangles, sizeof (struct md2_triangle_t),
		header.num_tris, fp);

	fseek (fp, header.offset_glcmds, SEEK_SET);
	fread (glcmds, sizeof (int), header.num_glcmds, fp);

	//Read frames
	fseek (fp, header.offset_frames, SEEK_SET);
	for (i = 0; i < header.num_frames; ++i)
	{
		//Memory allocation for vertices of this frame 
		frames[i].verts = (struct md2_vertex_t *)
	malloc (sizeof (struct md2_vertex_t) * header.num_vertices);

		//Read frame data 
		fread (frames[i].scale, sizeof (vec3_t), 1, fp);
		fread (frames[i].translate, sizeof (vec3_t), 1, fp);
		fread (frames[i].name, sizeof (char), 16, fp);
		fread (frames[i].verts, sizeof (struct md2_vertex_t),
			header.num_vertices, fp);
	}

	fclose (fp);


	frameNr = 0;

	return 0;//sekmingai
}



int model::OpenTexture(const char *filename)
{
	tex_id = LoadTexture(filename);


	return 0;
}
int model::Open(const char *modelFilename, const char *TexFilename)
{
   if (Openmodel(modelFilename) || OpenTexture(TexFilename))
	   return 1;//klaida atidarant

   if (header.num_frames == 1)
   {
	   list = glGenLists(1);

	   glNewList(list, GL_COMPILE);
   
	   glRotated(180, 0, -1, 0);
	   glRotated(90, -1, 0, 0);
	   RenderFrame(0);
	   glRotated(90, 1, 0, 0);
	   glRotated(180, 0, 1, 0);
   
	   glEndList();
   }
   
   return 0;
}


int model::Open(const char *path)
{
	char main_path[] = c_dir_model_main_path;
	
	char end_model[] = c_file_model_model_end;
	char end_tex[] = c_file_model_tex_end;

	char m[128], t[128];
	strcpy(m, main_path);
	strcat(m, path);
	strcat(m, end_model);
	
	strcpy(t, main_path);
	strcat(t, path);
	strcat(t, end_tex);

	return Open(m, t);
}




void model::RenderFrame(int n)
{
	int i, j;
	GLfloat s, t;
	vec3_t v;
	md2_frame_t *pframe;
	md2_vertex_t *pvert;

	/* check if n is in a valid range */
	if ((n < 0) || (n > header.num_frames - 1))
	return;

	/* enable model's texture */
	glBindTexture (GL_TEXTURE_2D, tex_id);

	/* draw the model */
	glBegin (GL_TRIANGLES);
    /* draw each triangle */
    for (i = 0; i < header.num_tris; ++i)
		/* draw each vertex */
		for (j = 0; j < 3; ++j)
		{
			pframe = &frames[n];
			pvert = &pframe->verts[triangles[i].vertex[j]];

			/* compute texture coordinates */
			s = (GLfloat)texcoords[triangles[i].st[j]].s / header.skinwidth;
			t = (GLfloat)texcoords[triangles[i].st[j]].t / header.skinheight;

			/* pass texture coordinates to OpenGL */
			glTexCoord2f (s, 1 - t);

			/* normal vector */
			glNormal3fv (anorms[pvert->normalIndex]);

			/* calculate vertex real position */
			//2 su 1 sukeista
			v[0] = (pframe->scale[0] * pvert->v[0]) + pframe->translate[0];
			v[1] = (pframe->scale[1] * pvert->v[1]) + pframe->translate[1];
			v[2] = (pframe->scale[2] * pvert->v[2]) + pframe->translate[2];
		
			//v[0] *= -1;//bug - reversed horizontal rendering. This fixes it

			glVertex3fv (v);
		}

	glEnd ();
}


void model::Render()
{
	int n = int(frameNr);
	float interp = frameNr - n;

	int i, j;
	GLfloat s, t;
	vec3_t v_curr, v_next, v, norm;
	float *n_curr, *n_next;
	md2_frame_t *pframe1, *pframe2;
	md2_vertex_t *pvert1, *pvert2;

	/* check if n is in a valid range */
	if ((n < 0) || (n > header.num_frames))
	return;

	/* enable model's texture */
	glBindTexture (GL_TEXTURE_2D, tex_id);

	/* draw the model */
	glBegin (GL_TRIANGLES);
	/* draw each triangle */
	for (i = 0; i < header.num_tris; ++i)
		/* draw each vertex */
		for (j = 0; j < 3; ++j)
		{
			pframe1 = &frames[n];
			pframe2 = &frames[n + 1];
			pvert1 = &pframe1->verts[triangles[i].vertex[j]];
			pvert2 = &pframe2->verts[triangles[i].vertex[j]];

			/* compute texture coordinates */
			s = (GLfloat)texcoords[triangles[i].st[j]].s / header.skinwidth;
			t = (GLfloat)texcoords[triangles[i].st[j]].t / header.skinheight;

			/* pass texture coordinates to OpenGL */
			glTexCoord2f (s, 1 - t);

			/* interpolate normals */
			n_curr = anorms[pvert1->normalIndex];
			n_next = anorms[pvert2->normalIndex];

			norm[0] = n_curr[0] + interp * (n_next[0] - n_curr[0]);
			norm[2] = n_curr[1] + interp * (n_next[1] - n_curr[1]);
			norm[1] = n_curr[2] + interp * (n_next[2] - n_curr[2]);

			glNormal3fv (norm);

			/* interpolate vertices */
			v_curr[0] = pframe1->scale[0] * pvert1->v[0] + pframe1->translate[0];
			v_curr[1] = pframe1->scale[1] * pvert1->v[1] + pframe1->translate[1];
			v_curr[2] = pframe1->scale[2] * pvert1->v[2] + pframe1->translate[2];

			v_next[0] = pframe2->scale[0] * pvert2->v[0] + pframe2->translate[0];
			v_next[1] = pframe2->scale[1] * pvert2->v[1] + pframe2->translate[1];
			v_next[2] = pframe2->scale[2] * pvert2->v[2] + pframe2->translate[2];

			v[0] = v_curr[0] + interp * (v_next[0] - v_curr[0]);
			v[2] = v_curr[1] + interp * (v_next[1] - v_curr[1]);
			v[1] = v_curr[2] + interp * (v_next[2] - v_curr[2]);

			glVertex3fv (v);
		}

	glEnd ();
}


void model::RenderFrameItpWithGLCmds ()
{
	int n = 0;
	float interp = 0;

	int i, *pglcmds;
	vec3_t v_curr, v_next, v, norm;
	float *n_curr, *n_next;
	struct md2_frame_t *pframe1, *pframe2;
	struct md2_vertex_t *pvert1, *pvert2;
	struct md2_glcmd_t *packet;

	// Check if n is in a valid range 
	if ((n < 0) || (n > header.num_frames - 1))
	return;

	// Enable model's texture 
	glBindTexture (GL_TEXTURE_2D, tex_id);

	// pglcmds points at the start of the command list 
	pglcmds = glcmds;

	// Draw the model 
	while ((i = *(pglcmds++)) != 0)
	{
		if (i < 0)
		{
			glBegin (GL_TRIANGLE_FAN);
			i = -i;
		}
		else
		{
			glBegin (GL_TRIANGLE_STRIP);
		}

		// Draw each vertex of this group 
		for (; i > 0; --i, pglcmds += 3)
		{
			packet = (struct md2_glcmd_t *)pglcmds;
			pframe1 = &frames[n];
			pframe2 = &frames[n + 1];
			pvert1 = &pframe1->verts[packet->index];
			pvert2 = &pframe2->verts[packet->index];

			// Pass texture coordinates to OpenGL 
			glTexCoord2f (packet->s, packet->t);

			//Interpolate normals
			n_curr = anorms[pvert1->normalIndex];
			n_next = anorms[pvert2->normalIndex];

			norm[0] = n_curr[0] + interp * (n_next[0] - n_curr[0]);
			norm[1] = n_curr[1] + interp * (n_next[1] - n_curr[1]);
			norm[2] = n_curr[2] + interp * (n_next[2] - n_curr[2]);

			glNormal3fv (norm);

			// Interpolate vertices 
			v_curr[0] = pframe1->scale[0] * pvert1->v[0] + pframe1->translate[0];
			v_curr[1] = pframe1->scale[1] * pvert1->v[1] + pframe1->translate[1];
			v_curr[2] = pframe1->scale[2] * pvert1->v[2] + pframe1->translate[2];

			v_next[0] = pframe2->scale[0] * pvert2->v[0] + pframe2->translate[0];
			v_next[1] = pframe2->scale[1] * pvert2->v[1] + pframe2->translate[1];
			v_next[2] = pframe2->scale[2] * pvert2->v[2] + pframe2->translate[2];

			v[0] = v_curr[0] + interp * (v_next[0] - v_curr[0]);
			v[1] = v_curr[1] + interp * (v_next[1] - v_curr[1]);
			v[2] = v_curr[2] + interp * (v_next[2] - v_curr[2]);

			glVertex3fv (v);
		}

		glEnd ();
	}
}

/*
void model::Strech(double k)
{
   int i, j;
   for (i = 0; i < header.num_tris; i++)
   {
	   for (j = 0; j < 3; j++)
			frames[0].verts[i].v[j] = unsigned char(double(frames[0].verts[i].v[j])*k);

	   //frames[0].verts[i].v[0] *= k;
	   //frames[0].verts[i].v[1] *= k;
	   //frames[0].verts[i].v[2] *= k;	   
   }
}
*/



void model::Draw()
{
    glBindTexture(GL_TEXTURE_2D, tex_id);

	if (header.num_frames == 1)
		glCallList(list);
	else
	{
		Progress();
		Render();
	}
}


void model::Draw(double x, double y, double z, double k)
{
	glTranslated(x, y, z);
	glRotated(k, 0, -1, 0);

	Draw();
	
	glRotated(k, 0, 1, 0);
	glTranslated(-x, -y, -z);
}

void model::Draw(point p, double k)
{
	Draw(p.x, 0, p.y, k);
}


void model::Draw(point3 p, double k)
{
	Draw(p.x, p.y, p.z, k);
}
