#ifndef SWORDITEM_H
#define SWORDITEM_H
#include "Item.h"
#include "Input.h"
#include "Game.h"


class SwordItem : public Item
{
public:
  SwordItem() {}
  void init(ShaderProgram &sp, int param = 0) {
    Item::init("sword.png", sp);
    type = "Sword";
    amount = 1;
  }
  void use(float deltaTime)
    {
      GameScene* scene = Game::gameScene();
      if (Input::instance().getMouseButtonDown(GLUT_LEFT_BUTTON))
      {
        scene->player->attack(2);
      }

      if (Input::instance().getMouseButtonHold(GLUT_LEFT_BUTTON)) {
        scene->mineBlock(deltaTime);
        return;
      }
      else if (Input::instance().getMouseButtonDown(GLUT_RIGHT_BUTTON)) {
      }
      return;
    }

};

#endif // SWORDITEM_H
