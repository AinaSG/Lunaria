#ifndef DRILLITEM_H
#define DRILLITEM_H
#include "Item.h"
#include "Input.h"
#include "Game.h"


class DrillItem : public Item
{
public:
  DrillItem() {}
  void init(ShaderProgram &sp, int param = 0) {
    Item::init("taladro.png", sp);
    type = "Drill";
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
        scene->mineBlock(deltaTime,300);
        return;
      }
      else if (Input::instance().getMouseButtonDown(GLUT_RIGHT_BUTTON)) {
      }
      return;
    }

};

#endif // DRILLITEM_H
