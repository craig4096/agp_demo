

#ifndef __GUN__
#define __GUN__

#include "mesh.h"
#include <vector>
#include "sound.h"
#include "boat.h"

class Gun {

	Mesh* gunfire;
	Mesh* barrels;
	Mesh* guards;
	GLuint guardTexture;
	float rotation;
	float rotationalVelocity;
	bool firing;
	float bulletsPerSecond;
	float bulletTime;
	Mesh* laser;
	Sound* fireSound;
	GLuint gunFireTexture;
	bool bulletFired;

	float gunFireAnimationFrame;

	struct Bullet {
		Bullet():hasHit(true) {}
		vec3 position;
		vec3 velocity;
		Matrix rotationMatrix;
		bool hasHit;
	};
	std::vector<Bullet> bullets;
	int currentBullet;
	float timeAccumulator;
	vec3 offset;

	void FireBullet();

public:
	vec3 campos;
	vec3 camdir;
	float yRot, xRot;

	Gun(int bulletsPerSecond);
	~Gun();

	void StartFire();
	void EndFire();

	void CheckBoat(Boat* b);
	void Draw(const vec3& campos, const vec3& lightDir);
	void Update(float timeStep);
};


#endif
