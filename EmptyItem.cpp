#include "EmptyItem.h"
#include "Input.h"
#include "Game.h"

void EmptyItem::use(float deltaTime)
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
