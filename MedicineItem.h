#ifndef MEDICINEITEM_H
#define MEDICINEITEM_H
#include "Item.h"
#include "Input.h"
#include "Game.h"


class MedicineItem : public Item
{
public:
  MedicineItem() {}
  void init(ShaderProgram &sp, int param = 0) {
    Item::init("medi.png", sp);
    type = "Medicine";
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
        Game::instance().scene.player->heal(2);
        --amount;
      }
      return;
    }
};

#endif // MEDICINEITEM_H
