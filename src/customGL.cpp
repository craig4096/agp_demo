

#include "customGL.h"

CustomGL CustomGL::instance;

CustomGL::CustomGL()
{
}

CustomGL::~CustomGL()
{
}

void CustomGL::SetProjectionMatrix(const Matrix& m)
{
	this->projectionMatrix = m;
	CalculateModelViewProjectionMatrix();
}

void CustomGL::SetViewMatrix(const Matrix& m)
{
	this->viewMatrix = m;
	CalculateModelViewProjectionMatrix();
}

void CustomGL::SetModelMatrix(const Matrix& m)
{
	this->modelMatrix = m;
	CalculateModelViewProjectionMatrix();
}

void CustomGL::CalculateModelViewProjectionMatrix()
{
	// use opengl again...
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(projectionMatrix.m);
	glMultMatrixf(viewMatrix.m);
	glMultMatrixf(modelMatrix.m);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewProjectionMatrix.m);
}

void CustomGL::SetModelMatrixToIdentity()
{
	modelMatrix.MakeIdentity();
	CalculateModelViewProjectionMatrix();
}


