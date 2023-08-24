

#ifndef __VEC3__
#define __VEC3__

class vec3 {
public:
	float x, y, z;

	vec3();
	vec3(float x, float y, float z);

	vec3 operator +(const vec3&) const;
	vec3 operator -(const vec3&) const;
	vec3 operator *(float) const;

	void Normalize();
};

#endif
