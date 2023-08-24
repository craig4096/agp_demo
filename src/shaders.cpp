
#include "shaders.h"


#define CHECK_COMPILATION

#ifdef CHECK_COMPILATION
#include <iostream>
#include <cstdlib>
using namespace std;
#endif

namespace shaders
{

struct ShaderCode {
	const char* vertex;
	const char* fragment;
}
shader_code[SHADER_COUNT] = {

{	// REFLECTION SHADER
"uniform mat4 viewMatrix;"
"uniform mat4 projectionMatrix;"
"uniform mat4 modelMatrix;"
"uniform vec3 campos;"
"attribute vec3 pos;"
"attribute vec3 normal;"
"varying vec3 n;"
"varying vec3 viewVec;"
"void main() {"
"	vec4 worldPos = modelMatrix * vec4(pos, 1.0);"
"	vec4 viewPos = viewMatrix * worldPos;"
"	gl_Position = projectionMatrix * viewPos;"
"	viewVec = vec3(worldPos) - campos;"
"	n = mat3(modelMatrix) * normal;" // rotate normal into world space
"}"
,
"uniform samplerCube cubemap;"
"uniform vec3 color;"
"varying vec3 viewVec;"
"varying vec3 n;"
"void main() {"
"	gl_FragColor = textureCube(cubemap, reflect(viewVec, normalize(n))) * 0.8;"
"}"
},
{	// TEXTURE SHADER
"uniform mat4 modelViewProjectionMatrix;"
"attribute vec3 pos;"
"attribute vec2 texcoord;"
"varying vec2 uv;"
"void main() {"
"	gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);"
"	uv = texcoord;"
"}"
,
"uniform sampler2D tex;"
"varying vec2 uv;"
"void main() {"
"	gl_FragColor = texture2D(tex, uv);"
"}"
},
{	// WATER SHADER
"uniform mat4 modelViewProjectionMatrix;"
"uniform vec3 campos;"
"uniform float sOffset;"
"uniform float tOffset;"
"attribute vec3 pos;"
"attribute vec2 texcoord;"
"varying vec2 uv1;"
"varying vec2 uv2;"
"varying vec3 viewVec;"
"void main() {"
"	uv1 = texcoord + vec2(sOffset, 0.0);"
"	uv2 = texcoord + vec2(0.0, tOffset);"
"	viewVec = campos - pos;"
"	gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);"
"}"
,
"uniform sampler2D tex;"
"uniform samplerCube cubemap;"
"varying vec3 viewVec;"
"varying vec2 uv1;"
"varying vec2 uv2;"
"void main() {"
"	vec4 color1 = texture2D(tex, uv1);"
"	vec4 color2 = texture2D(tex, uv2);"
"	vec3 normal1 = vec3((color1.r-0.5)*2.0, (color1.b-0.5)*2.0, (color1.g-0.5)*-2.0);"
"	vec3 normal2 = vec3((color2.r-0.5)*2.0, (color2.b-0.5)*2.0, (color2.g-0.5)*-2.0);"
"	vec3 normal = normalize(normal1 + normal2);"
"	vec3 vv = normalize(viewVec);"
"	vec4 reflectcolor = textureCube(cubemap, reflect(-vv, normal));"
"	vec4 refractcolor = vec4(0.2, 0.1, 0.0, 0.0);"
"	float fresnelApprox = pow(dot(vv, vec3(0,1,0)), 2);"
"	gl_FragColor = ((refractcolor * fresnelApprox) + (reflectcolor * (1.0-fresnelApprox)) ) * 0.7;"
"}"
},
{// DIRECTIONAL LIGHT TEXTURE
"uniform mat4 modelViewProjectionMatrix;"
"uniform mat4 modelMatrix;"
"attribute vec3 pos;"
"attribute vec3 normal;"
"attribute vec2 texcoord;"
"varying vec2 uv;"
"varying vec3 n;"
"void main() {"
"	gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);"
"	n = mat3(modelMatrix) * normal;"
"	uv = texcoord;"
"}"
,
"uniform vec3 lightDir;"
"uniform sampler2D tex;"
"varying vec2 uv;"
"varying vec3 n;"
"void main() {"
"	vec4 color = texture2D(tex, uv);"
"	float lambert = max(0.0, dot(normalize(n), normalize(vec3(1,1,-1)) ));"
"	gl_FragColor = (color * lambert) + (color*0.3);"
"}"
},
{ // ANIMATED TEXTURE SHADER
"uniform mat4 modelViewProjectionMatrix;"
"uniform float sOffset;"
"uniform float tOffset;"
"attribute vec3 pos;"
"attribute vec2 texcoord;"
"varying vec2 uv;"
"void main() {"
"	gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);"
"	uv = texcoord + vec2(sOffset, tOffset);"
"}"
,
"uniform sampler2D tex;"
"varying vec2 uv;"
"void main() {"
"	gl_FragColor = texture2D(tex, uv);"
"}"
},
{ // COLOR SHADER
"uniform mat4 modelViewProjectionMatrix;"
"attribute vec3 pos;"
"void main() {"
"	gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);"
"}"
,
"uniform vec4 color;"
"void main() {"
"	gl_FragColor = color;"
"}"
}
};


struct Shader {
	GLuint program;
	GLuint vertex;
	GLuint fragment;
} shaders[SHADER_COUNT];


void SetUniforms()
{
	// sets the custom pipeline matrices
	CustomGL& gl = CustomGL::Get();
	glUniformMatrix4fv(glGetUniformLocation(GetProgram(), "modelMatrix"), 1, GL_FALSE, gl.GetModelMatrix().m);
	glUniformMatrix4fv(glGetUniformLocation(GetProgram(), "viewMatrix"), 1, GL_FALSE, gl.GetViewMatrix().m);
	glUniformMatrix4fv(glGetUniformLocation(GetProgram(), "projectionMatrix"), 1, GL_FALSE, gl.GetProjectionMatrix().m);
	glUniformMatrix4fv(glGetUniformLocation(GetProgram(), "modelViewProjectionMatrix"), 1, GL_FALSE, gl.GetModelViewProjectionMatrix().m);
}


void Init()
{
	// compile all the shaders
	for(int i = 0; i < SHADER_COUNT; ++i)
	{
		ShaderCode& c = shader_code[i];
		Shader& s = shaders[i];
		

		s.vertex = glCreateShader(GL_VERTEX_SHADER);
		s.fragment = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(s.vertex, 1, &c.vertex, NULL);
		glShaderSource(s.fragment, 1, &c.fragment, NULL);



		glCompileShader(s.vertex);
		glCompileShader(s.fragment);

		s.program = glCreateProgram();

		glAttachShader(s.program, s.vertex);
		glAttachShader(s.program, s.fragment);

		glLinkProgram(s.program);


	#ifdef CHECK_COMPILATION
		// for catching comilation errors
		char errormsg[2048];
		GLsizei length;
		GLint compileError;

		// make sure it compiled
		glGetShaderiv(s.vertex, GL_COMPILE_STATUS, &compileError);
		if(compileError == GL_FALSE)
		{
			// get the error
			glGetShaderInfoLog(s.vertex, 2048, &length, errormsg);

			cout << errormsg << endl;
			exit(0);
		}

		glGetShaderiv(s.fragment, GL_COMPILE_STATUS, &compileError);
		if(compileError == GL_FALSE)
		{
			// get the error
			glGetShaderInfoLog(s.fragment, 2048, &length, errormsg);

			cout << errormsg << endl;
			exit(0);
		}

		// check linking errors
		GLint status;
		glGetProgramiv(s.program, GL_LINK_STATUS, &status);
		if(status == GL_FALSE)
		{
			cout << "could not link shaders" << endl;
			exit(0);
		}
	#endif
	}


	// get the location variables

	//REFLECTION_MAP_CAMPOS_LOCATION = glGetUniformLocation(shaders[REFLECTION_MAP].program, "campos");
	//REFLECTION_MAP_CAMPOS_LOCATION = glGetUniformLocation(shaders[REFLECTION_MAP].program, "campos");

	//WATER_CUBE_MAP_LOCATION = glGetUniformLocation(shaders[WATER].program, "cubemap");
	//WATER_TEX1_LOCATION = glGetUniformLocation(shaders[WATER].program, "tex1");
	//WATER_POS_LOCATION = glGetAttribLocation(shaders[WATER].program, "pos");
	//WATER_TEXCOORD_LOCATION = glGetAttribLocation(shaders[WATER].program, "texcoord");
	//WATER_CAMPOS_LOCATION = glGetUniformLocation(shaders[WATER].program, "campos");
}

GLuint currentProgram = 0;

void Set(ShaderType t)
{
	currentProgram = shaders[t].program;
	glUseProgram(shaders[t].program);
	// need to set the uniform matrices each time a new shader
	// is bound
	SetUniforms();
}


GLuint GetProgram()
{
	return currentProgram;
}


void Unset()
{
	currentProgram = 0;
	glUseProgram(0);
}

void Free()
{
	for(int i = 0; i < SHADER_COUNT; ++i)
	{
		Shader& s = shaders[i];

		glDetachShader(s.program, s.vertex);
		glDetachShader(s.program, s.fragment);

		glDeleteShader(s.vertex);
		glDeleteShader(s.fragment);

		glDeleteProgram(s.program);
	}
}


}
