

#ifndef __MATRIX__
#define __MATRIX__

#include "vec3.h"


class Matrix {
public:
	float m[16];

	Matrix();

	// sets the matrix based on translation
	// rotation and scale factors
	void MakeModelMatrix(const vec3& translation,
				const vec3& rotation,
				const vec3& scale);

	void MakeViewMatrix(const vec3& eye, const vec3& lookAt);
	void MakeProjectionMatrix(float yFov, float aspect, float zNear, float zFar);
	void MakeTranslationMatrix(const vec3& trans);
	void MakeScalingMatrix(const vec3& scale);
	void MakeIdentity();

	Matrix Multiply(const Matrix& other) const;
};


#endif
