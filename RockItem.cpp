#include "RockItem.h"
#include "Input.h"
#include "Game.h"

RockItem::RockItem()
{
}

void RockItem::init(ShaderProgram &sp)
{
  Item::init("moonground.png", sp);
  amount = 1;
}

void RockItem::use(float deltaTime)
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
      scene->map->setTile(tilePos, Block::Rock);
    }
  }

  return;
}
