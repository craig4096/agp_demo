

#ifndef __SKYBOX__
#define __SKYBOX__

#include "customGL.h"
#include "vec3.h"


struct Skybox {
	GLuint vertices;
	GLuint texcoords;
	GLuint textures[6];
public:
	Skybox(const char* textures[6]);
	~Skybox();
	void Draw(const vec3& campos, float scale = 1000.0f);
};


#endif
