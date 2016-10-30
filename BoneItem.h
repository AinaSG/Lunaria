#ifndef BONEITEM_H
#define BONEITEM_H
#include "Item.h"
#include "Input.h"
#include "Game.h"


class BoneItem : public Item
{
public:
  BoneItem() {}
  void init(ShaderProgram &sp, int param = 0) {
    Item::init("alienbone.png", sp);
    type = "Bone";
    amount = 1;
  }
  void use(float deltaTime)
  {
    if (Input::instance().getMouseButtonDown(GLUT_LEFT_BUTTON))
    {
      Game::instance().scene.player->attack();
    }
    Scene* scene = &Game::instance().scene;
    if (Input::instance().getMouseButtonHold(GLUT_LEFT_BUTTON)) {
      scene->mineBlock(deltaTime);
      return;
    }
    else if (Input::instance().getMouseButtonDown(GLUT_RIGHT_BUTTON)) {
    }
    return;
  }
};

#endif // BONEITEM_H
