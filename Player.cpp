#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Input.h"

#define WALK_SPEED 200
#define JUMP_SPEED 400
#define GRAVITY 500

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	speed = glm::vec2(0,0);
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
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
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}



void Player::update(int deltaTime)
{
    float dt = deltaTime/1000.0f;
    dt = 0.017;
	sprite->update(deltaTime);

    if(Input::instance().getSpecialKey(GLUT_KEY_LEFT)){
		speed.x = -WALK_SPEED;
	}
    else if(Input::instance().getSpecialKey(GLUT_KEY_RIGHT)){
		speed.x = WALK_SPEED;
	}
	else{
		speed.x = 0;
	}

    speed.y += GRAVITY*dt;

    if(!bJumping && Input::instance().getSpecialKey(GLUT_KEY_UP)){
		speed.y = -JUMP_SPEED;
		bJumping = true;
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

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
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
