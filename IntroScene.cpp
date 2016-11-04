#include "IntroScene.h"
#include "Game.h"
#include "ResourceManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"
#include <sstream>

IntroScene::IntroScene()
{

}

IntroScene::~IntroScene()
{
  for (int i = 0; i < 5; ++i) if (bg[i] != nullptr) delete bg[i];
}

void IntroScene::init()
{
  Scene::init();

  currentImage = 0;
  elapsedTime = 0.f;

  for (int i = 0; i < 5; ++i) {
      stringstream nameStr;
      nameStr << "intro" << i+1 << ".png";
      Texture* tex = ResourceManager::instance().getTexture(nameStr.str());
      bg[i] = Sprite::createSprite(Game::screenSize,glm::vec2(1.0,1.0),tex,&texProgram);
  }

}


void IntroScene::update(int deltaTime)
{
  elapsedTime += deltaTime/1000.f;
  currentImage = floor(elapsedTime/1.f);
  if (currentImage > 4) Game::instance().startGame();
}

void IntroScene::render()
{
  glm::mat4 model(1.0f);
  glm::mat4 view(1.0f);

  texProgram.use();
  texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
  texProgram.setUniformMatrix4f("projection", projection);
  texProgram.setUniformMatrix4f("model", model);
  texProgram.setUniformMatrix4f("view", view);

  bg[currentImage]->render();
}
