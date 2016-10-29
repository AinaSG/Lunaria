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
		bool isDead() const;
		int dealDamage(int damage,glm::vec2 enemy_pos);
		bool pointInside(glm::vec2 click);

protected:
	bool bJumping;
	glm::ivec2 tileMapDispl, position;
    Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
    glm::vec2 speed;
		int walkSpeed;
		int jumpSpeed;
		int my_size_x;
		int my_size_y;

		int kb_speed_x;
		int kb_speed_y;

		int life;
		bool dead;
		bool hit;
		int damage;

		int waitattack;
		int timewaited_attack;

		const int GRAVITY = 500;

};


#endif // _CHARACTER_INCLUDE
