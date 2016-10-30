#ifndef DRILLITEM_H
#define DRILLITEM_H
#include "Item.h"
#include "Input.h"
#include "Game.h"
#include "GameScene.h"


class DrillItem : public Item
{
public:
  DrillItem() {}
  void init(ShaderProgram &sp, int param = 0) {
    Item::init("taladro.png", sp);
    type = "Drill";
    amount = 1;
  }

  void use(float deltaTime) {
    GameScene* scene = Game::gameScene();
    if (Input::instance().getMouseButtonDown(GLUT_LEFT_BUTTON))
    {
      scene->player->attack();
    }

    if (Input::instance().getMouseButtonHold(GLUT_LEFT_BUTTON)) {
      scene->mineBlock(deltaTime,300);
      return;
    }
    else if (Input::instance().getMouseButtonDown(GLUT_RIGHT_BUTTON)) {
    }
    return;
  }

};

#endif // DRILLITEM_H
