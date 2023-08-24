
#include "water.h"
#include "shaders.h"
#include "texture.h"
using namespace std;

Water::Water(const string& normalMap, float uvScale, float speed, float size)
	: speed(speed)
	, s(0.0f)
	, t(0.0f)
	, uvScale(uvScale)
	, size(size)
{
	this->normalMap = LoadStdTexture(normalMap);
}

Water::~Water()
{
	glDeleteTextures(1, &normalMap);
}

void Water::Draw(const vec3& campos)
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	shaders::Set(shaders::WATER);
	glUniform1i(glGetUniformLocation(shaders::GetProgram(), "cubemap"), 0);
	glUniform1i(glGetUniformLocation(shaders::GetProgram(), "tex"), 1);
	glUniform3f(glGetUniformLocation(shaders::GetProgram(), "campos"), campos.x, campos.y, campos.z);
	glUniform1f(glGetUniformLocation(shaders::GetProgram(), "sOffset"), s);
	glUniform1f(glGetUniformLocation(shaders::GetProgram(), "tOffset"), t);

	GLuint posloc = glGetAttribLocation(shaders::GetProgram(), "pos");
	GLuint uvloc = glGetAttribLocation(shaders::GetProgram(), "texcoord");

	glBegin(GL_QUADS);
	glVertexAttrib2f(uvloc, 0.0, 0.0);
	glVertexAttrib3f(posloc, -size, 0.0, -size);
	glVertexAttrib2f(uvloc, 0.0, uvScale);
	glVertexAttrib3f(posloc, -size, 0.0, size);
	glVertexAttrib2f(uvloc, uvScale, uvScale);
	glVertexAttrib3f(posloc, size, 0.0, size);
	glVertexAttrib2f(uvloc, uvScale, 0.0);
	glVertexAttrib3f(posloc, size, 0.0, -size);
	glEnd();
	shaders::Unset();
	glBindTexture(GL_TEXTURE_2D, -1);

	glActiveTexture(GL_TEXTURE0);
}


void Water::Update(float timeStep)
{
	s += (speed+0.01) * timeStep;
	t += speed * timeStep;

	while(s >= 1.0f) s -= 1.0f;
	while(t >= 1.0f) t -= 1.0f;
}
