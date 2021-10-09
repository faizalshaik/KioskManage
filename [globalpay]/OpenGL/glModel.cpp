// original name : glModel.cpp
// more info @   : glModel.h

#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "glmodel.h"
#include "gltexture.h"

using namespace codesaur;

glModel::glModel()
{
	Default();
}

glModel::~glModel()
{
	Delete();
}

GLvoid glModel::Default()
{
	pathname = NULL;
	mtllibname = NULL;
	
	numvertices = 0;
	vertices = NULL;
	numnormals = 0;
	normals = NULL;
	numtexcoords = 0;
	texcoords = NULL;
	numfacetnorms = 0;
	facetnorms = NULL;
	numtriangles =  0;
	triangles = NULL;
	nummaterials = 0;
	materials = 0;
	numgroups = 0;
	groups = NULL;
	numtextures = 0;
	textures = NULL;
	position[0] = 0.0;
	position[1] = 0.0;
	position[2] = 0.0;
}

void glModel::Delete()
{
	glModelGroup* group;
	
	if (pathname) ::free(pathname);
    if (mtllibname) ::free(mtllibname);
    if (vertices) ::free(vertices);
    if (normals) ::free(normals);
    if (texcoords) ::free(texcoords);
	if (facetnorms) ::free(facetnorms);
	if (triangles) ::free(triangles);
	if (materials)
	{
		for (GLuint i = 0; i < nummaterials; i++)
			::free(materials[i].name);
		::free(materials);
	}
	if (textures)
	{
		for (GLuint i = 0; i < numtextures; i++)
		{
			::free(textures[i].name);
			::glDeleteTextures(1, &textures[i].id);
		}
		::free(textures);
	}
	while(groups)
	{
		group = groups;
		groups = groups->next;
		::free(group->name);
		::free(group->triangles);
		::free(group);
	}
}

GLvoid glModel::Unload()
{
	Delete();
	Default();
}

GLvoid glModel::LoadData(const char* objfilename)
{
	if (Read(objfilename) == GL_FALSE)
	{
	}
}

GLvoid glModel::Draw(GLuint mode)
{
	glModelGroup* group;
	glModelTriangle* triangle;
	glModelMaterial* material;
	GLuint IDtexture;
	
	assert(vertices);
	
	if (mode & GLMODEL_FLAT && !facetnorms) mode &= ~GLMODEL_FLAT;
	if (mode & GLMODEL_SMOOTH && !normals) mode &= ~GLMODEL_SMOOTH;
	if (mode & GLMODEL_TEXTURE && !texcoords) mode &= ~GLMODEL_TEXTURE;
	if (mode & GLMODEL_FLAT && mode & GLMODEL_SMOOTH) mode &= ~GLMODEL_FLAT;
	if (mode & GLMODEL_COLOR && !materials) mode &= ~GLMODEL_COLOR;
	if (mode & GLMODEL_MATERIAL && !materials) mode &= ~GLMODEL_MATERIAL;
	if (mode & GLMODEL_COLOR && mode & GLMODEL_MATERIAL) mode &= ~GLMODEL_COLOR;

	if (mode & GLMODEL_COLOR)
		::glEnable(GL_COLOR_MATERIAL);
	else
		if (mode & GLMODEL_MATERIAL)
			::glDisable(GL_COLOR_MATERIAL);
	
	if (mode & GLMODEL_TEXTURE)
	{
		::glEnable(GL_TEXTURE_2D);
		::glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	
	IDtexture = -1;
	group = groups;
	while (group)
	{
		material = &materials[group->material];
		if (material)
			IDtexture = material->IDtexture;
		else
			IDtexture = -1;
		
		if (mode & GLMODEL_MATERIAL)
		{
			::glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->ambient);
			::glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->diffuse);
			::glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->specular);
			::glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);
		}
		
		if (mode & GLMODEL_TEXTURE)
		{				
			if(IDtexture == -1)
				::glBindTexture(GL_TEXTURE_2D, 0);
			else
				::glBindTexture(GL_TEXTURE_2D, textures[IDtexture].id);		
		}

		if (mode & GLMODEL_COLOR)
			::glColor3fv(material->diffuse);
		
		::glBegin(GL_TRIANGLES);
		for (GLuint i = 0; i < group->numtriangles; i++)
		{
			triangle = &T(group->triangles[i]);

			if (mode & GLMODEL_FLAT)
				::glNormal3fv(&facetnorms[3 * triangle->findex]);
			if (mode & GLMODEL_SMOOTH)
				::glNormal3fv(&normals[3 * triangle->nindices[0]]);
			if (mode & GLMODEL_TEXTURE)
				::glTexCoord2fv(&texcoords[2 * triangle->tindices[0]]);
			
			::glVertex3fv(&vertices[3 * triangle->vindices[0]]);
			
			if (mode & GLMODEL_SMOOTH)
				::glNormal3fv(&normals[3 * triangle->nindices[1]]);
			if (mode & GLMODEL_TEXTURE)
				::glTexCoord2fv(&texcoords[2 * triangle->tindices[1]]);
			
			::glVertex3fv(&vertices[3 * triangle->vindices[1]]);
			
			if (mode & GLMODEL_SMOOTH)
				::glNormal3fv(&normals[3 * triangle->nindices[2]]);
			if (mode & GLMODEL_TEXTURE)
				::glTexCoord2fv(&texcoords[2 * triangle->tindices[2]]);
			
			::glVertex3fv(&vertices[3 * triangle->vindices[2]]);
		}
		::glEnd();
		group = group->next;
	}
}

GLuint glModel::List(GLuint mode)
{
	GLuint list = ::glGenLists(1);
	::glNewList(list, GL_COMPILE);
	Draw(mode);
	::glEndList();

	return list;
}

