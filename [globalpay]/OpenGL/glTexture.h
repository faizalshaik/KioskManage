/*
>>> glTexture class

	originally created by unknown geek
	TGA loading code by NeHe. http://nehe.gamedev.net
	byte swapping optimized by Steve Thomas
	superseded by coden. codesaur@gmail.com. Metatech LLC. http://www.metatech.mn

>>> 2013.01.12 - glTexture.h
*/

#pragma once

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

namespace codesaur {
#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif

#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif

	struct glTGAHeader
	{
		GLubyte hdr[12];
	};

	struct glTGA
	{
		GLubyte header[6];
		GLuint  bytesPerPixel;
		GLuint  imageSize;
		GLuint  temp;
		GLuint  type;
		GLuint  Height;
		GLuint  Width;
		GLuint  Bpp;
	};

	class glTexture
	{
	public:
		glTexture();
		~glTexture();
		void FreeData();

		GLuint PushTexture(char*, GLfloat*, GLfloat*, GLboolean alpha = GL_TRUE, GLboolean repeat = GL_TRUE, GLboolean filtering = GL_TRUE, GLboolean mipmaps = GL_TRUE);

	protected:
		GLboolean LoadTGA(const char*);
		GLboolean LoadCompressedTGA(const char*, FILE*);
		GLboolean LoadUncompressedTGA(const char*, FILE*);
		GLuint    CheckIfCompOrNotTGA(const glTGAHeader);

	private:
		GLubyte *imageData;
		glTGA    tga;
		GLuint   bpp;
		GLuint   width;
		GLuint   height;
		GLuint   texID;
		GLuint   type;
	};
}