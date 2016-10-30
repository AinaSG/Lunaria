#ifndef GLUEITEM_H
#define GLUEITEM_H
#include "Item.h"
#include "Input.h"
#include "Game.h"

class GlueItem : public Item
{
public:
  GlueItem() {}
  void init(ShaderProgram &sp, int param = 0) {
    Item::init("cola.png", sp);
    type = "Glue";
    amount = 1;
  }

  void use(float deltaTime){

    GameScene* scene = Game::gameScene();
    if (Input::instance().getMouseButtonDown(GLUT_LEFT_BUTTON))
    {
      scene->player->attack();
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

#endif // GLUEITEM_H
