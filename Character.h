#ifndef _CHARACTER_INCLUDE
#define _CHARACTER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Character is basically a Sprite that represents the character. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Character
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
    glm::ivec2 getPos() const;
    glm::ivec2 getSpeed() const;
		int getLife();
		void setLife( int l);
		bool isDead();
		int dealDamage(int damage);

protected:
	bool bJumping;
	glm::ivec2 tileMapDispl, position;
    Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
    glm::vec2 speed;
		int walkSpeed;
		int jumpSpeed;
		int life;
		bool dead;
		int damage;
		const int GRAVITY = 500;

};


#endif // _CHARACTER_INCLUDE