GLfloat glModel::Unitize()
{
	GLfloat maxx, minx, maxy, miny, maxz, minz;
	GLfloat cx, cy, cz, w, h, d;
	GLfloat scale;
	
	assert(vertices);

	maxx = minx = vertices[3 + 0];
	maxy = miny = vertices[3 + 1];
	maxz = minz = vertices[3 + 2];

	for (GLuint i = 1; i <= numvertices; i++)
	{
		if (maxx < vertices[3 * i + 0])
			maxx = vertices[3 * i + 0];
		if (minx > vertices[3 * i + 0])
			minx = vertices[3 * i + 0];
		
		if (maxy < vertices[3 * i + 1])
			maxy = vertices[3 * i + 1];
		if (miny > vertices[3 * i + 1])
			miny = vertices[3 * i + 1];
		
		if (maxz < vertices[3 * i + 2])
			maxz = vertices[3 * i + 2];
		if (minz > vertices[3 * i + 2])
			minz = vertices[3 * i + 2];
	}
	
	w = Absolute(maxx) + Absolute(minx);
	h = Absolute(maxy) + Absolute(miny);
	d = Absolute(maxz) + Absolute(minz);
	
	cx = (maxx + minx) / (GLfloat)2.0;
	cy = (maxy + miny) / (GLfloat)2.0;
	cz = (maxz + minz) / (GLfloat)2.0;

	scale = (GLfloat)2.0 / FloatMax(FloatMax(w, h), d);
	for (GLuint i = 1; i <= numvertices; i++)
	{
		vertices[3 * i + 0] -= cx;
		vertices[3 * i + 1] -= cy;
		vertices[3 * i + 2] -= cz;
		vertices[3 * i + 0] *= scale;
		vertices[3 * i + 1] *= scale;
		vertices[3 * i + 2] *= scale;
	}
	return scale;
}

GLvoid glModel::Dimensions(GLfloat* dimensions)
{
	GLfloat maxx, minx, maxy, miny, maxz, minz;
	
	assert(vertices);
	assert(dimensions);
	
	maxx = minx = vertices[3 + 0];
	maxy = miny = vertices[3 + 1];
	maxz = minz = vertices[3 + 2];
	
	for (GLuint i = 1; i <= numvertices; i++)
	{
		if (maxx < vertices[3 * i + 0])
			maxx = vertices[3 * i + 0];
		if (minx > vertices[3 * i + 0])
			minx = vertices[3 * i + 0];
		
		if (maxy < vertices[3 * i + 1])
			maxy = vertices[3 * i + 1];
		if (miny > vertices[3 * i + 1])
			miny = vertices[3 * i + 1];
		
		if (maxz < vertices[3 * i + 2])
			maxz = vertices[3 * i + 2];
		if (minz > vertices[3 * i + 2])
			minz = vertices[3 * i + 2];
	}
	dimensions[0] = Absolute(maxx) + Absolute(minx);
	dimensions[1] = Absolute(maxy) + Absolute(miny);
	dimensions[2] = Absolute(maxz) + Absolute(minz);
}
 
GLvoid glModel::Scale(GLfloat scale)
{
	for (GLuint i = 1; i <= numvertices; i++)
	{
		vertices[3 * i + 0] *= scale;
		vertices[3 * i + 1] *= scale;
		vertices[3 * i + 2] *= scale;
	}
}

GLvoid glModel::ReverseWinding()
{
	GLuint swap;
	
	for (GLuint i = 0; i < numtriangles; i++)
	{
		swap = T(i).vindices[0];
		T(i).vindices[0] = T(i).vindices[2];
		T(i).vindices[2] = swap;
		
		if (numnormals)
		{
			swap = T(i).nindices[0];
			T(i).nindices[0] = T(i).nindices[2];
			T(i).nindices[2] = swap;
		}

		if (numtexcoords)
		{
			swap = T(i).tindices[0];
			T(i).tindices[0] = T(i).tindices[2];
			T(i).tindices[2] = swap;
		}
	}
	
	for (GLuint i = 1; i <= numfacetnorms; i++)
	{
		facetnorms[3 * i + 0] = -facetnorms[3 * i + 0];
		facetnorms[3 * i + 1] = -facetnorms[3 * i + 1];
		facetnorms[3 * i + 2] = -facetnorms[3 * i + 2];
	}
	
	for (GLuint i = 1; i <= numnormals; i++)
	{
		normals[3 * i + 0] = -normals[3 * i + 0];
		normals[3 * i + 1] = -normals[3 * i + 1];
		normals[3 * i + 2] = -normals[3 * i + 2];
	}
}

GLvoid glModel::FacetNormals()
{
	GLfloat u[3];
	GLfloat v[3];
	
	assert(vertices);
	
	if (facetnorms)
		::free(facetnorms);
	
	numfacetnorms = numtriangles;
	facetnorms = (GLfloat*) ::malloc(sizeof(GLfloat) * 3 * (numfacetnorms + 1));
	
	for (GLuint i = 0; i < numtriangles; i++)
	{
		triangles[i].findex = i+1;
		
		u[0] = vertices[3 * T(i).vindices[1] + 0] - vertices[3 * T(i).vindices[0] + 0];
		u[1] = vertices[3 * T(i).vindices[1] + 1] - vertices[3 * T(i).vindices[0] + 1];
		u[2] = vertices[3 * T(i).vindices[1] + 2] - vertices[3 * T(i).vindices[0] + 2];
		v[0] = vertices[3 * T(i).vindices[2] + 0] - vertices[3 * T(i).vindices[0] + 0];
		v[1] = vertices[3 * T(i).vindices[2] + 1] - vertices[3 * T(i).vindices[0] + 1];
		v[2] = vertices[3 * T(i).vindices[2] + 2] - vertices[3 * T(i).vindices[0] + 2];
		
		Cross(u, v, &facetnorms[3 * (i + 1)]);
		Normalize(&facetnorms[3 * (i + 1)]);
	}
}

