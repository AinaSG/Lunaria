#include "MenuScene.h"
#include "Game.h"
#include "ResourceManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"

MenuScene::MenuScene()
{

}

MenuScene::~MenuScene()
{
  if (bg != nullptr) delete bg;
  if (arrow != nullptr) delete arrow;
}

void MenuScene::init()
{
  Scene::init();

  Texture* tex = ResourceManager::instance().getTexture("menu.png");
  bg = Sprite::createSprite(Game::screenSize,glm::vec2(1.0,1.0),tex,&texProgram);

  tex = ResourceManager::instance().getTexture("arrow.png");
  arrow = Sprite::createSprite(glm::ivec2(150,100),glm::vec2(1.0,1.0),tex,&texProgram);
}


void MenuScene::update(int deltaTime)
{
  if (Input::instance().getKeyDown(13)) {
    switch (menuEntry) {
      case 0:
        Game::instance().startGame();
        break;
      case 1:
        Game::instance().gotoHelp();
        break;
      case 2:
        Game::instance().gotoCredits();
        break;
    }
  }

  if (Input::instance().getSpecialKeyDown(GLUT_KEY_DOWN)) {
    menuEntry = (menuEntry+1)%3;
  }

  if (Input::instance().getSpecialKeyDown(GLUT_KEY_UP)) {
    menuEntry = (menuEntry-1 + 3)%3;
  }

  arrow->setPosition(glm::vec2(300,300+menuEntry*100));
}

void MenuScene::render()
{
  glm::mat4 model(1.0f);
  glm::mat4 view(1.0f);

  texProgram.use();
  texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
  texProgram.setUniformMatrix4f("projection", projection);
  texProgram.setUniformMatrix4f("model", model);
  texProgram.setUniformMatrix4f("view", view);

  bg->render();
  arrow->render();
}
