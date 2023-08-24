
#ifndef __CUSTOM_GL__
#define __CUSTOM_GL__

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "vec3.h"
#include "matrix.h"

class CustomGL {

	Matrix viewMatrix;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix modelViewProjectionMatrix;

	CustomGL();
	~CustomGL();

	static CustomGL instance;

	void CalculateModelViewProjectionMatrix();

public:

	static CustomGL& Get() { return instance; }

	void SetProjectionMatrix(const Matrix&);
	void SetViewMatrix(const Matrix&);
	void SetModelMatrix(const Matrix&);

	const Matrix& GetModelMatrix() const { return modelMatrix; }
	const Matrix& GetViewMatrix() const { return viewMatrix; }
	const Matrix& GetProjectionMatrix() const { return projectionMatrix; }
	const Matrix& GetModelViewProjectionMatrix() const { return modelViewProjectionMatrix; }

	void SetModelMatrixToIdentity();
};


#endif
