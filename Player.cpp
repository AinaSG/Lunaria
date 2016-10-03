#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define WALK_SPEED 6
#define JUMP_SPEED 15
#define GRAVITY 1

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
	sprite->update(deltaTime);

	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
		speed.x = -WALK_SPEED;
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
		speed.x = WALK_SPEED;
	}
	else{
		speed.x = 0;
	}

	speed.y += GRAVITY;
	if(!bJumping && Game::instance().getSpecialKey(GLUT_KEY_UP)){
		speed.y = -JUMP_SPEED;
		bJumping = true;
	}


	if(speed.x < 0)
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);

		bool hit = map->clampMoveX(posPlayer, glm::ivec2(32, 32), speed.x);
		if(hit) sprite->changeAnimation(STAND_LEFT);
	}
	else if(speed.x > 0)
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);

		bool hit = map->clampMoveX(posPlayer, glm::ivec2(32, 32), speed.x);
		if(hit) sprite->changeAnimation(STAND_RIGHT);
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
		bool hit = map->clampMoveY(posPlayer, glm::ivec2(32, 32), speed.y);
		if (hit) {
			speed.y = 0;
		}
		/*posPlayer.y += speed.y;
		if(map->collisionMoveUp(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.y -= speed.y;
			//sprite->changeAnimation(STAND_LEFT);
		}*/
	}
	else if(speed.y > 0)
	{
		//if(sprite->animation() != MOVE_RIGHT)
		//	sprite->changeAnimation(MOVE_RIGHT);
		bool hit = map->clampMoveY(posPlayer, glm::ivec2(32, 32), speed.y);
		if (hit) {
			speed.y = 0;
			bJumping = false;
		}
		/*posPlayer.y += speed.y;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32)))
		{
			speed.y = 0;
			posPlayer.y = (map->nearestPosYGround(posPlayer)).y;
			//posPlayer.y -= speed.y;
			//sprite->changeAnimation(STAND_RIGHT);
		}*/
	}

	/*if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(!map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if(jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if(jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			}
		}
		else
		{
			bJumping = false;

		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}*/

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
