#include "Item.h"
#include "ResourceManager.h"

Item::Item()
{
  sprite = nullptr;
}

Item::~Item()
{
  if (sprite != nullptr) delete sprite;
}

void Item::init(string textureName, ShaderProgram &shaderProgram)
{
  amount = 0;
  Texture * tex = ResourceManager::instance().getTexture(textureName);
  sprite = Sprite::createSprite(glm::ivec2(32,32),glm::vec2(1.0f,1.0f),tex, &shaderProgram);
}

void Item::render()
{

  if (sprite != NULL) sprite->render();
}


