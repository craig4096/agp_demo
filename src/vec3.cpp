

#include "vec3.h"
#include <math.h>

vec3::vec3() : x(0.0f), y(0.0f), z(0.0f) {}
vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) {}

vec3 vec3::operator +(const vec3& other) const
{
	return vec3(x + other.x, y + other.y, z + other.z);
}

vec3 vec3::operator -(const vec3& other) const
{
	return vec3(x - other.x, y - other.y, z - other.z);
}

vec3 vec3::operator *(float s) const
{
	return vec3(x*s,y*s,z*s);
}

void vec3::Normalize() {
	float mag = sqrt(x*x+y*y+z*z);
	if(mag == 0.0f) {
		x = y = z = 0.0f;
	} else {
		float magrecip = 1.0f / mag;
		x *= magrecip;
		y *= magrecip;
		z *= magrecip;
	}
}
