
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shaders.h"
#include "gun.h"
#include <algorithm>
#include "texture.h"

Gun::Gun(int bulletsPerSec)
	: rotation(0.0f)
	, firing(false)
	, rotationalVelocity(0.0f)
	, timeAccumulator(0.0f)
	, bulletsPerSecond((float)bulletsPerSec)
	, gunFireAnimationFrame(19.0f)
	, bulletFired(false)
{
	bulletTime = 1.0f / bulletsPerSecond;
	barrels = new Mesh("barrels.txt");
	guards = new Mesh("guards.txt");
	laser = new Mesh("laser.txt");
	guardTexture = LoadStdTexture("gun.bmp");
	bullets.resize(100);
	currentBullet = 0;
	offset = vec3(0.02, -0.07, -0.04);
	fireSound = new Sound("fire.wav", false);
	gunfire = new Mesh("gunfire.txt");
	gunFireTexture = LoadStdTexture("gunfire.bmp");
}

Gun::~Gun()
{
	glDeleteTextures(1, &guardTexture);
	glDeleteTextures(1, &gunFireTexture);
	delete barrels;
	delete guards;
	delete laser;
	delete fireSound;
	delete gunfire;
}

// for the gun firing animation - these are the offset st coordinates
// for each frame of the animation
float offsets[20][2] = {
	{ 0.0f, 0.8f, },
	{ 0.25f, 0.8f },
	{ 0.50f, 0.8f },
	{ 0.75f, 0.8f },
	{ 0.0f, 0.6f, },
	{ 0.25f, 0.6f },
	{ 0.50f, 0.6f },
	{ 0.75f, 0.6f },
	{ 0.0f, 0.4f, },
	{ 0.25f, 0.4f },
	{ 0.50f, 0.4f },
	{ 0.75f, 0.4f },
	{ 0.0f, 0.2f, },
	{ 0.25f, 0.2f },
	{ 0.50f, 0.2f },
	{ 0.75f, 0.2f },
	{ 0.0f, 0.0f, },
	{ 0.25f, 0.0f },
	{ 0.50f, 0.0f },
	{ 0.75f, 0.0f }
};


void Gun::Draw(const vec3& campos, const vec3& lightDir)
{
	// get the view matrix
	Matrix view = CustomGL::Get().GetViewMatrix();

	// transpose the top left 3x3
	std::swap(view.m[1], view.m[4]);
	std::swap(view.m[2], view.m[8]);
	std::swap(view.m[6], view.m[9]);
	// set the translation to be the same as campos
	view.m[12] = campos.x;
	view.m[13] = campos.y;
	view.m[14] = campos.z;

	Matrix model;
	model.MakeModelMatrix(offset, vec3(0,0,rotation), vec3(0.1, 0.1, 0.1));

	CustomGL::Get().SetModelMatrix(view.Multiply(model));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, -1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, guardTexture);

	shaders::Set(shaders::REFLECTION_MAP);
	glUniform3f(glGetUniformLocation(shaders::GetProgram(), "campos"), campos.x, campos.y, campos.z);
	barrels->Draw();
	shaders::Set(shaders::DIRECTIONAL_LIGHT_TEXTURE);
	glUniform3f(glGetUniformLocation(shaders::GetProgram(), "lightDir"), lightDir.x, lightDir.y, lightDir.z);
	glUniform1i(glGetUniformLocation(shaders::GetProgram(), "tex"), 0);
	guards->Draw();
	shaders::Unset();

	// draw the bullets
	Matrix scale;
	scale.MakeScalingMatrix(vec3(0.1,0.1,0.1));

	glDisable(GL_CULL_FACE);
	for(int i = 0; i < bullets.size(); ++i)
	{
		vec3 pos = bullets[i].position;
		//vec3 vel = bullets[i].velocity;
		Matrix trans;
		trans.MakeTranslationMatrix(pos);

		CustomGL::Get().SetModelMatrix(trans.Multiply(scale.Multiply(bullets[i].rotationMatrix)));
		shaders::Set(shaders::COLOR);
		glUniform4f(glGetUniformLocation(shaders::GetProgram(), "color"), 1.0, 0.8, 0.0, 0.0);
		laser->Draw();
	}
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	Matrix gunfireMat;
	gunfireMat.MakeModelMatrix(offset, vec3(0,0,0), vec3(0.1, 0.1, 0.1));
	CustomGL::Get().SetModelMatrix(view.Multiply(gunfireMat));
	shaders::Set(shaders::ANIMATED_TEXTURE);
	glUniform1i(glGetUniformLocation(shaders::GetProgram(), "tex"), 0);

	glUniform1f(glGetUniformLocation(shaders::GetProgram(), "sOffset"), offsets[(int)gunFireAnimationFrame][0]);
	glUniform1f(glGetUniformLocation(shaders::GetProgram(), "tOffset"), offsets[(int)gunFireAnimationFrame][1]);
	glBindTexture(GL_TEXTURE_2D, gunFireTexture);
	gunfire->Draw();
	CustomGL::Get().SetModelMatrixToIdentity();
	shaders::Unset();
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, -1);

	shaders::Unset();
	CustomGL::Get().SetModelMatrixToIdentity();
}

