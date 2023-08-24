


#define _USE_MATH_DEFINES
#include <math.h>

const float DEG_TO_RAD = M_PI/180.0f;

#include <cstdlib>
#include <iostream>
#include "game.h"
#include <SFML/System.hpp>

using namespace std;

void DrawScene();
void Resize(int w, int h);
void MouseMoved(int x, int y);
void KeyPressed(unsigned char c, int,int);
void KeyReleased(unsigned char c, int,int);
void Mouse(int,int,int,int);

Game* game;
sf::Clock timer;

float accumulator = 0;
float timeStep = 0.02; // 50fps
bool bExitApp = false;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("AGP Demo");

	glutDisplayFunc(DrawScene);
	glutReshapeFunc(Resize);
	glutPassiveMotionFunc(MouseMoved);
	glutMotionFunc(MouseMoved);
	glutKeyboardFunc(KeyPressed);
	glutKeyboardUpFunc(KeyReleased);
	glutMouseFunc(Mouse);

	///////////////////////////////////////////////////////////////////

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	if(glewInit() != GLEW_OK)
	{
		cout << "could not initiate GLEW" << endl;
		return 0;
	}

	cout << "initiating shaders..." << endl;
	// initiate shaders
	shaders::Init();
	cout << "shaders initiated" << endl;


	// sky box textures
	const char* textures[6] = {
		"cubemap/posx.bmp",
		"cubemap/negx.bmp",
		"cubemap/posy.bmp",
		"cubemap/negy.bmp",
		"cubemap/posz.bmp",
		"cubemap/negz.bmp"
	};
	game = new Game(textures);

	//glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	timer.restart();
	while(!bExitApp)
	{
		glutMainLoopEvent();
	}

	delete game;
	shaders::Free();
	return 0;
}

void MouseMoved(int x, int y)
{
	game->MouseMoved(x, y);
}

void Mouse(int button, int state, int x, int y)
{
	game->Mouse(button, state, x, y);
}

void KeyPressed(unsigned char c, int,int)
{
	if(c == 27)
	{
		bExitApp = true;
	}
	else
	{
		game->KeyPressed(c);
	}
}

void KeyReleased(unsigned char c, int,int)
{
	game->KeyReleased(c);
}


void Resize(int w, int h)
{
	glViewport(0, 0, w, h);
	Matrix proj;
	proj.MakeProjectionMatrix(90.0f, w/(float)h, 0.01, 10000.0f);
	CustomGL::Get().SetProjectionMatrix(proj);
}


void DrawScene()
{
	bool render = false;
	float timeDelta = timer.getElapsedTime().asSeconds();
	timer.restart();
	accumulator += timeDelta;
	while(accumulator >= timeStep) {
		game->Update(timeStep);
		accumulator -= timeStep;
		render = true;
	}

	if(render) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		game->Draw();
		glutSwapBuffers();
	}
	glutPostRedisplay();
}



