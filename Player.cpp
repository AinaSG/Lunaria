#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Input.h"
#include "ResourceManager.h"
#include "RockItem.h"
#include "DiamondItem.h"
#include <typeinfo>

#define WALK_SPEED 200
#define JUMP_SPEED 400
#define GRAVITY 500

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

Player::Player() : items(9) {}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
    this->shaderProgram = &shaderProgram;

	bJumping = false;
    speed = glm::vec2(0,0);
    Texture* tex = ResourceManager::instance().getTexture("bub.png");
    if (tex == nullptr) {
      std::cout << "Player texture not found" << std::endl;
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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

    tex = ResourceManager::instance().getTexture("inventory.png");
    if (tex == nullptr) {
      std::cout << "Inventory texture not found" << std::endl;
      return;
    }

    inventory = Sprite::createSprite(glm::ivec2(412,52), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    inventory->setPosition(inventoryPos);

    tex = ResourceManager::instance().getTexture("current_item.png");
    if (tex == nullptr) {
      std::cout << "Current item texture not found" << std::endl;
      return;
    }

    currentItemSprite = Sprite::createSprite(glm::ivec2(48,48), glm::vec2(1.0, 1.0), tex, &shaderProgram);


    for (int i = 0; i < items.size(); ++i) items[i] = new EmptyItem();

    setCurrentItem(1);

    giveItem<RockItem>();
    giveItem<DiamondItem>();
    giveItem<EmptyItem>();

    std::cout << items[0]->amount;
}



void Player::update(int deltaTime)
{
    float dt = deltaTime/1000.0f;
	sprite->update(deltaTime);

    items[currentItem]->use(deltaTime);

    if(Input::instance().getKey('a')){
		speed.x = -WALK_SPEED;
	}
    else if(Input::instance().getKey('d')){
		speed.x = WALK_SPEED;
	}
	else{
		speed.x = 0;
	}

    speed.y += GRAVITY*dt;

    if(!bJumping && (Input::instance().getKey('w') || Input::instance().getKey(' '))){
		speed.y = -JUMP_SPEED;
		bJumping = true;
	}

	if(speed.x < 0)
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);

        bool hit = map->clampMoveX(posPlayer, glm::ivec2(32, 32), int(speed.x * dt));
		if(hit) {

			sprite->changeAnimation(STAND_LEFT);
			speed.x = 0;
		}
	}
	else if(speed.x > 0)
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);

        bool hit = map->clampMoveX(posPlayer, glm::ivec2(32, 32), int(speed.x * dt));
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
        bool hit = map->clampMoveY(posPlayer, glm::ivec2(32, 32), int(floor(speed.y * dt)));
        if (hit) {

            speed.y = 0;

        }
    }
    else if(speed.y > 0)
    {
        //if(sprite->animation() != MOVE_RIGHT)
        //	sprite->changeAnimation(MOVE_RIGHT);
        bool hit = map->clampMoveY(posPlayer, glm::ivec2(32, 32), int(ceil(speed.y * dt)));
        if (hit) {

          speed.y = 0;
            bJumping = false;
        }
    }

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
  sprite->render();
}

void Player::renderInventory()
{
  inventory->render();
  renderItems();
  currentItemSprite->render();
}

void Player::renderItems()
{
  for (Item* i : items) {
    i->render();
  }
}

void Player::setCurrentItem(int n)
{
  currentItem = n;
  currentItemSprite->setPosition(inventoryPos + glm::ivec2(2+45*n,2));
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPos() const
{
	return posPlayer;
}

glm::ivec2 Player::getSpeed() const
{
    return glm::ivec2(speed);
}

template <class T> void Player::giveItem()
{
  std::cout << typeid(T).name() << std::endl;
  if (typeid(T) == typeid(EmptyItem)) return;

  for (Item* i : items) {
    if (dynamic_cast<T*>(i) != nullptr) {
      i->amount++;
      return;
    }
  }

  for (int i = 0; i < items.size(); ++i) {
    if (dynamic_cast<EmptyItem*>(items[i]) != nullptr) {
      delete items[i];
      items[i] = new T();
      items[i]->init(*shaderProgram);
      items[i]->setPosition(inventoryPos + glm::ivec2(10+45*i,10));
      return;
    }
  }
  std::cout << "Inventory full" << std::endl;
}
