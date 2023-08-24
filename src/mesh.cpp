

#include "mesh.h"
#include <fstream>
#include <vector>
#include <stdexcept>

Mesh::Mesh(const std::string& filename)
{
	std::ifstream ifs(filename.c_str());
	if(!ifs.good())
	{
		throw std::logic_error("could not open mesh file");
	}

	int nVerts;
	ifs >> nVerts;

	std::vector<float> verts(nVerts*3);
	std::vector<float> norms(nVerts*3);
	std::vector<float> uvs(nVerts*2);


	for(int i = 0; i < verts.size(); ++i)
	{
		ifs >> verts[i];
	}

	for(int i = 0; i < norms.size(); ++i)
	{
		ifs >> norms[i];
	}

	for(int i = 0; i < uvs.size(); ++i)
	{
		ifs >> uvs[i];
	}

	ifs >> indexCount; // tri count

	indexCount *= 3;
	std::vector<unsigned int> inds(indexCount);



	for(int i = 0; i < indexCount; ++i)
	{
		ifs >> inds[i];
	}

	// create the buffer objects
	glGenBuffers(1, &this->vertices);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertices);
	glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(float), &verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->normals);
	glBindBuffer(GL_ARRAY_BUFFER, this->normals);
	glBufferData(GL_ARRAY_BUFFER, norms.size()*sizeof(float), &norms[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, this->texcoords);
	glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(float), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size()*sizeof(int), &inds[0], GL_STATIC_DRAW);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &vertices);
	glDeleteBuffers(1, &indices);
	glDeleteBuffers(1, &normals);
	glDeleteBuffers(1, &texcoords);
}

void Mesh::Draw()
{
	GLuint posLoc = glGetAttribLocation(shaders::GetProgram(), "pos");
	GLuint normLoc = glGetAttribLocation(shaders::GetProgram(), "normal");
	GLuint uvLoc = glGetAttribLocation(shaders::GetProgram(), "texcoord");

	glEnableVertexAttribArray(posLoc);
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(normLoc);
	glBindBuffer(GL_ARRAY_BUFFER, normals);
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(uvLoc);
	glBindBuffer(GL_ARRAY_BUFFER, texcoords);
	glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(uvLoc);
	glDisableVertexAttribArray(normLoc);
	glDisableVertexAttribArray(posLoc);
}


