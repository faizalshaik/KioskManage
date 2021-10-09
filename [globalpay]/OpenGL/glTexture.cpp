// original name : glTexture.cpp
// more info @   : glTexture.h

#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "gltexture.h"

using namespace codesaur;

glTexture::glTexture()
{
	imageData = NULL;
}

glTexture::~glTexture()
{
	FreeData();
}

void glTexture::FreeData()
{
	if (imageData != NULL)
		::free(imageData);
}

GLuint glTexture::PushTexture(char *filename, GLfloat *texcoordwidth, GLfloat *texcoordheight, GLboolean alpha, GLboolean repeat, GLboolean filtering, GLboolean mipmaps)
{
	GLuint tex;
	GLuint pixelsize;
	GLint filter_min, filter_mag;
	GLint xSize2, ySize2;

	char *numefis = filename;
    while (*numefis ==' ') numefis++;
	
	if (LoadTGA((char*) numefis) == GL_FALSE)
		return 0;
	
	if (imageData == NULL)
		return 0;
	
	switch (type)
	{
	case GL_LUMINANCE:
		pixelsize = 1;
		break;
	case GL_RGB:
	case GL_BGR:
		pixelsize = 3;
		break;
    case GL_RGBA:
	case GL_BGRA:
		pixelsize = 4;
		break;
	default:
		pixelsize = 0;
		break;
	}
	
	if ((pixelsize * width) % 4 == 0)
		::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	else
		::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	xSize2 = width;
	ySize2 = height;
	
	::glGenTextures(1, &tex);
	::glBindTexture(GL_TEXTURE_2D, tex);
	
	if (filtering)
	{
		filter_min = (mipmaps) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
		filter_mag = GL_LINEAR;
	}
	else
	{
		filter_min = (mipmaps) ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
		filter_mag = GL_NEAREST;
	}
	
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (repeat) ? GL_REPEAT : GL_CLAMP);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (repeat) ? GL_REPEAT : GL_CLAMP);
	
	if (mipmaps)
		::gluBuild2DMipmaps(GL_TEXTURE_2D, type, xSize2, ySize2, type, GL_UNSIGNED_BYTE, imageData);
	else
		::glTexImage2D(GL_TEXTURE_2D, 0, type, xSize2, ySize2, 0, type, GL_UNSIGNED_BYTE, imageData); 

	*texcoordwidth = (GLfloat) xSize2;
	*texcoordheight = (GLfloat) ySize2;
	return tex;
}

GLboolean glTexture::LoadTGA(const char * filename)
{
	FILE* fTGA;
	glTGAHeader	tgaheader;
	GLboolean ret;
	
	if (::fopen_s(&fTGA, filename, "rb") != 0)
		return GL_FALSE;

	if (::fread(&tgaheader, sizeof(glTGAHeader), 1, fTGA) == 0)
	{
		if (fTGA != NULL)
			::fclose(fTGA);

		return GL_FALSE;
	}
	
	switch (CheckIfCompOrNotTGA(tgaheader))
	{
	case 1: ret = LoadUncompressedTGA(filename, fTGA); break;
	case 2: ret = LoadCompressedTGA(filename, fTGA); break;
	default:
			::fclose(fTGA);
			return GL_FALSE;
			break;
	}

	return ret;
}

GLboolean glTexture::LoadUncompressedTGA(const char * filename, FILE * fTGA)
{
	if (::fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{
		if (fTGA != NULL)
			fclose(fTGA);
		return GL_FALSE;
	}
	
	width = tga.header[1] * 256 + tga.header[0];
	height = tga.header[3] * 256 + tga.header[2];
	bpp = tga.header[4];
	tga.Width = width;
	tga.Height = height;
	tga.Bpp = bpp;
	
	if ((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp !=32)))
	{
		if (fTGA != NULL)
			::fclose(fTGA);
		
		return GL_FALSE;
	}
	
	if (bpp == 24)
		type = GL_RGB;
	else
		type = GL_RGBA;
	
	tga.bytesPerPixel = (tga.Bpp / 8);
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);
	imageData = (GLubyte *) ::malloc(tga.imageSize);	
	if (imageData == NULL)
	{
		::fclose(fTGA);
		return GL_FALSE;
	}

	if (::fread(imageData, 1, tga.imageSize, fTGA) != tga.imageSize)
	{
		if (imageData != NULL)
			::free(imageData);
		
		::fclose(fTGA);
		return GL_FALSE;
	}
	
	for (GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
		imageData[cswap] ^= imageData[cswap+2] ^= imageData[cswap] ^= imageData[cswap+2];

	::fclose(fTGA);
	return GL_TRUE;
}

