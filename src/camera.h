
#ifndef __CAMERA__
#define __CAMERA__

#include "vec3.h"

class Camera {
	int	cursorResetX;
	int	cursorResetY;
	float	weightX;
	float	weightY;
public:
	float	rotationY;
	float	rotationX;
	vec3	viewDirection;
	vec3	position;
	bool	restrained;
private:
	bool forward, backward, left, right;

	static vec3 CalculateViewDirection(float xRot, float yRot);
public:
	Camera(int cursorResetX, int cursorResetY);
	void MouseMoved(int x, int y);
	void KeyPressed(unsigned char c);
	void KeyReleased(unsigned char c);
	void Update(float timeDelta);
	const vec3& GetPosition() const { return position; }
	const vec3& GetViewDirection() const { return viewDirection; }

};

#endif
