#include "BlockItem.h"
#include "Input.h"
#include "Game.h"

BlockItem::BlockItem()
{
}

void BlockItem::init(ShaderProgram &sp, int param)
{
  Item::init(filenames[param-1], sp);
  blockID = param;
  amount = 1;
}

void BlockItem::use(float deltaTime)
{
  Scene* scene = &Game::instance().scene;
  if (Input::instance().getMouseButtonHold(GLUT_LEFT_BUTTON)) {
    scene->mineBlock(deltaTime);
    return;
  }
  else if (Input::instance().getMouseButtonDown(GLUT_RIGHT_BUTTON)) {
    glm::ivec2 mousePos = Input::instance().getMouseScreenPos();
    glm::ivec2 tilePos =  scene->screenToTile(mousePos);
    glm::vec2 worldPos = scene->screenToWorld(mousePos);
    glm::vec2 playerPos = scene->player->getPos();

    if (glm::distance(worldPos,playerPos) > 60) return;

    if (scene->map->getTile(tilePos) == Block::Empty) {
      scene->map->setTile(tilePos, blockID);
      --amount;
    }
  }

  return;
}