GLboolean glTexture::LoadCompressedTGA(const char * filename, FILE * fTGA)
{
	if (::fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{
		if (fTGA != NULL)
			::fclose(fTGA);
		
		return GL_FALSE;
	}
	
	width = tga.header[1] * 256 + tga.header[0];
	height = tga.header[3] * 256 + tga.header[2];
	bpp	= tga.header[4];
	tga.Width = width;
	tga.Height = height;
	tga.Bpp = bpp;
	
	if ((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp !=32)))
	{
		if (fTGA != NULL)
			::fclose(fTGA);
		
		return GL_FALSE;
	}
	
	if (bpp == 24)
		type = GL_RGB;
	else
		type = GL_RGBA;
	
	tga.bytesPerPixel = (tga.Bpp / 8);
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);
	imageData = (GLubyte *) ::malloc(tga.imageSize);	
	if (imageData == NULL)
	{
		::fclose(fTGA);
		return GL_FALSE;
	}
	
	GLuint pixelcount = tga.Height * tga.Width;
	GLuint currentpixel	= 0;
	GLuint currentbyte = 0;
	GLubyte* colorbuffer = (GLubyte *) ::malloc(tga.bytesPerPixel);
	do
	{
		GLubyte chunkheader = 0;		
		if (::fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)
		{
		
			if (fTGA != NULL)
				::fclose(fTGA);
			
			if (imageData != NULL)
				::free(imageData);
			
			return GL_FALSE;
		}
		
		if (chunkheader < 128)
		{
			chunkheader++;
			for (short counter = 0; counter < chunkheader; counter++)
			{
				if (::fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
				{
					if (fTGA != NULL)
						::fclose(fTGA);
					
					if (colorbuffer != NULL)
						::free(colorbuffer);
					
					if (imageData != NULL)	
						::free(imageData);	
					
					return GL_FALSE;
				}
				
				imageData[currentbyte] = colorbuffer[2];
				imageData[currentbyte + 1] = colorbuffer[1];
				imageData[currentbyte + 2] = colorbuffer[0];
				
				if (tga.bytesPerPixel == 4)		
					imageData[currentbyte + 3] = colorbuffer[3];
				
				currentbyte += tga.bytesPerPixel;
				currentpixel++;				
				if (currentpixel > pixelcount)	
				{

					if (fTGA != NULL)
						::fclose(fTGA);
					
					if (colorbuffer != NULL)
						::free(colorbuffer);
					
					if (imageData != NULL)	
						::free(imageData);	
					
					return GL_FALSE;
				}
			}
		}
		else
		{
			chunkheader -= 127;
			if (::fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
			{
				if (fTGA != NULL)
					::fclose(fTGA);
				
				if (colorbuffer != NULL)	
					::free(colorbuffer);
				
				if (imageData != NULL)
					::free(imageData);	
				
				return GL_FALSE;
			}
			
			for (short counter = 0; counter < chunkheader; counter++)
			{
				imageData[currentbyte] = colorbuffer[2];
				imageData[currentbyte + 1] = colorbuffer[1];
				imageData[currentbyte + 2] = colorbuffer[0];
				
				if (tga.bytesPerPixel == 4)
					imageData[currentbyte + 3] = colorbuffer[3];
				
				currentbyte += tga.bytesPerPixel;
				currentpixel++;				
				if (currentpixel > pixelcount)
				{
					if (fTGA != NULL)
						::fclose(fTGA);
					
					if (colorbuffer != NULL)
						::free(colorbuffer);
					
					if (imageData != NULL)
						::free(imageData);
					return GL_FALSE;
				}
			}
		}
	} while (currentpixel < pixelcount);
	
	::fclose(fTGA);
	return GL_TRUE;
}

GLuint glTexture::CheckIfCompOrNotTGA(const glTGAHeader tgahdr)
{
	GLubyte uTGAcompare[12] = {0,0,2,0,0,0,0,0,0,0,0,0};
	GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

	if (::memcmp(uTGAcompare, &tgahdr, sizeof(tgahdr)) == 0) return 1;
	if (::memcmp(cTGAcompare, &tgahdr, sizeof(tgahdr)) == 0) return 2;
	
	return 0;
}
