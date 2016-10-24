#include "DiamondItem.h"
#include "Input.h"
#include "Game.h"

DiamondItem::DiamondItem()
{

}

void DiamondItem::init(ShaderProgram &sp)
{
  Item::init("moondiamonds.png", sp);
  amount = 1;
}

void DiamondItem::use(float deltaTime)
{
  Scene* scene = &Game::instance().scene;
  if (Input::instance().getMouseButtonHold(GLUT_LEFT_BUTTON)) {
    scene->mineBlock(deltaTime);
    return;
  }
  else if (Input::instance().getMouseButtonDown(GLUT_RIGHT_BUTTON)) {
    glm::ivec2 mousePos = Input::instance().getMouseScreenPos();
    glm::ivec2 tilePos =  scene->screenToTile(mousePos);
    if (scene->map->getTile(tilePos) == Block::Empty) {
      scene->map->setTile(tilePos, Block::BlueMaterial);
    }
  }

  return;
}
