#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Character.h"
#include "Game.h"
#include "Input.h"


enum CharacterAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

void Character::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	cout << "leliao" << endl;
	bJumping = false;
	dead = false;
	speed = glm::vec2(0,0);
	spritesheet.loadFromFile("images/bub_astIP.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
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



void Character::update(int deltaTime)
{
  float dt = deltaTime/1000.0f;
	sprite->update(deltaTime);

    if(Input::instance().getSpecialKey(GLUT_KEY_LEFT)){
		speed.x = -walkSpeed;
	}
    else if(Input::instance().getSpecialKey(GLUT_KEY_RIGHT)){
		speed.x = walkSpeed;
	}
	else{
		speed.x = 0;
	}

    speed.y += GRAVITY*dt;

    if(!bJumping && Input::instance().getSpecialKey(GLUT_KEY_UP)){
		speed.y = -jumpSpeed;
		bJumping = true;
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

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

void Character::render()
{
	sprite->render();
}

void Character::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Character::setPosition(const glm::vec2 &pos)
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

glm::ivec2 Character::getPos() const
{
	return position;
}

int Character::getLife()
{
	return life;
}

void Character::setLife( int l)
{
	life = l;
}

bool Character::isDead()
{
	return dead;
}

int Character::dealDamage(int damage)
{
	life = life - damage;
	if (life < 0 ){
		life = 0;
		dead = true;
	}
}

glm::ivec2 Character::getSpeed() const
{
    return glm::ivec2(speed);
}
