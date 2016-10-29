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
    void renderLife();
    void renderShop();
    void rendeShopItems();
    void renderCrosshair();
    void renderHitEffect();

    void postDamage();

    bool can_buy_potion = false;
    bool can_buy_taladro = false;
    bool can_buy_espasa = false;
    bool can_buy_stage = false;

    int shipStage = 0;


	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

    glm::ivec2 getPos() const;
    glm::ivec2 getSpeed() const;
    glm::ivec2 getCrosshairPos() const;

    void attack(int hit_damage = -1);
    template <class T> void giveItem(int param = 0);

private:
    glm::ivec2 tileMapDispl;
    Sprite *inventorySprite, *currentItemSprite, *currentItemShopSprite, *crosshairSprite, *heartSprite, *hitSprite, *shopSprite;
    Sprite *taladroSprite, *stage1Sprite, *stage2Sprite, *stage3Sprite, *potionSprite, *swordSprite;
    Sprite *taladroSpriteBW, *stage1SpriteBW, *stage2SpriteBW, *stage3SpriteBW, *potionSpriteBW, *swordSpriteBW;

    const glm::ivec2 taladroPos = glm::ivec2(865,668);
    const glm::ivec2 stagePos = glm::ivec2(910,668);
    const glm::ivec2 potionPos = glm::ivec2(775,668);
    const glm::ivec2 swordPos = glm::ivec2(820,668);


    ShaderProgram *shaderProgram;
    int currentItem;
    int currentItemShop;

    int timePostHit;

    int postHitCounter;
    bool hitEffect;

    const glm::ivec2 inventoryPos = glm::ivec2(10,10);
    const glm::ivec2 shopPos = glm::ivec2(765,658);
    const glm::ivec2 hitbox =  glm::ivec2(16,32);

    vector<Item*> items;
    vector<Sprite*> shopItems;

    void renderItems();
    void checkShop();
    void setCurrentItem(int n);
    void setCurrentItemShop(int n);
};


#endif // _PLAYER_INCLUDE