GLvoid glModel::VertexNormals(GLfloat angle)
{
	glModelNode* node;
	glModelNode* tail;
	glModelNode** members;
	GLfloat* tmp_normals;
	GLuint tmp_numnormals;
	GLfloat average[3];
	GLfloat dot, cos_angle;
	GLuint avg;
	
	assert(facetnorms);
	
	cos_angle = ::cos(angle * (GLfloat) (M_PI / 180.0));
	
	if (normals)
		::free(normals);
	
	numnormals = numtriangles * 3;
	normals = (GLfloat*) ::malloc(sizeof(GLfloat) * 3 * (numnormals + 1));
	
	members = (glModelNode**) ::malloc(sizeof(glModelNode*) * (numvertices + 1));
	for (GLuint i = 1; i <= numvertices; i++)
		members[i] = NULL;
	
	for (GLuint i = 0; i < numtriangles; i++)
	{
		node = (glModelNode*) ::malloc(sizeof(glModelNode));
		node->index = i;
		node->next  = members[T(i).vindices[0]];
		members[T(i).vindices[0]] = node;
		
		node = (glModelNode*) ::malloc(sizeof(glModelNode));
		node->index = i;
		node->next  = members[T(i).vindices[1]];
		members[T(i).vindices[1]] = node;
		
		node = (glModelNode*) ::malloc(sizeof(glModelNode));
		node->index = i;
		node->next  = members[T(i).vindices[2]];
		members[T(i).vindices[2]] = node;
	}
	
	tmp_numnormals = 1;
	for (GLuint i = 1; i <= numvertices; i++)
	{
		node = members[i];

		average[0] = 0.0;
		average[1] = 0.0;
		average[2] = 0.0;
		avg = 0;
		while (node)
		{
			dot = Dot(&facetnorms[3 * T(node->index).findex], &facetnorms[3 * T(members[i]->index).findex]);
			if (dot > cos_angle)
			{
				node->averaged = GL_TRUE;
				average[0] += facetnorms[3 * T(node->index).findex + 0];
				average[1] += facetnorms[3 * T(node->index).findex + 1];
				average[2] += facetnorms[3 * T(node->index).findex + 2];
				avg = 1;
			}
			else
				node->averaged = GL_FALSE;

			node = node->next;
		}
		
		if (avg)
		{
			Normalize(average);
			normals[3 * tmp_numnormals + 0] = average[0];
			normals[3 * tmp_numnormals + 1] = average[1];
			normals[3 * tmp_numnormals + 2] = average[2];
			avg = tmp_numnormals;
			tmp_numnormals++;
		}
		
		node = members[i];
		while (node)
		{
			if (node->averaged)
			{
				if (T(node->index).vindices[0] == i)
					T(node->index).nindices[0] = avg;
				else
					if (T(node->index).vindices[1] == i)
						T(node->index).nindices[1] = avg;
					else
						if (T(node->index).vindices[2] == i)
							T(node->index).nindices[2] = avg;
			}
			else
			{
				normals[3 * tmp_numnormals + 0] = facetnorms[3 * T(node->index).findex + 0];
				normals[3 * tmp_numnormals + 1] = facetnorms[3 * T(node->index).findex + 1];
				normals[3 * tmp_numnormals + 2] = facetnorms[3 * T(node->index).findex + 2];
				
				if (T(node->index).vindices[0] == i)
					T(node->index).nindices[0] = tmp_numnormals;
				else
					if (T(node->index).vindices[1] == i)
						T(node->index).nindices[1] = tmp_numnormals;
					else
						if (T(node->index).vindices[2] == i)
							T(node->index).nindices[2] = tmp_numnormals;

				tmp_numnormals++;
			}
			node = node->next;
		}
	}	
	numnormals = tmp_numnormals - 1;
	
	for (GLuint i = 1; i <= numvertices; i++)
	{
		node = members[i];
		while (node)
		{
			tail = node;
			node = node->next;
			::free(tail);
		}
	}
	::free(members);
	
	tmp_normals = normals;
	normals = (GLfloat*) ::malloc(sizeof(GLfloat) * 3 * (numnormals + 1));
	for (GLuint i = 1; i <= numnormals; i++)
	{
		normals[3 * i + 0] = tmp_normals[3 * i + 0];
		normals[3 * i + 1] = tmp_normals[3 * i + 1];
		normals[3 * i + 2] = tmp_normals[3 * i + 2];
	}
	::free(tmp_normals);
}

GLvoid glModel::LinearTexture()
{
	glModelGroup *group;
	GLfloat dimensions[3];
	GLfloat x, y, scalefactor;
	
	if (texcoords)
		::free(texcoords);
	
	numtexcoords = numvertices;
	texcoords = (GLfloat*) ::malloc(sizeof(GLfloat) * 2 * (numtexcoords + 1));
	
	Dimensions(dimensions);
	scalefactor = (GLfloat) 2.0 / Absolute(FloatMax(FloatMax(dimensions[0], dimensions[1]), dimensions[2]));
	
	for (GLuint i = 1; i <= numvertices; i++)
	{
		x = vertices[3 * i + 0] * scalefactor;
		y = vertices[3 * i + 2] * scalefactor;
		texcoords[2 * i + 0] = (GLfloat) ((x + 1.0) / 2.0);
		texcoords[2 * i + 1] = (GLfloat) ((y + 1.0) / 2.0);
	}
	
	group = groups;
	while (group)
	{
		for (GLuint i = 0; i < group->numtriangles; i++)
		{
			T(group->triangles[i]).tindices[0] = T(group->triangles[i]).vindices[0];
			T(group->triangles[i]).tindices[1] = T(group->triangles[i]).vindices[1];
			T(group->triangles[i]).tindices[2] = T(group->triangles[i]).vindices[2];
		}    
		group = group->next;
	}
}

