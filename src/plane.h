

#ifndef __PLANE__
#define __PLANE__
#include "mesh.h"
#include "vec3.h"

class Plane {
public:
	Mesh* body;
	Mesh* cockpit;
	Mesh* propeller;
	vec3 position;
	GLuint bodyTexture;
	float propellerRotation;
	float propellerSpeed;
	int health;
public:

	Plane(Mesh* body, Mesh* cockpit, Mesh* propeller, GLuint bodyTexture, const vec3& pos);
	~Plane();
	void Draw(const vec3& campos, const vec3& lightDir);
	void Update(float timeDelta);
	void TakeHit();
};


#endif
