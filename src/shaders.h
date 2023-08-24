

#ifndef __SHADER__
#define __SHADER__

#include "customGL.h"

namespace shaders
{

enum ShaderType {
	REFLECTION_MAP,
	TEXTURED,
	WATER,
	DIRECTIONAL_LIGHT_TEXTURE,
	ANIMATED_TEXTURE,
	COLOR,
	SHADER_COUNT
};

void Init();
void Set(ShaderType);
GLuint GetProgram();	// returns the program for the currently set shader
void Unset();
void Free();
void SetUniforms();

}

#endif
