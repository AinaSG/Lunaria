#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "RockEnemy.h"
#include "Game.h"
#include "Input.h"
#include "ResourceManager.h"



enum RockEnemyAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

void RockEnemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	walkSpeed = 180;
	jumpSpeed = 50;
	strategy = 2;

	hit = false;

	life = 2;
	damage = 1;

	waiting = false;
	waitime = 0;
	waitdir = false;

	kb_speed_x = 400;
    kb_speed_y = 200;

    my_size_x = 32;
    my_size_y = 32;

	waitattack = 500;
    timewaited_attack = 0;

	speed = glm::vec2(0, -100);
	Texture* tex = ResourceManager::instance().getTexture("rock_bub_IP.png");
    if (tex == nullptr) {
      std::cout << "RockEnemy texture not found" << std::endl;
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

}



void RockEnemy::update(int deltaTime)
{
    float dt = deltaTime/1000.0f;
		Player * player = Game::instance().scene.player;

	int dist_to_player = glm::distance(glm::vec2(player->getPos()), glm::vec2(position));
	sprite->update(deltaTime);

	//decide strategy
	if(glm::distance(glm::vec2(player->getPos()), glm::vec2(position))>200){
		strategy = 2;
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
	        }
	    }

			timewaited_attack = timewaited_attack + deltaTime;

		  if(dist_to_player <= 15){
		    if (timewaited_attack >= waitattack){
		      timewaited_attack = 0;
		      player->dealDamage(damage, position);
		    }
		  }

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}