GLvoid glModel::SpheremapTexture()
{
	glModelGroup* group;
	GLfloat theta, phi, rho, x, y, z, r;
	
	assert(normals);
	
	if (texcoords)
		::free(texcoords);
	
	numtexcoords = numnormals;
	texcoords = (GLfloat*) ::malloc(sizeof(GLfloat) * 2 * (numtexcoords + 1));
	
	for (GLuint i = 1; i <= numnormals; i++)
	{
		z = normals[3 * i + 0];
		y = normals[3 * i + 1];
		x = normals[3 * i + 2];
		r = ::sqrt((x * x) + (y * y));
		rho = ::sqrt((r * r) + (z * z));
		
		if (r == 0.0)
		{
			theta = 0.0;
			phi = 0.0;
		}
		else
		{
			if (z == 0.0)
				phi = (GLfloat) (M_PI / 2.0);
			else
				phi = ::acos(z / rho);
			
			if(y == 0.0)
				theta = (GLfloat) (M_PI / 2.0);
			else
				theta = ::asin(y / r) + (GLfloat) (M_PI / 2.0);
		}
		
		texcoords[2 * i + 0] = theta / (GLfloat) M_PI;
		texcoords[2 * i + 1] = phi / (GLfloat) M_PI;
	}
	
	group = groups;
	while (group)
	{
		for (GLuint i = 0; i < group->numtriangles; i++)
		{
			T(group->triangles[i]).tindices[0] = T(group->triangles[i]).nindices[0];
			T(group->triangles[i]).tindices[1] = T(group->triangles[i]).nindices[1];
			T(group->triangles[i]).tindices[2] = T(group->triangles[i]).nindices[2];
		}
		group = group->next;
	}
}

GLvoid glModel::Weld(GLfloat epsilon)
{
	GLfloat* vectors;
	GLfloat* copies;
	GLuint numvectors;

	numvectors = numvertices;
	vectors  = vertices;
	copies = WeldVectors(vectors, &numvectors, epsilon);
    
	for (GLuint i = 0; i < numtriangles; i++)
	{
		T(i).vindices[0] = (GLuint) vectors[3 * T(i).vindices[0] + 0];
		T(i).vindices[1] = (GLuint) vectors[3 * T(i).vindices[1] + 0];
		T(i).vindices[2] = (GLuint) vectors[3 * T(i).vindices[2] + 0];
	}

	::free(vectors);
	numvertices = numvectors;
	vertices = (GLfloat*) ::malloc(sizeof(GLfloat) * 3 * (numvertices + 1));

	for (GLuint i = 1; i <= numvertices; i++)
	{
		vertices[3 * i + 0] = copies[3 * i + 0];
		vertices[3 * i + 1] = copies[3 * i + 1];
		vertices[3 * i + 2] = copies[3 * i + 2];
	}
	::free(copies);
}

GLboolean glModel::Read(const char* filename)
{
	FILE* file;
	
	if (::fopen_s(&file, filename, "r") != 0)
		return GL_FALSE;

	if (pathname)
		Unload();

	pathname = ::_strdup(filename);	
	FirstPass(file);

	vertices = (GLfloat*) ::malloc(sizeof(GLfloat) * 3 * (numvertices + 1));
	triangles = (glModelTriangle*) ::malloc(sizeof(glModelTriangle) * numtriangles);
	if (numnormals) normals = (GLfloat*) ::malloc(sizeof(GLfloat) * 3 * (numnormals + 1));
	if (numtexcoords) texcoords = (GLfloat*) ::malloc(sizeof(GLfloat) * 2 * (numtexcoords + 1));
	
	::rewind(file);	
	SecondPass(file);
	::fclose(file);

	return GL_TRUE;
}

GLboolean glModel::ReadMTL(const char* name)
{
	if (!pathname) return GL_FALSE;

	FILE* file;
	char* dir;
	char* filename;
	char* texturename;
	char buf[128];
	GLuint nmaterials;
	
	dir = ExtractDirName(pathname);
	filename = (char*) ::malloc(sizeof(char) * (::strlen(dir) + ::strlen(name) + 1));
	::strcpy(filename, dir);
	::strcat(filename, name);
	::free(dir);

	if (::fopen_s(&file, filename, "r") != 0)
		return GL_FALSE;

	::free(filename);

	nmaterials = 1;
	while (::fscanf(file, "%s", buf) != EOF)
	{
		switch (buf[0])
		{
		case '#':
			::fgets(buf, sizeof(buf), file);
			break;
		case 'n':
			::fgets(buf, sizeof(buf), file);
			nmaterials++;
			::sscanf(buf, "%s %s", buf, buf);
			break;
		default:
			::fgets(buf, sizeof(buf), file);
			break;
		}
	}	
	::rewind(file);
	
	materials = (glModelMaterial*) ::malloc(sizeof(glModelMaterial) * nmaterials);
	nummaterials = nmaterials;
	
	for (GLuint i = 0; i < nmaterials; i++)
	{
		materials[i].name = NULL;
		materials[i].shininess = (GLfloat) 65.0;
		materials[i].diffuse[0] = (GLfloat) 0.8;
		materials[i].diffuse[1] = (GLfloat) 0.8;
		materials[i].diffuse[2] = (GLfloat) 0.8;
		materials[i].diffuse[3] = (GLfloat) 1.0;
		materials[i].ambient[0] = (GLfloat) 0.2;
		materials[i].ambient[1] = (GLfloat) 0.2;
		materials[i].ambient[2] = (GLfloat) 0.2;
		materials[i].ambient[3] = (GLfloat) 1.0;
		materials[i].specular[0] = (GLfloat) 0.0;
		materials[i].specular[1] = (GLfloat) 0.0;
		materials[i].specular[2] = (GLfloat) 0.0;
		materials[i].specular[3] = (GLfloat) 1.0;
		materials[i].IDtexture = -1;
	}
	materials[0].name = ::_strdup("default");
	
	nmaterials = 0;
	while (::fscanf(file, "%s", buf) != EOF)
	{
		switch (buf[0])
		{
		case '#':
			::fgets(buf, sizeof(buf), file);
			break;
		case 'n':
			::fgets(buf, sizeof(buf), file);
			::sscanf(buf, "%s %s", buf, buf);
			nmaterials++;
			materials[nmaterials].name = ::_strdup(buf);
			break;
		case 'N':
			if (buf[1] != 's') break;
			::fscanf(file, "%f", &materials[nmaterials].shininess);
			materials[nmaterials].shininess /= 1000.0;
			materials[nmaterials].shininess *= 128.0;
			break;
		case 'K':
			switch (buf[1])
			{
			case 'd':
				::fscanf(file, "%f %f %f", &materials[nmaterials].diffuse[0], &materials[nmaterials].diffuse[1], &materials[nmaterials].diffuse[2]);
				break;
			case 's':
				::fscanf(file, "%f %f %f", &materials[nmaterials].specular[0], &materials[nmaterials].specular[1], &materials[nmaterials].specular[2]);
				break;
			case 'a':
				::fscanf(file, "%f %f %f", &materials[nmaterials].ambient[0], &materials[nmaterials].ambient[1], &materials[nmaterials].ambient[2]);
				break;
			default:
				::fgets(buf, sizeof(buf), file);
				break;
			}
			break;
		case 'm':
			filename = (char*) ::malloc(FILENAME_MAX);
			::fgets(filename, FILENAME_MAX, file);
			texturename = ::_strdup(filename);
			::free(filename);
			if (::strncmp(buf, "map_Kd", 6) == 0) 
			{
				materials[nmaterials].IDtexture = FindOrAddTexture(texturename);
				::free(texturename);
			}
			else
				::free(texturename);
			break;
		default:
			::fgets(buf, sizeof(buf), file);
			break;
		}
	}
	return GL_TRUE;
}

