#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Item.h"
#include "EmptyItem.h"
#include "Character.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player : public Character
{

public:
    Player();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
    void renderHUD();
    void renderCrosshair();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

    glm::ivec2 getPos() const;
    glm::ivec2 getSpeed() const;
    glm::ivec2 getCrosshairPos() const;

    void attack(int hit_damage = -1);
    template <class T> void giveItem(int param = 0);

private:
    glm::ivec2 tileMapDispl;
    Sprite *inventorySprite, *currentItemSprite, *crosshairSprite;
    ShaderProgram *shaderProgram;
    int currentItem;
    const glm::ivec2 inventoryPos = glm::ivec2(10,10);
    const glm::ivec2 hitbox =  glm::ivec2(16,32);

    vector<Item*> items;

    void renderItems();
    void setCurrentItem(int n);
};


#endif // _PLAYER_INCLUDE
