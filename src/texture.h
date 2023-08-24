

#ifndef __TEXTURE__
#define __TEXTURE__

#include "customGL.h"
#include <string>

GLuint LoadStdTexture(const std::string& filename);
GLuint LoadSkyboxTexture(const std::string& filename);


#endif
