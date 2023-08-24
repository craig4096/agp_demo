

#ifndef __GAME__
#define __GAME__

#include "customGL.h"
#include "mesh.h"
#include "shaders.h"
#include "glbmp.h"
#include "skybox.h"
#include "water.h"
#include "texture.h"
#include "camera.h"
#include "plane.h"
#include "gun.h"
#include <vector>
#include "sound.h"
#include "boat.h"

class Game {

	Camera* camera;
	Gun* gun;
	Gun* gun2;
	std::vector<Plane*> planes;
	Mesh* boatMesh;
	GLuint boatTexture;
	std::vector<Boat*> boats;
	GLuint cubemap;
	Water* water;
	Skybox* skybox;
	vec3 lightDir;
	Mesh* planeBodyMesh;
	Mesh* planeCockpitMesh;
	Mesh* planePropellerMesh;
	GLuint planeBodyTexture;
	Sound* planeSound;
	int waveNumber;

	static void LoadCubeMapSide(GLenum target, const char* filename);

public:

	Game(const char* textures[6]);
	~Game();

	void Update(float timeStep);
	void Draw();
	void Mouse(int button, int state, int x, int y);
	void MouseMoved(int x, int y);
	void KeyPressed(unsigned char c);
	void KeyReleased(unsigned char c);
	

};


#endif
