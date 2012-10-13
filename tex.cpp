#include "tex.h"


#include <gl\gl.h>			// Header File For The OpenGL32 Library


AUX_RGBImageRec *LoadBMP(const char *Filename)				
{
	FILE *File=0;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return 0;									// If Not Return 0
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return 0;										// If Load Failed Return 0
}


GLuint LoadTexture(const char *filename)
{
	GLuint t;

	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To 0

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	//TextureImage[0] = auxDIBImageLoad("tex/NeHe.bmp");
	if (TextureImage[0]=LoadBMP(filename))
	{
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &t);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, t);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}

	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	//return Status;										// Return The Status
	return t;
}

GLuint LoadTexture(std::string &filename)
{
	return LoadTexture(filename.c_str());
}