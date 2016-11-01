#include "InfoScene.h"
#include "Game.h"
#include "ResourceManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"

InfoScene::InfoScene(const char* filename) : imageName(filename)
{

}

InfoScene::~InfoScene()
{
  if (bg != nullptr) delete bg;
}

void InfoScene::init()
{
  Scene::init();

  Texture* tex = ResourceManager::instance().getTexture(imageName);
  bg = Sprite::createSprite(Game::screenSize,glm::vec2(1.0,1.0),tex,&texProgram);
}


void InfoScene::update(int deltaTime)
{
  if (Input::instance().getKeyDown(13)) {
    Game::instance().gotoMenu();
  }
}

void InfoScene::render()
{
  glm::mat4 model(1.0f);
  glm::mat4 view(1.0f);

  texProgram.use();
  texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
  texProgram.setUniformMatrix4f("projection", projection);
  texProgram.setUniformMatrix4f("model", model);
  texProgram.setUniformMatrix4f("view", view);

  bg->render();
}
