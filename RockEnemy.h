#ifndef _ROCKENEMY_INCLUDE
#define _ROCKENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Character.h"


// RockEnemy is basically a Sprite that represents the rockenemy. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class RockEnemy : public Character
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	int strategy; //1 = Wander, 2 = Attack
	bool waiting;
	int waitime;
	bool waitdir; //true = <--- ; false = --->
};


#endif // _ROCKENEMY_INCLUDE
