
#include "customGL.h"
#include "camera.h"
#include "math3d.h"


Camera::Camera(int cursorResetX, int cursorResetY)
	: cursorResetX(cursorResetX)
	, cursorResetY(cursorResetY)
	, forward(false), backward(false), left(false), right(false)
	, weightX(0.03f)
	, weightY(0.03f)
	, rotationX(90.0f)
	, rotationY(180.0f)
	, position(0, 1.6, 0)
{
	restrained = false;
}

vec3 Camera::CalculateViewDirection(float xRot, float yRot)
{
	vec3 viewDir;
	// x and z can be calculated from the y axis
	// rotation using sin and cos
	viewDir.x = sin(yRot * DEG_TO_RAD);
	viewDir.z = cos(yRot * DEG_TO_RAD);

	// y can be calculated from the
	// x rotation using cos
	viewDir.y = cos(xRot * DEG_TO_RAD);

	if(viewDir.y > 0.0f)
	{
		viewDir.x *= (1.0f-viewDir.y);
		viewDir.z *= (1.0f-viewDir.y);
	}
	else
	{
		viewDir.x *= (1.0f+viewDir.y);
		viewDir.z *= (1.0f+viewDir.y);
	}
	return viewDir;
}

void Camera::MouseMoved(int x, int y)
{
	// calculate the delta between the point and the screen center
	int dx = cursorResetX - x;
	int dy = y - cursorResetY;

	rotationY += ((float)dx * weightX);
	rotationX += ((float)dy * weightY);

	// clamp x rotation
	if(rotationX < 1.0f)
	{
		rotationX = 1.0f;
	}
	else if(rotationX > 179.0f)
	{
		rotationX = 179.0f;
	}


	// calculate view Dir based on rotations
	viewDirection = CalculateViewDirection(rotationX, rotationY);
}

void Camera::KeyPressed(unsigned char c)
{
	switch(c)
	{
	case 'w': forward = true; break;
	case 's': backward =  true; break;
	case 'a': left = true; break;
	case 'd': right = true; break;
	}
}

void Camera::KeyReleased(unsigned char c)
{
	switch(c)
	{
	case 'w': forward = false; break;
	case 's': backward =  false; break;
	case 'a': left = false; break;
	case 'd': right = false; break;
	}
}

void Camera::Update(float timeDelta)
{
	// reset cursor position
	glutWarpPointer(cursorResetX, cursorResetY);

	vec3 vel(0,0,0);
	if(forward) vel = vel + viewDirection;
	if(backward) vel = vel - viewDirection;

	if(restrained)
		vel.y = 0.0f;

	vel.Normalize();

	static float speed = 1.0f;
	position = position + (vel * timeDelta * speed);

	if(restrained)
	{
		if(position.x < -1.0) position.x = -1.0;
		if(position.x > 1.0) position.x = 1.0;

		if(position.z < -1.0) position.z = -1.0;
		if(position.z > 1.0) position.z = 1.0;
	}
}
