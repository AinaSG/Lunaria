#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
}

bool Game::update(int deltaTime)
{
	if (mouseDown) {
		mouseHold();
	}

	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	mousePos = glm::ivec2(x,y);
}

void Game::mousePress(int button)
{
	glm::ivec2 tileCoords = getMouseWorldPos()/16;
	if (scene.map->getTile(tileCoords) != 0) {
		scene.map->removeTile(tileCoords);
	}
}

void Game::mouseHold()
{
	glm::ivec2 tileCoords = getMouseWorldPos()/16;
	if (scene.map->getTile(tileCoords) != 0) {
		scene.map->removeTile(tileCoords);
	}
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

glm::ivec2 Game::getMouseWorldPos()
{
	return scene.getCameraPos() + mousePos - glm::ivec2(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
}

glm::ivec2 Game::getMouseScreenPos()
{
	return mousePos; 
}





