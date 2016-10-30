#include "Item.h"
#include "ResourceManager.h"
#include "Game.h"
#include "Input.h"

Item::Item()
{
  sprite = nullptr;
}

Item::~Item()
{
  if (sprite != nullptr) delete sprite;
}

void Item::use(float deltaTime)
{
  if (Input::instance().getMouseButtonDown(GLUT_LEFT_BUTTON))
  {
    Game::instance().scene.player->attack();
  }
  if (Input::instance().getMouseButtonHold(GLUT_LEFT_BUTTON))
  {
    Game::instance().scene.mineBlock(deltaTime);
  }
}

void Item::init(string textureName, ShaderProgram &shaderProgram)
{
  amount = 0;
  Texture * tex = ResourceManager::instance().getTexture(textureName);
  tex->setMagFilter(GL_NEAREST);
  tex->setMinFilter(GL_NEAREST);
  sprite = Sprite::createSprite(glm::ivec2(32,32),glm::vec2(1.0f,1.0f),tex, &shaderProgram);
}

void Item::render()
{
  if (sprite != NULL) sprite->render();
}

string Item::getType(){
  return type;
}

void Item::spend(int num){
  amount = amount - num;
}
