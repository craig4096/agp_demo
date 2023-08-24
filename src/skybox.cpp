

#include "skybox.h"
#include "shaders.h"
#include "texture.h"
#include <iostream>
using namespace std;



Skybox::Skybox(const char* textures[6])
{
	// create the vertex buffer
	float verts[] = {
		1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0,		// posx
		-1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0,	// negx
		1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0,		// posy
		1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0,	// negy
		1.0, -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0,		// posz
		-1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0	// negz
	};

	glGenBuffers(1, &vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);


	float texcoords[] = {
		1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,
		1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,
		1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,
		1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,
		1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,
		1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0
	};

	glGenBuffers(1, &this->texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, this->texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

	this->textures[0] = LoadSkyboxTexture(textures[0]);
	this->textures[1] = LoadSkyboxTexture(textures[1]);
	this->textures[2] = LoadSkyboxTexture(textures[2]);
	this->textures[3] = LoadSkyboxTexture(textures[3]);
	this->textures[4] = LoadSkyboxTexture(textures[4]);
	this->textures[5] = LoadSkyboxTexture(textures[5]);
}

Skybox::~Skybox()
{
	glDeleteBuffers(1, &vertices);
	glDeleteBuffers(1, &texcoords);
	for(int i = 0; i < 6; ++i) {
		glDeleteTextures(1, &textures[i]);
	}
}


void Skybox::Draw(const vec3& campos, float scale)
{
	Matrix model;
	model.MakeModelMatrix(campos, vec3(0,0,0), vec3(scale, scale, scale));
	CustomGL::Get().SetModelMatrix(model);

	glEnableVertexAttribArray(0); // pos
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1); // texcoord
	glBindBuffer(GL_ARRAY_BUFFER, texcoords);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	shaders::Set(shaders::TEXTURED);
	for(int i = 0; i < 6; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glDrawArrays(GL_QUADS, i*4, 4);
	}
	shaders::Unset();
	glBindTexture(GL_TEXTURE_2D, -1);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	CustomGL::Get().SetModelMatrixToIdentity();
}