void Gun::FireBullet()
{
	bulletFired = true;
	gunFireAnimationFrame = 0.0f;
	fireSound->Play();

	Matrix v = CustomGL::Get().GetViewMatrix();
	vec3 xAxis(v.m[0], v.m[4], v.m[8]);
	vec3 yAxis(v.m[1], v.m[5], v.m[9]);
	vec3 zAxis(v.m[2], v.m[6], v.m[10]);

	Bullet& b = bullets[currentBullet];
	b.hasHit = false;
	b.position = campos + (xAxis * offset.x) + (yAxis * (offset.y + 0.016)) + (zAxis * (-offset.z - 0.15));
	b.velocity = (camdir * 25.0) + vec3(0,0,-5); // plus the velocity of the plane

	// bullets rotation matrix is the transpose of the view matrix top 3x3
	Matrix& rm = b.rotationMatrix;
	rm = v;
	rm.m[12] = rm.m[13] = rm.m[14] = 0.0f; // clear translation part
	std::swap(rm.m[1], rm.m[4]);
	std::swap(rm.m[2], rm.m[8]);
	std::swap(rm.m[6], rm.m[9]);
	
	currentBullet++;
	if(currentBullet >= bullets.size()) {
		currentBullet = 0;
	}
}

void Gun::StartFire()
{
	firing = true;
}

void Gun::EndFire()
{
	firing = false;
}

void Gun::Update(float timeStep)
{
	if(bulletFired) {
		// update gun fire animation

		// there are 20 frames in the animation, 
		float fps = bulletsPerSecond * 20.0f;
		gunFireAnimationFrame += fps * timeStep;

		if(gunFireAnimationFrame >= 20.0f) {
			bulletFired = false;
			gunFireAnimationFrame = 19.0f;
		}
	}

	if(firing) {
		rotationalVelocity = (bulletsPerSecond / 6.0f) * 360.0f;

		timeAccumulator += timeStep;
		while(timeAccumulator >= bulletTime)
		{
			FireBullet();
			timeAccumulator -= bulletTime;
		}

	} else {
		rotationalVelocity = 0.0f;
	}

	rotation += rotationalVelocity * timeStep;

	// update the bullets
	for(int i = 0; i < bullets.size(); ++i)
	{
		bullets[i].position = bullets[i].position + (bullets[i].velocity * timeStep);
	}
}


void Gun::CheckBoat(Boat* b)
{
	for(int i = 0; i < bullets.size(); ++i)
	{
		if(!bullets[i].hasHit && b->PointInside(bullets[i].position))
		{
			b->TakeHit(10);
			bullets[i].hasHit = true;
		}
	}
}