GLvoid glModel::FirstPass(FILE* file)
{
	GLuint nvertices;
    GLuint nnormals;
    GLuint ntexcoords;
    GLuint ntriangles;
	glModelGroup* group;
	unsigned v, n, t;
	char buf[128];
	
	group = AddGroup("default");

	nvertices = nnormals = ntexcoords = ntriangles = 0;
	while (::fscanf(file, "%s", buf) != EOF)
	{
		switch (buf[0])
		{
		case '#':
			::fgets(buf, sizeof(buf), file);
			break;
		case 'v':
			switch (buf[1])
			{
			case '\0':
				::fgets(buf, sizeof(buf), file);
				nvertices++;
				break;
			case 'n':
				::fgets(buf, sizeof(buf), file);
				nnormals++;
				break;
			case 't':
				::fgets(buf, sizeof(buf), file);
				ntexcoords++;
				break;
			default:
				return;
				break;
			}
			break;
		case 'm':
			::fgets(buf, sizeof(buf), file);
			::sscanf(buf, "%s %s", buf, buf);
			mtllibname = ::_strdup(buf);
			if (ReadMTL(mtllibname) == GL_FALSE)
			{
			}
			break;
		case 'u':
			::fgets(buf, sizeof(buf), file);
			break;
		case 'g':
			::fgets(buf, sizeof(buf), file);
#if SINGLE_STRING_GROUP_NAMES
			::sscanf(buf, "%s", buf);
#else
			buf[::strlen(buf)-1] = '\0';
#endif
			group = AddGroup(buf);
			break;
		case 'f':
			v = n = t = 0;
			::fscanf(file, "%s", buf);
			if (::strstr(buf, "//"))
			{
				::sscanf(buf, "%d//%d", &v, &n);
				::fscanf(file, "%d//%d", &v, &n);
				::fscanf(file, "%d//%d", &v, &n);
				ntriangles++;
				group->numtriangles++;
				while(::fscanf(file, "%d//%d", &v, &n) > 0)
				{
					ntriangles++;
					group->numtriangles++;
				}
			}
			else
				if (::sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3)
				{
					::fscanf(file, "%d/%d/%d", &v, &t, &n);
					::fscanf(file, "%d/%d/%d", &v, &t, &n);
					ntriangles++;
					group->numtriangles++;
					while (::fscanf(file, "%d/%d/%d", &v, &t, &n) > 0)
					{
						ntriangles++;
						group->numtriangles++;
					}
				}
				else
					if (::sscanf(buf, "%d/%d", &v, &t) == 2)
					{
						::fscanf(file, "%d/%d", &v, &t);
						::fscanf(file, "%d/%d", &v, &t);
						ntriangles++;
						group->numtriangles++;
						while (::fscanf(file, "%d/%d", &v, &t) > 0)
						{
							ntriangles++;
							group->numtriangles++;
						}
					}
					else
					{
						::fscanf(file, "%d", &v);
						::fscanf(file, "%d", &v);
						ntriangles++;
						group->numtriangles++;
						while (::fscanf(file, "%d", &v) > 0)
						{
							ntriangles++;
							group->numtriangles++;
						}
					}
			break;
		default:
			::fgets(buf, sizeof(buf), file);
			break;
		}
	}

	numvertices = nvertices;
	numnormals = nnormals;
	numtexcoords = ntexcoords;
	numtriangles = ntriangles;
	
	group = groups;
	while (group)
	{
		group->triangles = (GLuint*) ::malloc(sizeof(GLuint) * group->numtriangles);
		group->numtriangles = 0;
		group = group->next;
	}
}

