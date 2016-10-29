#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"
#include "Input.h"
#include "ResourceManager.h"



enum EnemyAnims
{
  STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
  bJumping = false;
  walkSpeed = 100;
  jumpSpeed = 200;
  strategy = 1;

  life = 3;
  damage = 2;

  waiting = false;
  waitime = 0;
  waitdir = false;

  speed = glm::vec2(0,0);
  Texture* tex = ResourceManager::instance().getTexture("evil_bub.png");
  if (tex == nullptr) {
    std::cout << "Enemy texture not found" << std::endl;
    return;
  }

  sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), tex, &shaderProgram);
  sprite->setNumberAnimations(4);

  sprite->setAnimationSpeed(STAND_LEFT, 8);
  sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

  sprite->setAnimationSpeed(STAND_RIGHT, 8);
  sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

  sprite->setAnimationSpeed(MOVE_LEFT, 8);
  sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
  sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
  sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

  sprite->setAnimationSpeed(MOVE_RIGHT, 8);
  sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
  sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
  sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

  sprite->changeAnimation(0);
  tileMapDispl = tileMapPos;
  sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

}



void Enemy::update(int deltaTime)
{
  float dt = deltaTime/1000.0f;
  Player * player = Game::instance().scene.player;

  sprite->update(deltaTime);

  //decide strategy
  if(glm::distance(glm::vec2(player->getPos()), glm::vec2(position))>200){
    strategy = 1;
  }
  else{
    strategy = 2;
  }

  if (strategy == 1){ //Wander
    if (waitime > 500){
      waiting = !waiting;
      waitime = 0;
      int changedir = rand()%3;
      if (changedir == 0){
        waitdir = !waitdir;
      }
    }

    waitime = waitime + deltaTime;

    if (waiting){
      speed.x = 0;
      speed.y += GRAVITY*dt;

    }
    else {
      if (waitdir){
        speed.x = -walkSpeed;
        speed.y += GRAVITY*dt;
      }
      else {
        speed.x = walkSpeed;
        speed.y += GRAVITY*dt;
      }
    }
  }

  if (strategy == 2){ //Attack
    if(player->getPos().x < position.x){
      speed.x = -walkSpeed;
    }
    else if(player->getPos().x > position.x){
      speed.x = walkSpeed;
    }
    else{
      speed.x = 0;
    }

    speed.y += GRAVITY*dt;

    if(!bJumping && player->getPos().y <= position.y){
      speed.y = -jumpSpeed;
      bJumping = true;
    }

  }

  if(speed.x < 0)
  {
    if(sprite->animation() != MOVE_LEFT)
      sprite->changeAnimation(MOVE_LEFT);

    bool hit = map->clampMoveX(position, glm::ivec2(32, 32), int(speed.x * dt));
    if(hit) {

      sprite->changeAnimation(STAND_LEFT);
      speed.x = 0;
    }
  }
  else if(speed.x > 0)
  {
    if(sprite->animation() != MOVE_RIGHT)
      sprite->changeAnimation(MOVE_RIGHT);

    bool hit = map->clampMoveX(position, glm::ivec2(32, 32), int(speed.x * dt));
    if(hit) {

      sprite->changeAnimation(STAND_RIGHT);
      speed.x = 0;
    }
  }
  else
  {
    if(sprite->animation() == MOVE_LEFT)
      sprite->changeAnimation(STAND_LEFT);
    else if(sprite->animation() == MOVE_RIGHT)
      sprite->changeAnimation(STAND_RIGHT);
  }

  if(speed.y < 0)
  {
    //if(sprite->animation() != MOVE_LEFT)
    //	sprite->changeAnimation(MOVE_LEFT);
    bool hit = map->clampMoveY(position, glm::ivec2(32, 32), int(floor(speed.y * dt)));
    if (hit) {

      speed.y = 0;

    }
  }
  else if(speed.y > 0)
  {
    //if(sprite->animation() != MOVE_RIGHT)
    //	sprite->changeAnimation(MOVE_RIGHT);
    bool hit = map->clampMoveY(position, glm::ivec2(32, 32), int(ceil(speed.y * dt)));
    if (hit) {

      speed.y = 0;
      bJumping = false;
    }
  }

  sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}