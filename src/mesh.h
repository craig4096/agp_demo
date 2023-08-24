

#ifndef __MESH__
#define __MESH__

#include "customGL.h"
#include <string>
#include "shaders.h"

class Mesh {
	GLuint vertices;
	GLuint indices;
	GLuint texcoords;
	GLuint normals;
	GLuint indexCount;
public:

	Mesh(const std::string& filename);
	~Mesh();
	void Draw();
};


#endif