GLvoid glModel::SecondPass(FILE* file) 
{
	GLuint nvertices;
    GLuint nnormals;
    GLuint ntexcoords;
    GLuint ntriangles;
    GLfloat* avertices;
    GLfloat* anormals;
    GLfloat* atexcoords;
    glModelGroup* group;
    GLuint material;
    GLuint v, n, t;
    char buf[128];
	char afis[80];
	
	avertices = vertices;
    anormals = normals;
    atexcoords = texcoords;
    group = groups;

	nvertices = nnormals = ntexcoords = 1;
	ntriangles = 0;
	material = 0;
	
	while (::fscanf(file, "%s", buf) != EOF)
	{
		switch (buf[0])
		{
		case '#':
			::fgets(buf, sizeof(buf), file);
			break;
		case 'v':
			switch (buf[1])
			{
			case '\0':
				::fscanf(file, "%f %f %f", &avertices[3 * nvertices + 0], &avertices[3 * nvertices + 1], &avertices[3 * nvertices + 2]);
				nvertices++;
				break;
			case 'n':
				::fscanf(file, "%f %f %f", &anormals[3 * nnormals + 0], &anormals[3 * nnormals + 1], &anormals[3 * nnormals + 2]);
				nnormals++;
				break;
			case 't':
				::fscanf(file, "%f %f", &atexcoords[2 * ntexcoords + 0], &atexcoords[2 * ntexcoords + 1]);
				ntexcoords++;
				break;
			}
			break;
		case 'u':
			::fgets(buf, sizeof(buf), file);
			::sscanf(buf, "%s %s", buf, buf);
			group->material = material = FindMaterial(buf);
			break;
		case 'g':
			::fgets(buf, sizeof(buf), file);
#if SINGLE_STRING_GROUP_NAMES
			::sscanf(buf, "%s", buf);
#else
			buf[::strlen(buf) - 1] = '\0';
#endif
			group = FindGroup(buf);
			group->material = material;
			break;
		case 'f':
			v = n = t = 0;
			T(ntriangles).findex = -1;
			T(ntriangles).neighbor[0] = -1;
			T(ntriangles).neighbor[1] = -1;
			T(ntriangles).neighbor[2] = -1;
			::fscanf(file, "%s", buf);
			if (::strstr(buf, "//"))
			{
				::sscanf(buf, "%d//%d", &v, &n);
				T(ntriangles).vindices[0] = v;
				T(ntriangles).nindices[0] = n;
				::fscanf(file, "%d//%d", &v, &n);
				T(ntriangles).vindices[1] = v;
				T(ntriangles).nindices[1] = n;
				::fscanf(file, "%d//%d", &v, &n);
				T(ntriangles).vindices[2] = v;
				T(ntriangles).nindices[2] = n;
				group->triangles[group->numtriangles++] = ntriangles;
				ntriangles++;
				while (::fscanf(file, "%d//%d", &v, &n) > 0)
				{
					T(ntriangles).vindices[0] = T(ntriangles - 1).vindices[0];
					T(ntriangles).nindices[0] = T(ntriangles - 1).nindices[0];
					T(ntriangles).vindices[1] = T(ntriangles - 1).vindices[2];
					T(ntriangles).nindices[1] = T(ntriangles - 1).nindices[2];
					T(ntriangles).vindices[2] = v;
					T(ntriangles).nindices[2] = n;
					group->triangles[group->numtriangles++] = ntriangles;
					ntriangles++;
				}
			}
			else
				if (::sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3)
				{
					T(ntriangles).vindices[0] = v;
					T(ntriangles).tindices[0] = t;
					T(ntriangles).nindices[0] = n;
					::fscanf(file, "%d/%d/%d", &v, &t, &n);
					T(ntriangles).vindices[1] = v;
					T(ntriangles).tindices[1] = t;
					T(ntriangles).nindices[1] = n;
					::fscanf(file, "%d/%d/%d", &v, &t, &n);
					T(ntriangles).vindices[2] = v;
					T(ntriangles).tindices[2] = t;
					T(ntriangles).nindices[2] = n;
					group->triangles[group->numtriangles++] = ntriangles;
					ntriangles++;					
					while (::fscanf(file, "%d/%d/%d", &v, &t, &n) > 0)
					{
						T(ntriangles).vindices[0] = T(ntriangles - 1).vindices[0];
						T(ntriangles).tindices[0] = T(ntriangles - 1).tindices[0];
						T(ntriangles).nindices[0] = T(ntriangles - 1).nindices[0];
						T(ntriangles).vindices[1] = T(ntriangles - 1).vindices[2];
						T(ntriangles).tindices[1] = T(ntriangles - 1).tindices[2];
						T(ntriangles).nindices[1] = T(ntriangles - 1).nindices[2];
						T(ntriangles).vindices[2] = v;
						T(ntriangles).tindices[2] = t;
						T(ntriangles).nindices[2] = n;
						group->triangles[group->numtriangles++] = ntriangles;
						ntriangles++;
					}
				}
				else
					if (::sscanf(buf, "%d/%d", &v, &t) == 2)
					{
						T(ntriangles).vindices[0] = v;
						T(ntriangles).tindices[0] = t;
						::fscanf(file, "%d/%d", &v, &t);
						T(ntriangles).vindices[1] = v;
						T(ntriangles).tindices[1] = t;
						::fscanf(file, "%d/%d", &v, &t);
						T(ntriangles).vindices[2] = v;
						T(ntriangles).tindices[2] = t;
						group->triangles[group->numtriangles++] = ntriangles;
						ntriangles++;
						while (::fscanf(file, "%d/%d", &v, &t) > 0)
						{
							T(ntriangles).vindices[0] = T(ntriangles - 1).vindices[0];
							T(ntriangles).tindices[0] = T(ntriangles - 1).tindices[0];
							T(ntriangles).vindices[1] = T(ntriangles - 1).vindices[2];
							T(ntriangles).tindices[1] = T(ntriangles - 1).tindices[2];
							T(ntriangles).vindices[2] = v;
							T(ntriangles).tindices[2] = t;
							group->triangles[group->numtriangles++] = ntriangles;
							ntriangles++;
						}
					}
					else
					{
						::sscanf(buf, "%d", &v);
						T(ntriangles).vindices[0] = v;
						::fscanf(file, "%d", &v);
						T(ntriangles).vindices[1] = v;
						::fscanf(file, "%d", &v);
						T(ntriangles).vindices[2] = v;
						group->triangles[group->numtriangles++] = ntriangles;
						ntriangles++;
						while (::fscanf(file, "%d", &v) > 0)
						{
							T(ntriangles).vindices[0] = T(ntriangles - 1).vindices[0];
							T(ntriangles).vindices[1] = T(ntriangles - 1).vindices[2];
							T(ntriangles).vindices[2] = v;
							group->triangles[group->numtriangles++] = ntriangles;
							ntriangles++;
						}
					}
			break;
		default:
			::fgets(buf, sizeof(buf), file);
			break;
		}
	}
}

glModelGroup* glModel::AddGroup(const char* name)
{
	glModelGroup* group;
	group = FindGroup(name);
	if (!group)
	{
		group = (glModelGroup*) ::malloc(sizeof(glModelGroup));
		group->name = ::_strdup(name);
		group->material = 0;
		group->numtriangles = 0;
		group->triangles = NULL;
		group->next = groups;
		groups = group;
		numgroups++;
	}
	return group;
}

