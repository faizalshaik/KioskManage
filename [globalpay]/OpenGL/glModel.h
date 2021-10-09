/*
>>> Wavefront OBJ model file format reader/writer/manipulator

	originally created by Nate Robbins. 2000. nate@pobox.com. http://www.pobox.com/~nate
	improved version of GLM .08.05.2008 Tudor Carean
	superseded by coden aka Narankhuu N. 2013. codesaur@gmail.com. Metatech LLC. http://www.metatech.mn

>>> 2013.01.12 - glModel.h
*/

#pragma once

#define  _USE_MATH_DEFINES 
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <gl/gl.h>

namespace codesaur {
#define GLMODEL_NONE     (0)
#define GLMODEL_FLAT     (1 << 0)
#define GLMODEL_SMOOTH   (1 << 1)
#define GLMODEL_TEXTURE  (1 << 2)
#define GLMODEL_COLOR    (1 << 3)
#define GLMODEL_MATERIAL (1 << 4)

#define T(x) (triangles[(x)])
	
	struct glModelNode
	{
		GLuint       index;
		GLboolean    averaged;
		glModelNode *next;
	};

	struct glModelMaterial
	{
		char*   name;
		GLfloat diffuse[4];
		GLfloat ambient[4];
		GLfloat specular[4];
		GLfloat emmissive[4];
		GLfloat shininess;
		GLuint  IDtexture;
	};

	struct glModelTriangle
	{
		GLuint    vindices[3];
		GLuint    nindices[3];
		GLuint    tindices[3];
		GLuint    findex;
		GLuint    neighbor[3];
		GLboolean visible;
	};

	struct glModelTexture
	{
		char   *name;
		GLuint  id;
		GLfloat width;
		GLfloat height;
	};

	struct glModelGroup
	{
		char         *name;
		GLuint        numtriangles;
		GLuint       *triangles;
		GLuint        material;
		glModelGroup *next;
	};

	class glModel
	{
	public:
		glModel();
		~glModel();

		GLvoid Default();
		GLvoid Delete();
		GLvoid Unload();
		GLvoid LoadData(const char*);

		GLvoid Draw(GLuint);
		GLuint List(GLuint);

		GLfloat Unitize();
		GLvoid Dimensions(GLfloat*);
		GLvoid Scale(GLfloat);
		GLvoid ReverseWinding();
		GLvoid FacetNormals();
		GLvoid VertexNormals(GLfloat);
		GLvoid LinearTexture();
		GLvoid SpheremapTexture();
		GLvoid Weld(GLfloat);

	private:
		char            *pathname;
		char            *mtllibname;

		GLuint           numvertices;
		GLfloat         *vertices;
		GLuint           numnormals;
		GLfloat         *normals;
		GLuint           numtexcoords;
		GLfloat         *texcoords;
		GLuint           numfacetnorms;
		GLfloat         *facetnorms;
		GLuint           numtriangles;
		glModelTriangle *triangles;
		GLuint           nummaterials;
		glModelMaterial *materials;
		GLuint           numgroups;
		glModelGroup    *groups;
		GLuint           numtextures;
		glModelTexture  *textures;
		GLfloat          position[3];

	protected:
		GLboolean Read(const char*);
		GLboolean ReadMTL(const char*);

		GLvoid FirstPass(FILE*);
		GLvoid SecondPass(FILE*);

		glModelGroup* AddGroup(const char*);
		glModelGroup* FindGroup(const char*);

		GLuint FindMaterial(const char*);
		GLuint FindOrAddTexture(char*);

		char* ExtractDirName(const char*);
		GLfloat Absolute(GLfloat);
		GLfloat FloatMax(GLfloat, GLfloat);
		GLfloat Dot(GLfloat*, GLfloat*);
		GLvoid Cross(GLfloat*, GLfloat*, GLfloat*);
		GLvoid Normalize(GLfloat*);
		GLboolean Equal(GLfloat*, GLfloat*, GLfloat);
		GLfloat* WeldVectors(GLfloat*, GLuint*, GLfloat);
	};
}