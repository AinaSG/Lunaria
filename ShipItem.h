#ifndef SHIPITEM_H
#define SHIPITEM_H
#include "Item.h"
#include "Input.h"
#include "Game.h"


class ShipItem : public Item
{
public:
  ShipItem() {}
  void init(ShaderProgram &sp, int param = 0) {
    Item::init("ship.png", sp);
    type = "Ship";
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
        --amount;
        cout << "UWIN" << endl;
      }
      return;
    }

};

#endif // DRILLITEM_H