glModelGroup* glModel::FindGroup(const char* name)
{
	glModelGroup* group;
	group = groups;
	while (group)
	{
		if (!::strcmp(name, group->name))
			break;
		group = group->next;
	}
	return group;
}

GLuint glModel::FindMaterial(const char* name)
{
	GLuint uret;

	for (uret = 0; uret < nummaterials; uret++)
		if (!::strcmp(materials[uret].name, name))
			goto found;
	
	uret = 0;
found:
	return uret;
}

GLuint glModel::FindOrAddTexture(char* name)
{
	char *dir, *filename;
	float width, height;
	
	char *numefis = name;
	while (*numefis == ' ') numefis++;
	
	for (GLuint i = 0; i < numtextures; i++)
	{
		if (!::strcmp(textures[i].name, numefis))
			return i;
	}

	if (::strstr(name, ":\\"))
	{
		filename = (char*) ::malloc(sizeof(char) * (::strlen(name) + 1));
		::strcpy(filename, name);
	}
	else
	{
		dir = ExtractDirName(pathname);
		filename = (char*) ::malloc(sizeof(char) * (::strlen(dir) + ::strlen(numefis) + 1));
		::strcpy(filename, dir);
		::strcat(filename, numefis);
		::free(dir);
	}
	int lung = ::strlen(filename);
	if (filename[lung - 1] < 32) filename[lung - 1] = 0;
	if (filename[lung - 2] < 32) filename[lung - 2] = 0;    
	
	numtextures++;
	textures = (glModelTexture*) ::realloc(textures, sizeof(glModelTexture) * numtextures);
	textures[numtextures - 1].name = ::_strdup(numefis);
	glTexture tex_temp;
	textures[numtextures - 1].id = tex_temp.PushTexture(filename, &width, &height);
	textures[numtextures - 1].width = width;
	textures[numtextures - 1].height = height;
	
	::free(filename);
	return (numtextures - 1);
}

char* glModel::ExtractDirName(const char* path)
{
	char* dir;
	char* s;
	
	dir = ::_strdup(path);
	s = ::strrchr(dir, '/');
	
	if (s)
		s[1] = '\0';
	else
	{
		s = ::strrchr(dir, '\\');
		if (s)
			s[1] = '\0';
		else
			dir[0] = '\0';
	}
	return dir;
}

GLfloat glModel::Absolute(GLfloat f)
{
	if (f < 0)
		return -f;
	return f;
}

GLfloat glModel::FloatMax(GLfloat a, GLfloat b)
{
	if (b > a)
		return b;
	return a;
}

GLfloat glModel::Dot(GLfloat* u, GLfloat* v)
{
	assert(u); assert(v);
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

GLvoid glModel::Cross(GLfloat* u, GLfloat* v, GLfloat* n)
{
	assert(u); assert(v); assert(n);
	n[0] = u[1] * v[2] - u[2] * v[1];
	n[1] = u[2] * v[0] - u[0] * v[2];
	n[2] = u[0] * v[1] - u[1] * v[0];
}

GLvoid glModel::Normalize(GLfloat* v)
{
	GLfloat shift;	
	assert(v);	
	shift = (GLfloat)::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] /= shift;
	v[1] /= shift;
	v[2] /= shift;
}

GLboolean glModel::Equal(GLfloat* u, GLfloat* v, GLfloat epsilon)
{
	if (Absolute(u[0] - v[0]) < epsilon && Absolute(u[1] - v[1]) < epsilon && Absolute(u[2] - v[2]) < epsilon)
		return GL_TRUE;
	return GL_FALSE;
}

GLfloat* glModel::WeldVectors(GLfloat* vectors, GLuint* numvectors, GLfloat epsilon)
{
	GLfloat* copies;
	GLuint copied;
	GLuint i, j;

	copies = (GLfloat*) ::malloc(sizeof(GLfloat) * 3 * (*numvectors + 1));
	::memcpy(copies, vectors, (sizeof(GLfloat) * 3 * (*numvectors + 1)));
	copied = 1;
	for (i = 1; i <= *numvectors; i++)
	{
		for (j = 1; j <= copied; j++)
		{
			if (Equal(&vectors[3 * i], &copies[3 * j], epsilon))
				goto duplicate;
		}
		
		copies[3 * copied + 0] = vectors[3 * i + 0];
		copies[3 * copied + 1] = vectors[3 * i + 1];
		copies[3 * copied + 2] = vectors[3 * i + 2];
		j = copied;
		copied++;

duplicate:
		vectors[3 * i + 0] = (GLfloat)j;
	}
	*numvectors = copied-1;
	return copies;
}

