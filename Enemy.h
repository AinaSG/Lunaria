#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Character.h"


// Enemy is basically a Sprite that represents the enemy. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Enemy : public Character
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	int strategy; //1 = Wander, 2 = Attack
	bool waiting;
	int waitime;

	
	bool waitdir; //true = <--- ; false = --->
};


#endif // _ENEMY_INCLUDE
