#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "InfoScene.h"
#include "Input.h"


const glm::ivec2 Game::screenSize = glm::ivec2(SCREEN_WIDTH,SCREEN_HEIGHT);
const glm::ivec2 Game::halfScreenSize = Game::screenSize/2;

void Game::init()
{
	bPlay = true;
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    nextScene = nullptr;
    scene = new MenuScene();
    scene->init();

    if(!text.init("fonts/OpenSans-Regular.ttf"))
        cout << "Could not load font!!!" << endl;

     if(!boldText.init("fonts/OpenSans-ExtraBold.ttf"))
        cout << "Could not load bold font!!!" << endl;
}

bool Game::update(int deltaTime)
{
    scene->update(deltaTime);
    Input::instance().update();
    if (nextScene != nullptr) {
      delete scene;
      scene = nextScene;
      scene->init();
      nextScene = nullptr;
    }
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->render();
}

void Game::startGame()
{
  nextScene = new GameScene();
}

void Game::gotoMenu()
{
  nextScene = new MenuScene();
}

void Game::gotoHelp()
{
  nextScene = new InfoScene("help.png");
}

void Game::gotoCredits()
{
  nextScene = new InfoScene("credits.png");
}





