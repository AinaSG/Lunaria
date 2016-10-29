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
#include <string>

enum PlayerAnims
{
    STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT
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
	  sprite->setNumberAnimations(6);

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

        sprite->setAnimationSpeed(JUMP_RIGHT, 8);
        sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.25f, 0.25f));

        sprite->setAnimationSpeed(JUMP_LEFT, 8);
        sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.f, 0.25f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

    tex = ResourceManager::instance().getTexture("inventory.png");
    if (tex == nullptr) {
      std::cout << "Inventory texture not found" << std::endl;
      return;
    }
    inventorySprite = Sprite::createSprite(glm::ivec2(412,52), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    inventorySprite->setPosition(inventoryPos);

    tex = ResourceManager::instance().getTexture("current_item.png");
    if (tex == nullptr) {
      std::cout << "Current item texture not found" << std::endl;
      return;
    }
    currentItemSprite = Sprite::createSprite(glm::ivec2(48,48), glm::vec2(1.0, 1.0), tex, &shaderProgram);

    tex = ResourceManager::instance().getTexture("cross3.png");
    if (tex == nullptr) {
      std::cout << "Crosshair texture not found" << std::endl;
      return;
    }
    crosshairSprite = Sprite::createSprite(glm::ivec2(11,11), glm::vec2(1.0, 1.0), tex, &shaderProgram);


    for (int i = 0; i < items.size(); ++i) items[i] = new EmptyItem();

    setCurrentItem(0);

    giveItem<BlockItem>(Block::Rock);
    delete items[0];
    items[0] = new EmptyItem();
}



void Player::update(int deltaTime)
{
    float dt = deltaTime/1000.0f;

    glm::ivec2 chPos = Game::instance().scene.worldToScreen(getCrosshairPos());
    Input::instance().setMousePosition(chPos);

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


    items[currentItem]->use(deltaTime);
    if (items[currentItem]->amount == 0) {
      delete items[currentItem];
      items[currentItem] = new EmptyItem();
    }

    if(Input::instance().getKey('a')){
			if (speed.x >= 0 or bJumping) speed.x = -walkSpeed;
	}
    else if(Input::instance().getKey('d')){
        if (speed.x <= 0 or bJumping ) speed.x = walkSpeed;
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
        bool wasHit = map->clampMoveX(position, hitbox, int(speed.x * dt));
        if(wasHit) {
			speed.x = 0;
		}
	}
	else if(speed.x > 0)
    {

        bool wasHit = map->clampMoveX(position, hitbox, int(speed.x * dt));
        if(wasHit) {
			speed.x = 0;
		}
    }

    if(speed.y < 0)
    {
        bool wasHit = map->clampMoveY(position, hitbox, int(floor(speed.y * dt)));
        if (wasHit) {

            speed.y = 0;

        }
    }
    else if(speed.y > 0)
    {
        bool wasHit = map->clampMoveY(position, hitbox, int(ceil(speed.y * dt)));
        if (wasHit) {

          speed.y = 0;
          bJumping = false;
		  hit = false;
        }
    }

    if (bJumping || speed.y > 0) {
      if (speed.x > 0) {
        sprite->changeAnimation(JUMP_RIGHT);
      } else if (speed.x < 0)
        sprite->changeAnimation(JUMP_LEFT);
      else {
        int anim = sprite->animation();
        if (anim == MOVE_RIGHT || anim == STAND_RIGHT)
          sprite->changeAnimation(JUMP_RIGHT);
        else if (anim == MOVE_LEFT || anim == STAND_LEFT)
          sprite->changeAnimation(JUMP_LEFT);
      }
    } else {
      if (speed.x < 0) {
        if (sprite->animation() != MOVE_LEFT)
          sprite->changeAnimation(MOVE_LEFT);
      } else if (speed.x > 0) {
        if (sprite->animation() != MOVE_RIGHT)
          sprite->changeAnimation(MOVE_RIGHT);
      } else {
        int anim = sprite->animation();
        if (anim == MOVE_RIGHT || anim == JUMP_RIGHT)
            sprite->changeAnimation(STAND_RIGHT);
        else if (anim == MOVE_LEFT || anim == JUMP_LEFT)
            sprite->changeAnimation(STAND_LEFT);
      }
    }


    sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x - 8 ), float(tileMapDispl.y + position.y)));
}

void Player::render()
{
  sprite->render();
}

void Player::renderHUD()
{
  inventorySprite->render();
  renderItems();
  currentItemSprite->render();
}

void Player::renderItems()
{
  int i= 0;
  for (Item* item : items) {
    item->render();

    if (item->amount > 0) {
      string n = std::to_string(item->amount);
      Game::instance().scene.boldText.render(n,glm::ivec2(10+45*i + (item->amount > 10 ? 16 : 21),75),15,glm::vec4(1.0f,174.0f/255.0f,0.0f,1.0f));
      shaderProgram->use();
    }
    ++i;
  }
}

void Player::renderCrosshair()
{
  crosshairSprite->setPosition(getCrosshairPos() - glm::ivec2(5,5));
  crosshairSprite->render();
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

glm::ivec2 Player::getCrosshairPos() const
{
  glm::vec2 mousePos = Game::instance().scene.screenToWorld(Input::instance().getMouseScreenPos());
  glm::vec2 myPos = glm::vec2(position + glm::ivec2(8,16));
  glm::vec2 chVector = mousePos - myPos;

  if (glm::length(chVector) > 60.0f) {
    chVector = glm::normalize(chVector) * 60.0f;
  }
  return myPos + chVector;
}

template <class T> void Player::giveItem(int param /* = 0 */)
{
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
