#ifndef ITEM_H
#define ITEM_H

#include "Sprite.h"

class Item
{
public:
  Sprite* sprite;

public:
  void init(string textureName, ShaderProgram &sp);
  int amount;
  string type;

public:
  Item();
  ~Item();

  virtual void use(float deltaTime);
  virtual void init(ShaderProgram &sp, int param = 0) = 0;

  void spend(int num);
  void render();
  void setPosition(glm::ivec2 pos) { if (sprite != nullptr) sprite->setPosition(pos); }
  string getType();

};

#endif // ITEM_H
