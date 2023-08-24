

#ifndef __BOAT__
#define __BOAT__

#include "mesh.h"
#include "customGL.h"
#include "vec3.h"

class Boat {
	Mesh* mesh;
	GLuint texture;
	vec3 position;
	int health;
	int maxHealth;
public:

	Boat(Mesh*, GLuint texture);
	~Boat();

	void Update(float timeStep);
	void Reset(int newHealth);
	void SetPosition(const vec3& pos) { position = pos; }
	void Draw(const vec3& lightDir);
	bool PointInside(const vec3& point) const;
	void TakeHit(int hit);
};


#endif
