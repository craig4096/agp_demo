

#ifndef __WATER__
#define __WATER__

#include "customGL.h"
#include <string>
#include "vec3.h"

class Water {
	GLuint normalMap;
	float s, t;
	float speed; // how fast the water appears to be moving
	float uvScale;
	float size;
public:

	Water(const std::string& normalMap, float uvScale, float waterSpeed, float size);
	~Water();

	void Draw(const vec3& campos);
	void Update(float timeStep);
};


#endif
