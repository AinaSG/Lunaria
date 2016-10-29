#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Input.h"
#include "ResourceManager.h"
#include "BlockItem.h"
#include <typeinfo>



enum PlayerAnims
{
    STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT
};

Player::Player() : items(9) {}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
    this->shaderProgram = &shaderProgram;

	bJumping = false;
	hit = false;
	walkSpeed = 200;
	jumpSpeed = 400;

	my_size_x = 32;
	my_size_y = 32;

	kb_speed_x = 200;
	kb_speed_y = 200;

  timePostHit = 10;
  postHitCounter = 0;
  hitEffect = false;

	life = 10;
	damage = 1;
	speed = glm::vec2(0,0);
	Texture* tex = ResourceManager::instance().getTexture("bub_astIP.png");
    if (tex == nullptr) {
      std::cout << "Player texture not found" << std::endl;
      return;
    }

    sprite = Sprite::createSprite(glm::ivec2(my_size_x, my_size_y), glm::vec2(0.25, 0.25), tex, &shaderProgram);

	  sprite->setNumberAnimations(6);

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

        sprite->setAnimationSpeed(JUMP_RIGHT, 8);
        sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.25f, 0.25f));

        sprite->setAnimationSpeed(JUMP_LEFT, 8);
        sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.f, 0.25f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));

    tex = ResourceManager::instance().getTexture("inventory.png");
    if (tex == nullptr) {
      std::cout << "Inventory texture not found" << std::endl;
      return;
    }
    inventorySprite = Sprite::createSprite(glm::ivec2(412,52), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    inventorySprite->setPosition(inventoryPos);

    tex = ResourceManager::instance().getTexture("hit.png");
    if (tex == nullptr) {
      std::cout << "Hit texture not found" << std::endl;
      return;
    }
    hitSprite = Sprite::createSprite(glm::ivec2(960,720), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    hitSprite->setPosition(glm::vec2(0,0));

    tex = ResourceManager::instance().getTexture("current_item.png");
    if (tex == nullptr) {
      std::cout << "Current item texture not found" << std::endl;
      return;
    }
    currentItemSprite = Sprite::createSprite(glm::ivec2(48,48), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    currentItemShopSprite = Sprite::createSprite(glm::ivec2(48,48), glm::vec2(1.0, 1.0), tex, &shaderProgram);

    tex = ResourceManager::instance().getTexture("hrt.png");

    if (tex == nullptr) {
      std::cout << "Heart texture not found" << std::endl;
      return;
    }

    heartSprite = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);

    tex = ResourceManager::instance().getTexture("shop.png");

    if (tex == nullptr) {
      std::cout << "Shop texture not found" << std::endl;
      return;
    }

    shopSprite = Sprite::createSprite(glm::ivec2(185,52), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    shopSprite->setPosition(shopPos);

    tex = ResourceManager::instance().getTexture("cross3.png");
    if (tex == nullptr) {
      std::cout << "Crosshair texture not found" << std::endl;
      return;
    }
    crosshairSprite = Sprite::createSprite(glm::ivec2(11,11), glm::vec2(1.0, 1.0), tex, &shaderProgram);

    /////////////////////LOADING SHOP ITEMS////////////////////////////////////////////
    tex = ResourceManager::instance().getTexture("taladro.png");

    if (tex == nullptr) {
      std::cout << "Taladro texture not found" << std::endl;
      return;
    }

    taladroSprite = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    taladroSprite->setPosition(taladroPos);

    tex = ResourceManager::instance().getTexture("taladro_bw.png");

    if (tex == nullptr) {
      std::cout << "Taladro_bw texture not found" << std::endl;
      return;
    }

    taladroSpriteBW = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    taladroSpriteBW->setPosition(taladroPos);

    tex = ResourceManager::instance().getTexture("fase1.png");

    if (tex == nullptr) {
      std::cout << "Stage1 texture not found" << std::endl;
      return;
    }

    stage1Sprite = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    stage1Sprite->setPosition(stagePos);

    tex = ResourceManager::instance().getTexture("fase1_bw.png");

    if (tex == nullptr) {
      std::cout << "Stage1_bw texture not found" << std::endl;
      return;
    }

    stage1SpriteBW = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    stage1SpriteBW->setPosition(stagePos);

    tex = ResourceManager::instance().getTexture("fase2.png");

    if (tex == nullptr) {
      std::cout << "Stage2 texture not found" << std::endl;
      return;
    }

    stage2Sprite = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    stage2Sprite->setPosition(stagePos);

    tex = ResourceManager::instance().getTexture("fase2_bw.png");

    if (tex == nullptr) {
      std::cout << "Stage2_bw texture not found" << std::endl;
      return;
    }

    stage2SpriteBW = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    stage2SpriteBW->setPosition(stagePos);

    tex = ResourceManager::instance().getTexture("fase3.png");

    if (tex == nullptr) {
      std::cout << "Stage3 texture not found" << std::endl;
      return;
    }

    stage3Sprite = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    stage3Sprite->setPosition(stagePos);

    tex = ResourceManager::instance().getTexture("fase3_bw.png");

    if (tex == nullptr) {
      std::cout << "Stage3_bw texture not found" << std::endl;
      return;
    }

    stage3SpriteBW = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    stage3SpriteBW->setPosition(stagePos);

    tex = ResourceManager::instance().getTexture("medi.png");

    if (tex == nullptr) {
      std::cout << "Potion texture not found" << std::endl;
      return;
    }

    potionSprite = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    potionSprite->setPosition(potionPos);

    tex = ResourceManager::instance().getTexture("medi_bw.png");

    if (tex == nullptr) {
      std::cout << "Potion_bw texture not found" << std::endl;
      return;
    }

    potionSpriteBW = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    potionSpriteBW->setPosition(potionPos);

    tex = ResourceManager::instance().getTexture("sword.png");

    if (tex == nullptr) {
      std::cout << "Sword texture not found" << std::endl;
      return;
    }

    swordSprite = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    swordSprite->setPosition(swordPos);

    tex = ResourceManager::instance().getTexture("sword_bw.png");

    if (tex == nullptr) {
      std::cout << "Sword_bw texture not found" << std::endl;
      return;
    }

    swordSpriteBW = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    swordSpriteBW->setPosition(swordPos);


    ////////////////////////END SHOP ITEMS /////////////////////////////////////////////
    ////////////////////////LOADING HELP ///////////////////////////////////////////////
    tex = ResourceManager::instance().getTexture("help_drill.png");

    if (tex == nullptr) {
      std::cout << "Drill help texture not found" << std::endl;
      return;
    }

    taladroHelpSprite = Sprite::createSprite(glm::ivec2(185,104), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    taladroHelpSprite->setPosition(helpPos);

    tex = ResourceManager::instance().getTexture("help_1s.png");

    if (tex == nullptr) {
      std::cout << "S1 help texture not found" << std::endl;
      return;
    }

    stage1HelpSprite = Sprite::createSprite(glm::ivec2(185,104), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    stage1HelpSprite->setPosition(helpPos);

    tex = ResourceManager::instance().getTexture("help_2s.png");

    if (tex == nullptr) {
      std::cout << "S2 help texture not found" << std::endl;
      return;
    }

    stage2HelpSprite = Sprite::createSprite(glm::ivec2(185,104), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    stage2HelpSprite->setPosition(helpPos);

    tex = ResourceManager::instance().getTexture("help_3s.png");

    if (tex == nullptr) {
      std::cout << "S3 help texture not found" << std::endl;
      return;
    }

    stage3HelpSprite = Sprite::createSprite(glm::ivec2(185,104), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    stage3HelpSprite->setPosition(helpPos);

    tex = ResourceManager::instance().getTexture("help_medicine.png");

    if (tex == nullptr) {
      std::cout << "Medicine help texture not found" << std::endl;
      return;
    }

    potionHelpSprite = Sprite::createSprite(glm::ivec2(185,104), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    potionHelpSprite->setPosition(helpPos);

    tex = ResourceManager::instance().getTexture("help_sword.png");

    if (tex == nullptr) {
      std::cout << "Sword help texture not found" << std::endl;
      return;
    }

    swordHelpSprite = Sprite::createSprite(glm::ivec2(185,104), glm::vec2(1.0, 1.0), tex, &shaderProgram);
    swordHelpSprite->setPosition(helpPos);
    ////////////////////////END HELP ///////////////////////////////////////////////////

    for (int i = 0; i < items.size(); ++i) items[i] = new EmptyItem();

    setCurrentItem(0);
    setCurrentItemShop(0);

    giveItem<BlockItem>(Block::Rock);
    delete items[0];
    items[0] = new EmptyItem();
}



void Player::update(int deltaTime)
{
    float dt = deltaTime/1000.0f;

    glm::ivec2 chPos = Game::instance().scene.worldToScreen(getCrosshairPos());
    Input::instance().setMousePosition(chPos);

    if(hitEffect){
      postHitCounter = postHitCounter + deltaTime;
    }
    if(postHitCounter > timePostHit ){
      postHitCounter = 0;
      hitEffect = false;
    }

  checkShop();

	sprite->update(deltaTime);

  if (Input::instance().getSpecialKeyDown(GLUT_KEY_RIGHT)) {
    setCurrentItemShop(++currentItemShop%4);
  } else if (Input::instance().getSpecialKeyDown(GLUT_KEY_LEFT)) {
    setCurrentItemShop((--currentItemShop+4)%4);
  }

  if (Input::instance().getSpecialKeyDown(GLUT_KEY_UP)) {
    showHelp = !showHelp;
  }

    for (int i = 1; i <= 9; ++i) {
      if (Input::instance().getKeyDown('0' + i)) {
        setCurrentItem(i-1);
      }
    }

    if (Input::instance().getMouseWheel() < 0) {
      setCurrentItem(++currentItem%9);
    } else if (Input::instance().getMouseWheel() > 0) {
      setCurrentItem((--currentItem+9)%9);
    }

    if (!bJumping) {
      items[currentItem]->use(deltaTime);
      if (items[currentItem]->amount == 0) {
        delete items[currentItem];
        items[currentItem] = new EmptyItem();
      }
    }

    if(Input::instance().getKey('a')){
			if (speed.x >= 0 or bJumping) speed.x = -walkSpeed;
	}
    else if(Input::instance().getKey('d')){
        if (speed.x <= 0 or bJumping ) speed.x = walkSpeed;
	}
	else{
		if (!hit){
			speed.x = 0;
		}
	}

    speed.y += GRAVITY*dt;

    if(!bJumping && (Input::instance().getKey('w') || Input::instance().getKey(' '))){
		speed.y = -jumpSpeed;
		bJumping = true;
	}

	if(speed.x < 0)
    {
        bool wasHit = map->clampMoveX(position, hitbox, int(speed.x * dt));
        if(wasHit) {
			speed.x = 0;
		}
	}
	else if(speed.x > 0)
    {

        bool wasHit = map->clampMoveX(position, hitbox, int(speed.x * dt));
        if(wasHit) {
			speed.x = 0;
		}
    }

    if(speed.y < 0)
    {
        bool wasHit = map->clampMoveY(position, hitbox, int(floor(speed.y * dt)));
        if (wasHit) {

            speed.y = 0;

        }
    }
    else if(speed.y > 0)
    {
        bool wasHit = map->clampMoveY(position, hitbox, int(ceil(speed.y * dt)));
        if (wasHit) {

          speed.y = 0;
          bJumping = false;
		  hit = false;
        }
    }

    if (bJumping || speed.y > 0) {
      if (speed.x > 0) {
        sprite->changeAnimation(JUMP_RIGHT);
      } else if (speed.x < 0)
        sprite->changeAnimation(JUMP_LEFT);
      else {
        int anim = sprite->animation();
        if (anim == MOVE_RIGHT || anim == STAND_RIGHT)
          sprite->changeAnimation(JUMP_RIGHT);
        else if (anim == MOVE_LEFT || anim == STAND_LEFT)
          sprite->changeAnimation(JUMP_LEFT);
      }
    } else {
      if (speed.x < 0) {
        if (sprite->animation() != MOVE_LEFT)
          sprite->changeAnimation(MOVE_LEFT);
      } else if (speed.x > 0) {
        if (sprite->animation() != MOVE_RIGHT)
          sprite->changeAnimation(MOVE_RIGHT);
      } else {
        int anim = sprite->animation();
        if (anim == MOVE_RIGHT || anim == JUMP_RIGHT)
            sprite->changeAnimation(STAND_RIGHT);
        else if (anim == MOVE_LEFT || anim == JUMP_LEFT)
            sprite->changeAnimation(STAND_LEFT);
      }
    }


    sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x - 8 ), float(tileMapDispl.y + position.y)));
}

void Player::render()
{
  sprite->render();
}

void Player::renderHUD()
{
  inventorySprite->render();
  renderItems();
  renderLife();
  renderShop();
  renderHelp();
  if (hitEffect){
    renderHitEffect();
  }
  currentItemSprite->render();
  currentItemShopSprite->render();
}

void Player::renderHelp()
{
  if(showHelp){
    if (currentItemShop == 0){
      potionHelpSprite->render();
    }
    else if (currentItemShop == 1){
      swordHelpSprite->render();
    }
    else if (currentItemShop == 2){
      taladroHelpSprite->render();
    }
    else {
      if(shipStage == 0){
          stage1HelpSprite->render();
      }
      else if (shipStage == 1){
          stage2HelpSprite->render();
      }
      else {
          stage3HelpSprite->render();
      }
    }
  }
}

void Player::renderLife()
{
  int x_heart = 900;
  int y_heart = 20;
  int distance_bh = 37;

  for (int i=0; i < life; ++i){
    heartSprite->setPosition(glm::vec2(x_heart-(i*distance_bh), y_heart));
    heartSprite->render();
  }
}

void Player::postDamage(){
  hitEffect  = true;
  cout <<"POSTDAM" << endl;
}

void Player::renderItems()
{
  for (Item* i : items) {
    i->render();
  }
}

void Player::checkShop(){

}

void Player::renderShop()
{
  shopSprite->render();
  if (can_buy_potion){
    potionSprite->render();
  }
  else{
    potionSpriteBW->render();
  }
  if (can_buy_espasa){
    swordSprite->render();
  }
  else{
    swordSpriteBW->render();
  }
  if (can_buy_taladro){
    taladroSprite->render();
  }
  else{
    taladroSpriteBW->render();
  }

  if(shipStage==0){
    if (can_buy_stage){
      stage1Sprite->render();
    }
    else{
      stage1SpriteBW->render();
    }
  }
  else if(shipStage==1){
    if (can_buy_stage){
      stage2Sprite->render();
    }
    else{
      stage2SpriteBW->render();
    }

  }
  else {
    if (can_buy_stage){
      stage3Sprite->render();
    }
    else{
      stage3SpriteBW->render();
    }

  }

}

void Player::renderCrosshair()
{
  crosshairSprite->setPosition(getCrosshairPos() - glm::ivec2(5,5));
  crosshairSprite->render();
}

void Player::renderHitEffect()
{
  cout <<"HIIT" << endl;
  hitSprite->render();
}

void Player::setCurrentItem(int n)
{
  currentItem = n;
  currentItemSprite->setPosition(inventoryPos + glm::ivec2(2+45*n,2));
}

void Player::setCurrentItemShop(int n)
{
  currentItemShop = n;
  currentItemShopSprite->setPosition(shopPos + glm::ivec2(2+45*n,2));
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
    position = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}

glm::ivec2 Player::getPos() const
{
    return position;
}

glm::ivec2 Player::getSpeed() const
{
    return glm::ivec2(speed);
}

void Player::attack(int hit_damage)
{
	if (hit_damage == -1) hit_damage = damage;
	Scene* scene = &(Game::instance().scene);
	glm::ivec2 click_pos = scene->screenToWorld(Input::instance().getMouseScreenPos());

    for(int i = 0; i<scene->enemyVector.size(); ++i){
        //glm::vec2 enemy_pos = scene->enemyVector[i]->getPos();
        if (scene->enemyVector[i]->pointInside(click_pos)){
            scene->enemyVector[i]->dealDamage(damage, position);
            break;
        }
    }

    for(int i = 0; i<scene->rockEnemyVector.size(); ++i){
        //glm::vec2 enemy_pos = scene->enemyVector[i]->getPos();
        if (scene->rockEnemyVector[i]->pointInside(click_pos)){
            scene->rockEnemyVector[i]->dealDamage(damage, position);
            break;
        }
    }
}

glm::ivec2 Player::getCrosshairPos() const
{
  glm::vec2 mousePos = Game::instance().scene.screenToWorld(Input::instance().getMouseScreenPos());
  glm::vec2 myPos = glm::vec2(position + glm::ivec2(8,16));
  glm::vec2 chVector = mousePos - myPos;

  if (glm::length(chVector) > 60.0f) {
    chVector = glm::normalize(chVector) * 60.0f;
  }
  return myPos + chVector;
}

template <class T> void Player::giveItem(int param /* = 0 */)
{
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
  std::cout << "Inventory full" << std::endl;
}
