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
  if(blockID == 0){
    type = "Empty";
  }
  else if (blockID == 1){
    type = "Stone";
  }
  else if (blockID == 2) {
    type = "Diamond";
  }
  else if (blockID == 3) {
    type = "Ruby";
  }
  else{
    type = "BedRock";
  }
}

void BlockItem::use(float deltaTime)
{
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
    if (scene->player->isJumping()) return;

    glm::ivec2 mousePos = Input::instance().getMouseScreenPos();
    glm::ivec2 tilePos =  scene->screenToTile(mousePos);
    glm::vec2 worldPos = scene->screenToWorld(mousePos);
    glm::vec2 playerPos = scene->player->getPos();

    if (scene->map->getTile(tilePos) == Block::Empty) {
      scene->map->setTile(tilePos, blockID);
      --amount;
    }
  }

  return;
}
