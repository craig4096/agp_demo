
#include "game.h"

Game::Game(const char* textures[6])
	: lightDir(-1,-1,-1)
	, waveNumber(0)
{
	lightDir.Normalize();
	water = new Water("wave.bmp",200, 0.1, 1000);

	// load the boat mesh
	boatMesh = new Mesh("boat.txt");
	boatTexture = LoadStdTexture("boat.bmp");

	// create the cube map
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	// load the sides
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, textures[0]);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, textures[1]);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, textures[2]);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, textures[3]);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, textures[4]);
	LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, textures[5]);

	// set the magnification and minification filters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// unbind the cube map texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, -1);

	// load the skybox
	skybox = new Skybox(textures);


	// create the camera
	camera = new Camera(200, 200);
	gun = new Gun(10);

	planeBodyMesh = new Mesh("plane.txt");
	planePropellerMesh = new Mesh("propeller.txt");
	planeCockpitMesh = new Mesh("cockpit.txt");
	planeBodyTexture = LoadStdTexture("plane.bmp");

	for(int i = 0; i < 10; ++i)
		planes.push_back(new Plane(planeBodyMesh, planeCockpitMesh, planePropellerMesh, planeBodyTexture, vec3((i*8)-40, 3, 100)));

	planeSound = new Sound("plane.wav", true);
	planeSound->Play();

	boats.resize(3);
	for(int i = 0; i < boats.size(); ++i)
	{
		boats[i] = new Boat(boatMesh, boatTexture);
		boats[i]->SetPosition(vec3((i*30) - 30, 0, 0));
	}
}

Game::~Game()
{
	glDeleteTextures(1, &cubemap);
	delete boatMesh;
	delete water;
	delete skybox;
	delete camera;
	delete gun;
	delete planeBodyMesh;
	delete planePropellerMesh;
	delete planeCockpitMesh;
	delete planeSound;
	for(int i = 0; i < planes.size(); ++i)
		delete planes[i];
	for(int i = 0; i < boats.size(); ++i)
		delete boats[i];
	glDeleteTextures(1, &boatTexture);
	glDeleteTextures(1, &planeBodyTexture);
}


void Game::Update(float timeStep)
{
	camera->Update(timeStep);
	for(int i = 0; i < planes.size(); ++i)
	{
		planes[i]->Update(timeStep);
	}

	// update boats
	for(int i = 0; i < boats.size(); ++i)
	{
		boats[i]->Update(timeStep);
	}

	gun->campos = camera->GetPosition();
	gun->camdir = camera->GetViewDirection();

	camera->position = planes[7]->position + vec3(0,1,0);
	static const float dist = 75;
	if(camera->position.z < -dist)
	{
		for(int i = 0; i < planes.size(); ++i)
		{
			planes[i]->position.z = dist;
		}
		camera->position.z = dist;

		waveNumber++;

		// reset the boats
		for(int i = 0; i < boats.size(); ++i)
		{
			boats[i]->Reset(100 + ((waveNumber*waveNumber)*5));
		}
	}

	gun->Update(timeStep);

	// check if any of the bullets in Gun are intersecting with any of the boats
	for(int i = 0; i < boats.size(); ++i)
	{
		gun->CheckBoat(boats[i]);
	}

	water->Update(timeStep);
}


void Game::Draw()
{
	Matrix view;
	view.MakeViewMatrix(camera->GetPosition(), camera->GetPosition() + camera->GetViewDirection());
	CustomGL::Get().SetViewMatrix(view);

	// draw the skybox
	skybox->Draw(camera->GetPosition());


	// draw the boats
	for(size_t i = 0; i < boats.size(); ++i)
	{
		boats[i]->Draw(lightDir);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	for(int i = 0; i < planes.size(); ++i)
		planes[i]->Draw(camera->GetPosition(), lightDir);

	// draw the water
	water->Draw(camera->GetPosition());

	// draw gun
	gun->Draw(camera->GetPosition(), lightDir);

	glBindTexture(GL_TEXTURE_CUBE_MAP, -1);
}


void Game::LoadCubeMapSide(GLenum target, const char* filename)
{
	glbmp_t bmp;
	if(glbmp_LoadBitmap(filename, GLBMP_TOP_DOWN, &bmp))
	{
		glTexImage2D(target, 0, GL_RGB, bmp.width, bmp.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.rgb_data);

		glbmp_FreeBitmap(&bmp);
	}
}

void Game::Mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT) {
		if(state == GLUT_DOWN) {
			gun->StartFire();
		} else {
			gun->EndFire();
		}
	}
}

void Game::MouseMoved(int x, int y)
{
	camera->MouseMoved(x, y);
}

void Game::KeyPressed(unsigned char c)
{
	camera->KeyPressed(c);
}

void Game::KeyReleased(unsigned char c)
{
	camera->KeyReleased(c);
}

