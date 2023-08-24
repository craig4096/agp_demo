
#include "texture.h"
#include "glbmp.h"
using namespace std;

GLuint LoadStdTexture(const string& filename)
{
	glbmp_t bmp;
	if(glbmp_LoadBitmap(filename.c_str(), 0, &bmp))
	{
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.width, bmp.height, GL_RGB, GL_UNSIGNED_BYTE, bmp.rgb_data);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.width, bmp.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.rgb_data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glbmp_FreeBitmap(&bmp);

		return texture;
	}

	return -1;
}


GLuint LoadSkyboxTexture(const string& filename)
{
	glbmp_t bmp;
	if(glbmp_LoadBitmap(filename.c_str(), 0, &bmp))
	{
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.width, bmp.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.rgb_data);

		// set the texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glbmp_FreeBitmap(&bmp);

		return texture;
	}

	return -1;
}
