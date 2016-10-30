#include "MenuScene.h"
#include "Game.h"
#include "ResourceManager.h"
#include <glm/gtc/matrix_transform.hpp>

MenuScene::MenuScene()
{

}

MenuScene::~MenuScene()
{

}

void MenuScene::init()
{
  Scene::init();

  cameraPos = glm::vec2(0,0);
}

void MenuScene::update(int deltaTime)
{
 Game::instance().startGame();
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

}
