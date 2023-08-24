
#include "matrix.h"
#include <GL/glew.h>

Matrix::Matrix()
{
	for(int i = 0; i < 16; ++i) m[i] = 0.0f;	
}

void Matrix::MakeIdentity()
{
	for(int i = 0; i < 16; ++i) m[i] = 0.0f;
	m[0] = 1.0f;
	m[5] = 1.0f;
	m[10] = 1.0f;
	m[15] = 1.0f;
}

void Matrix::MakeModelMatrix(const vec3& t, const vec3& r, const vec3& s)
{
	// use opengl to calculate the model matrix...
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(t.x, t.y, t.z);
	glRotatef(r.x, 1, 0, 0);
	glRotatef(r.y, 0, 1, 0);
	glRotatef(r.z, 0, 0, 1);
	glScalef(s.x, s.y, s.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
}


void Matrix::MakeTranslationMatrix(const vec3& trans)
{
	MakeIdentity();
	m[12] = trans.x;
	m[13] = trans.y;
	m[14] = trans.z;
}

void Matrix::MakeScalingMatrix(const vec3& scale)
{
	MakeIdentity();
	m[0] = scale.x;
	m[5] = scale.y;
	m[10] = scale.z;
}

void Matrix::MakeProjectionMatrix(float yFov, float aspect, float zNear, float zFar)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective(yFov, aspect, zNear, zFar);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
}

void Matrix::MakeViewMatrix(const vec3& eye, const vec3& lookAt)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
}


Matrix Matrix::Multiply(const Matrix& other) const
{
	Matrix out;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);
	glMultMatrixf(other.m);
	glGetFloatv(GL_MODELVIEW_MATRIX, out.m);
	return out;
}
