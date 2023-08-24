

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "plane.h"
#include "texture.h"
#include "math3d.h"


Plane::Plane(Mesh* body, Mesh* cockpit, Mesh* propeller, GLuint bodyTexture, const vec3& pos)
	: body(body)
	, cockpit(cockpit)
	, propeller(propeller)
	, bodyTexture(bodyTexture)
{
	position = pos;
	propellerSpeed = 3000.0f;
	propellerRotation = 0.0f;
}

Plane::~Plane()
{
}

void Plane::Draw(const vec3& campos, const vec3& lightDir)
{
	Matrix bodyMat, propelMat;

	// draw body
	glBindTexture(GL_TEXTURE_2D, bodyTexture);
	bodyMat.MakeModelMatrix(position, vec3(0,0,0), vec3(1,1,1));
	CustomGL::Get().SetModelMatrix(bodyMat);
	shaders::Set(shaders::DIRECTIONAL_LIGHT_TEXTURE);
	glUniform3f(glGetUniformLocation(shaders::GetProgram(), "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform1i(glGetUniformLocation(shaders::GetProgram(), "tex"), 0);
	body->Draw();

	// draw cockpit
	shaders::Set(shaders::REFLECTION_MAP);
	glUniform1i(glGetUniformLocation(shaders::GetProgram(), "cubemap"), 0);
	glUniform3f(glGetUniformLocation(shaders::GetProgram(), "campos"), campos.x, campos.y, campos.z);
	cockpit->Draw();

	// draw propeller
	propelMat.MakeModelMatrix(vec3(0,0,0), vec3(0,0,propellerRotation), vec3(1,1,1));
	CustomGL::Get().SetModelMatrix(bodyMat.Multiply(propelMat));
	shaders::Set(shaders::DIRECTIONAL_LIGHT_TEXTURE);
	glUniform3f(glGetUniformLocation(shaders::GetProgram(), "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform1i(glGetUniformLocation(shaders::GetProgram(), "tex"), 0);
	propeller->Draw();
	glBindTexture(GL_TEXTURE_2D, -1);

	shaders::Unset();
	CustomGL::Get().SetModelMatrixToIdentity();
}

void Plane::Update(float timeStep)
{
	propellerRotation += propellerSpeed * timeStep;

	//rotation += 30 * timeStep;
	//static const float dist = 20;
	//position.x = sin(rotation * DEG_TO_RAD) * dist;
	//position.z = cos(rotation * DEG_TO_RAD) * dist;

	//yaw = rotation-90;

	position.z += -5.0 * timeStep;
}
