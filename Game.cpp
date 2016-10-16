#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Input.h"


const glm::ivec2 Game::screenSize = glm::ivec2(SCREEN_WIDTH,SCREEN_HEIGHT);
const glm::ivec2 Game::halfScreenSize = Game::screenSize/2;

void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
    Input::instance().update();
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}





