#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Input.h"
#include "ResourceManager.h"
#include "BlockItem.h"
#include <typeinfo>



enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

Player::Player() : items(9) {}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
    this->shaderProgram = &shaderProgram;

	bJumping = false;
	hit = false;
	walkSpeed = 200;
	jumpSpeed = 400;

	my_size_x = 32;
	my_size_y = 32;

	kb_speed_x = 200;
	kb_speed_y = 200;

	life = 10;
	damage = 1;
	speed = glm::vec2(0,0);
	Texture* tex = ResourceManager::instance().getTexture("bub_astIP.png");
    if (tex == nullptr) {
      std::cout << "Player texture not found" << std::endl;
      return;
    }

    sprite = Sprite::createSprite(glm::ivec2(my_size_x, my_size_y), glm::vec2(0.25, 0.25), tex, &shaderProgram);
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

    giveItem<BlockItem>(Block::RedMaterial);
    giveItem<BlockItem>(Block::Rock);

    std::cout << items[0]->amount;
}



void Player::update(int deltaTime)
{
    float dt = deltaTime/1000.0f;

	sprite->update(deltaTime);

    for (int i = 1; i <= 9; ++i) {
      if (Input::instance().getKeyDown('0' + i)) {
        setCurrentItem(i-1);
      }
    }

    if (Input::instance().getMouseWheel() < 0) {
      setCurrentItem(++currentItem%9);
    } else if (Input::instance().getMouseWheel() > 0) {
      setCurrentItem((--currentItem+9)%9);
    }

    if (!bJumping) {
      items[currentItem]->use(deltaTime);
      if (items[currentItem]->amount == 0) {
        delete items[currentItem];
        items[currentItem] = new EmptyItem();
      }
    }

    if(Input::instance().getKey('a')){
			if (speed.x <= 0 or bJumping) speed.x = -walkSpeed;
	}
    else if(Input::instance().getKey('d')){
        if (speed.x >= 0 or bJumping ) speed.x = walkSpeed;
	}
	else{
		if (!hit){
			speed.x = 0;
		}
	}

    speed.y += GRAVITY*dt;

    if(!bJumping && (Input::instance().getKey('w') || Input::instance().getKey(' '))){
		speed.y = -jumpSpeed;
		bJumping = true;
	}

	if(speed.x < 0)
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);

        bool hited = map->clampMoveX(position, glm::ivec2(32, 32), int(speed.x * dt));
		if(hited) {

			sprite->changeAnimation(STAND_LEFT);
			speed.x = 0;
		}
	}
	else if(speed.x > 0)
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);

        bool hited = map->clampMoveX(position, glm::ivec2(32, 32), int(speed.x * dt));
		if(hited) {

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
        bool hited = map->clampMoveY(position, glm::ivec2(32, 32), int(floor(speed.y * dt)));
        if (hited) {

            speed.y = 0;

        }
    }
    else if(speed.y > 0)
    {
        //if(sprite->animation() != MOVE_RIGHT)
        //	sprite->changeAnimation(MOVE_RIGHT);
        bool hited = map->clampMoveY(position, glm::ivec2(32, 32), int(ceil(speed.y * dt)));
        if (hited) {

          speed.y = 0;
            bJumping = false;
						hit = false;
        }
    }

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
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
    position = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

glm::ivec2 Player::getPos() const
{
    return position;
}

glm::ivec2 Player::getSpeed() const
{
    return glm::ivec2(speed);
}

void Player::attack(int hit_damage)
{
	if (hit_damage == -1) hit_damage = damage;
	Scene* scene = &(Game::instance().scene);
	glm::ivec2 click_pos = scene->screenToWorld(Input::instance().getMouseScreenPos());
	if(glm::distance(glm::vec2(click_pos), glm::vec2(position)) < 600){
		for(int i = 0; i<scene->enemyVector.size(); ++i){
			//glm::vec2 enemy_pos = scene->enemyVector[i]->getPos();
			if (scene->enemyVector[i]->pointInside(click_pos)){
				scene->enemyVector[i]->dealDamage(damage, position);
				break;
			}
		}
		for(int i = 0; i<scene->rockEnemyVector.size(); ++i){
			//glm::vec2 enemy_pos = scene->enemyVector[i]->getPos();
			if (scene->rockEnemyVector[i]->pointInside(click_pos)){
				scene->rockEnemyVector[i]->dealDamage(damage, position);
				break;
			}
		}
	}
}

template <class T> void Player::giveItem(int param /* = 0 */)
{
  std::cout << typeid(T).name() << std::endl;
  if (typeid(T) == typeid(EmptyItem)) return;

  for (Item* i : items) {
    if (dynamic_cast<T*>(i) != nullptr) {
      if (typeid(T) == typeid(BlockItem)) {
        if (dynamic_cast<BlockItem*>(i)->getBlockID() != param) continue;
      }
      if (i->amount != -1) i->amount++;
      return;
    }
  }

  for (int i = 0; i < items.size(); ++i) {
    if (dynamic_cast<EmptyItem*>(items[i]) != nullptr) {
      delete items[i];
      items[i] = new T();
      items[i]->init(*shaderProgram, param);
      items[i]->setPosition(inventoryPos + glm::ivec2(10+45*i,10));
      return;
    }
  }
  std::cout << "Inventory full" << std::endl;
}
