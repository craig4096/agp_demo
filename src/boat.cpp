

#include "boat.h"

Boat::Boat(Mesh* mesh, GLuint texture)
	: health(100)
	, maxHealth(100)
	, mesh(mesh)
	, texture(texture)
{
}

Boat::~Boat()
{
}


void Boat::Draw(const vec3& lightDir)
{
	Matrix m;
	m.MakeModelMatrix(position, vec3(0,180,0), vec3(1,1,1));
	CustomGL::Get().SetModelMatrix(m);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	shaders::Set(shaders::DIRECTIONAL_LIGHT_TEXTURE);
	glUniform3f(glGetUniformLocation(shaders::GetProgram(), "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform1i(glGetUniformLocation(shaders::GetProgram(), "tex"), 0);
	mesh->Draw();
	shaders::Unset();
	glBindTexture(GL_TEXTURE_2D, -1);
	CustomGL::Get().SetModelMatrixToIdentity();


	// draw the billboard health bar above the boat
	if(health > 0 && health < maxHealth)
	{
		Matrix m = CustomGL::Get().GetViewMatrix();
		// transpose
		std::swap(m.m[1], m.m[4]);
		std::swap(m.m[2], m.m[8]);
		std::swap(m.m[6], m.m[9]);
		m.m[12] = position.x;
		m.m[13] = position.y + 5;
		m.m[14] = position.z;
		CustomGL::Get().SetModelMatrix(m);

		static const float sizeX = 3.0;
		static const float sizeY = 0.3;
		float midX = -sizeX + (sizeX*2*((float)health / (float)maxHealth));

		// now draw the health quad
		shaders::Set(shaders::COLOR);
		glUniform4f(glGetUniformLocation(shaders::GetProgram(), "color"), 0, 1, 0, 0);
		glBegin(GL_QUADS);
			glVertex3f(-sizeX, -sizeY, 0);
			glVertex3f(midX, -sizeY, 0);
			glVertex3f(midX, sizeY, 0);
			glVertex3f(-sizeX, sizeY, 0);
		glEnd();
		glUniform4f(glGetUniformLocation(shaders::GetProgram(), "color"), 1, 0, 0, 0);
		glBegin(GL_QUADS);
			glVertex3f(midX, -sizeY, 0);
			glVertex3f(sizeX, -sizeY, 0);
			glVertex3f(sizeX, sizeY, 0);
			glVertex3f(midX, sizeY, 0);
		glEnd();
	}
	shaders::Unset();
}


void Boat::Update(float timeStep)
{
	if(health <= 0)
	{
		position.y -= 1.0f * timeStep;
		if(position.y <= -2.0f)
			position.y = -2.0f;
	}
}

void Boat::Reset(int health)
{
	this->health = health;
	this->maxHealth = health;
	position.y = 0;
}


bool Boat::PointInside(const vec3& point) const
{
	vec3 tmp = point - position;
	return tmp.x > -3.0f && tmp.x < 3.0f &&
		tmp.y > 0.0f && tmp.y < 4.0f &&
		tmp.z > -7.0f && tmp.z < 7.0f;
}

void Boat::TakeHit(int hit)
{
	health -= hit;
}

