#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Item.h"
#include "EmptyItem.h"
#include "Character.h"
#include "BlockItem.h"
#include <typeinfo>

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player : public Character
{

public:
    Player();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
  void buy();

	void render();
    void renderHUD();
    void renderLife();
    void renderShop();
    void renderHelp();
    void rendeShopItems();
    void renderCrosshair();
    void renderHitEffect();

    void postDamage();
    void onDeath();
    void heal(int hp);

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
    void setCrosshairPos(glm::vec2 p);

    void attack(int hit_damage = -1);
    bool isJumping() { return bJumping; }

    template <class T> void giveItem(int param = 0) {

      if (typeid(T) == typeid(EmptyItem)) return;

      for (Item* i : items) {
        if (dynamic_cast<T*>(i) != nullptr) {
          if (typeid(T) == typeid(BlockItem)) {
            if (dynamic_cast<BlockItem*>(i)->getBlockID() != param) continue;
          }
          if (i->amount != -1) i->amount++;
          return;
        }
      }

      for (int i = 0; i < items.size(); ++i) {
        if (dynamic_cast<EmptyItem*>(items[i]) != nullptr) {
          delete items[i];
          items[i] = new T();
          items[i]->init(*shaderProgram, param);
          items[i]->setPosition(inventoryPos + glm::ivec2(10+45*i,10));
          return;
        }
      }
    };

private:
    glm::ivec2 tileMapDispl;
    Sprite *inventorySprite, *currentItemSprite, *currentItemShopSprite, *crosshairSprite, *heartSprite, *hitSprite, *shopSprite;
    Sprite *taladroSprite, *stage1Sprite, *stage2Sprite, *stage3Sprite, *potionSprite, *swordSprite;
    Sprite *taladroHelpSprite, *stage1HelpSprite, *stage2HelpSprite, *stage3HelpSprite, *potionHelpSprite, *swordHelpSprite;
    Sprite *taladroSpriteBW, *stage1SpriteBW, *stage2SpriteBW, *stage3SpriteBW, *potionSpriteBW, *swordSpriteBW;

    const glm::ivec2 taladroPos = glm::ivec2(865,668);
    const glm::ivec2 stagePos = glm::ivec2(910,668);
    const glm::ivec2 potionPos = glm::ivec2(775,668);
    const glm::ivec2 swordPos = glm::ivec2(820,668);
    const int maxLife = 14;

    Texture* normal_player;
    Texture* bone_player;
    Texture* diamond_player;
    Texture* drill_player;
    Texture* glue_player;
    Texture* medicine_player;
    Texture* ruby_player;
    Texture* ship_player;
    Texture* stone_player;
    Texture* sword_player;

    glm::vec2 crosshairPos;



    ShaderProgram *shaderProgram;
    int currentItem;
    int currentItemShop;

    int timePostHit;

    int postHitCounter;
    bool hitEffect;

    bool showHelp = false;
    const glm::ivec2 helpPos = glm::ivec2(765,544);

    const glm::ivec2 inventoryPos = glm::ivec2(10,10);
    const glm::ivec2 shopPos = glm::ivec2(765,658);
    const glm::ivec2 hitbox =  glm::ivec2(16,32);

    vector<Item*> items;
    vector<Sprite*> shopItems;

    void renderItems();
    void checkShop();
    void setCurrentItem(int n);
    void setCurrentItemShop(int n);
    void updateViewItems();
};


#endif // _PLAYER_INCLUDE