#if 0
GLboolean glModel::Save(const char* filename, GLuint mode, const char* setmtlname)
{
	FILE* file;
	glModelGroup* group;

	if (mode & GLMODEL_FLAT && !facetnorms) mode &= ~GLMODEL_FLAT;
	if (mode & GLMODEL_SMOOTH && !normals) mode &= ~GLMODEL_SMOOTH;
	if (mode & GLMODEL_TEXTURE && !texcoords) mode &= ~GLMODEL_TEXTURE;
	if (mode & GLMODEL_FLAT && mode & GLMODEL_SMOOTH) mode &= ~GLMODEL_FLAT;
	if (mode & GLMODEL_COLOR && !materials) mode &= ~GLMODEL_COLOR;
	if (mode & GLMODEL_MATERIAL && !materials) mode &= ~GLMODEL_MATERIAL;
	if (mode & GLMODEL_COLOR && mode & GLMODEL_MATERIAL) mode &= ~GLMODEL_COLOR;

	if (::fopen_s(&file, filename, "w") != 0)
		return GL_FALSE;

	::fprintf(file, "#  \n#  Wavefront OBJ generated by codesaur::glModel class\n#  \n");
	
	if (mode & GLMODEL_MATERIAL)
	{
		if (setmtlname)
		{
			::fprintf(file, "\nmtllib %s\n", setmtlname);
			WriteMTL(filename, setmtlname);
		}
		else
			if (mtllibname)
			{
				::fprintf(file, "\nmtllib %s\n", mtllibname);
				WriteMTL(filename, mtllibname);
			}
	}
	
	::fprintf(file, "\n");
	::fprintf(file, "# %d vertices\n", numvertices);
	for (GLuint i = 1; i <= numvertices; i++)
		::fprintf(file, "v %f %f %f\n", vertices[3 * i + 0], vertices[3 * i + 1], vertices[3 * i + 2]);
	
	if (mode & GLMODEL_SMOOTH)
	{
		::fprintf(file, "\n");
		::fprintf(file, "# %d normals\n", numnormals);
		for (GLuint i = 1; i <= numnormals; i++)
			::fprintf(file, "vn %f %f %f\n", normals[3 * i + 0], normals[3 * i + 1], normals[3 * i + 2]);
	}
	else
		if (mode & GLMODEL_FLAT)
		{
			::fprintf(file, "\n");
			::fprintf(file, "# %d normals\n", numfacetnorms);
			for (GLuint i = 1; i <= numnormals; i++)
				::fprintf(file, "vn %f %f %f\n", facetnorms[3 * i + 0], facetnorms[3 * i + 1], facetnorms[3 * i + 2]);
		}
	
	if (mode & GLMODEL_TEXTURE)
	{
		::fprintf(file, "\n");
		::fprintf(file, "# %d texcoords\n", texcoords);
		for (GLuint i = 1; i <= numtexcoords; i++)
			::fprintf(file, "vt %f %f\n", texcoords[2 * i + 0], texcoords[2 * i + 1]);
	}
	
	::fprintf(file, "\n");
	::fprintf(file, "# %d groups\n", numgroups);
	::fprintf(file, "# %d faces (triangles)\n", numtriangles);
	::fprintf(file, "\n");
	
	group = groups;	
	while (group)
	{
		::fprintf(file, "g %s\n", group->name);
		if (mode & GLMODEL_MATERIAL)
			::fprintf(file, "usemtl %s\n", materials[group->material].name);
		for (GLuint i = 0; i < group->numtriangles; i++)
		{
			if (mode & GLMODEL_SMOOTH && mode & GLMODEL_TEXTURE)
			{
				::fprintf(file, "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
					T(group->triangles[i]).vindices[0], 
					T(group->triangles[i]).nindices[0], 
					T(group->triangles[i]).tindices[0],
					T(group->triangles[i]).vindices[1],
					T(group->triangles[i]).nindices[1],
					T(group->triangles[i]).tindices[1],
					T(group->triangles[i]).vindices[2],
					T(group->triangles[i]).nindices[2],
					T(group->triangles[i]).tindices[2]);
			}
			else
				if (mode & GLMODEL_FLAT && mode & GLMODEL_TEXTURE)
				{
					::fprintf(file, "f %d/%d %d/%d %d/%d\n",
						T(group->triangles[i]).vindices[0],
						T(group->triangles[i]).findex,
						T(group->triangles[i]).vindices[1],
						T(group->triangles[i]).findex,
						T(group->triangles[i]).vindices[2],
						T(group->triangles[i]).findex);
				}
				else
					if (mode & GLMODEL_TEXTURE) {
						::fprintf(file, "f %d/%d %d/%d %d/%d\n",
							T(group->triangles[i]).vindices[0],
							T(group->triangles[i]).tindices[0],
							T(group->triangles[i]).vindices[1],
							T(group->triangles[i]).tindices[1],
							T(group->triangles[i]).vindices[2],
							T(group->triangles[i]).tindices[2]);
					}
					else
						if (mode & GLMODEL_SMOOTH)
						{
							::fprintf(file, "f %d//%d %d//%d %d//%d\n",
								T(group->triangles[i]).vindices[0],
								T(group->triangles[i]).nindices[0],
								T(group->triangles[i]).vindices[1],
								T(group->triangles[i]).nindices[1],
								T(group->triangles[i]).vindices[2], 
								T(group->triangles[i]).nindices[2]);
						}
						else
							if (mode & GLMODEL_FLAT)
							{
								::fprintf(file, "f %d//%d %d//%d %d//%d\n",
									T(group->triangles[i]).vindices[0],
									T(group->triangles[i]).findex,
									T(group->triangles[i]).vindices[1],
									T(group->triangles[i]).findex,
									T(group->triangles[i]).vindices[2],
									T(group->triangles[i]).findex);
							}
							else
							{
								fprintf(file, "f %d %d %d\n",
									T(group->triangles[i]).vindices[0],
									T(group->triangles[i]).vindices[1],
									T(group->triangles[i]).vindices[2]);
							}
		}
		::fprintf(file, "\n");
		group = group->next;
	}
	::fclose(file);
	return GL_TRUE;
}

GLvoid glModel::WriteMTL(const char* modelpath, const char* name)
{
	FILE* file;
	char* dir;
	char* filename;
	glModelMaterial* material;
		
	dir = ExtractDirName(modelpath);
	filename = (char*) ::malloc(sizeof(char) * (::strlen(dir) + ::strlen(name) + 1));
	::strcpy(filename, dir);
	::strcat(filename, name);
	::free(dir);
	
	if (::fopen_s(&file, filename, "w") != 0)
		return;

	::free(filename);
	
	::fprintf(file, "#  \n#  Wavefront MTL generated by codesaur::glModel class\n#  \n");
	
	for (GLuint i = 0; i < nummaterials; i++)
	{
		material = &materials[i];
		::fprintf(file, "\nnewmtl %s\n", material->name);
		::fprintf(file, "Ka %f %f %f\n", material->ambient[0], material->ambient[1], material->ambient[2]);
		::fprintf(file, "Kd %f %f %f\n", material->diffuse[0], material->diffuse[1], material->diffuse[2]);
		::fprintf(file, "Ks %f %f %f\n", material->specular[0],material->specular[1],material->specular[2]);
		::fprintf(file, "Ns %f\n", material->shininess / (GLfloat) 128.0 * (GLfloat) 1000.0);
	}
}
#endif