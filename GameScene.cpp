#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Input.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <sstream>
#include "BlockItem.h"
#include "ResourceManager.h"
#include <algorithm>

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 10

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
    if (map != nullptr)     delete map;
    if (backmap != nullptr) delete backmap;
    if (player != nullptr)  delete player;

    //Sprites ( memory leaks (?) )
    if (background != nullptr) delete background;
    if (breakingOverlay[0] != nullptr) delete breakingOverlay[0];
    if (breakingOverlay[1] != nullptr) delete breakingOverlay[1];
    if (breakingOverlay[2] != nullptr) delete breakingOverlay[2];

    for (int i = 0; i < enemyVector.size(); ++i) {
        if (enemyVector[i] != nullptr) delete enemyVector[i];
    }
    for (int i = 0; i < rockEnemyVector.size(); ++i) {
        if (rockEnemyVector[i] != nullptr) delete rockEnemyVector[i];
    }
}

void GameScene::init() {
    Scene::init();

    map = TileMap::createTileMap(texProgram);
    backmap = TileMap::loadTileMap("levels/generatedLevel_bg.txt", texProgram);

    Texture *tex = ResourceManager::instance().getTexture("bg.png");
    if (tex == nullptr) {
      std::cout << "Background texture not found" << std::endl;
      return;
    }

    background = Sprite::createSprite(Game::screenSize, glm::vec2(1.0, 1.0), tex, &texProgram);

    for (int i = 0; i < 3; ++i){
      std::ostringstream stream;
      stream << "breaking" << i << ".png";
      tex = ResourceManager::instance().getTexture(stream.str());
      if (tex == nullptr) {
        std::cout << "Breaking texture "  << i << " not found" << std::endl;
        return;
      }

      breakingOverlay[i] = Sprite::createSprite(glm::ivec2(16,16), glm::vec2(1.0, 1.0), tex, &texProgram);
    }

    player = new Player();
    player->init(glm::ivec2(0, 0), texProgram);
    player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
    player->setTileMap(map);

    for (int i = 0; i< map->getMapSize().y; ++i){
      for (int j = 0; j<  map->getMapSize().x; ++j){
        if(map->getTile(j,i) == 5){
          //Creem un enemic aqui

          Enemy *newEnemy = new Enemy();
          newEnemy->init(glm::ivec2(0, 0), texProgram);
          newEnemy->setPosition(glm::vec2(j * map->getTileSize(), i * map->getTileSize()));
          newEnemy->setTileMap(map);

          //Afegim l'enemic a la llista d'enemics
          enemyVector.push_back(newEnemy);

          //Posem aire al tilemap a on l'spawn
          map->setTile(j, i, 0);
        }
      }
    }

    breakingPos = NULL_POS;
    breakPercent = 100;

    cameraPos = player->getPos();
}

void GameScene::update(int deltaTime) {
    player->update(deltaTime);

    for (int i = 0; i< enemyVector.size(); ++i){
      enemyVector[i]->update(deltaTime);
    }

    enemyVector.erase(std::remove_if(
      enemyVector.begin(), enemyVector.end(),
      [](const Enemy* enemy) {
          return enemy->isDead(); // put your condition here
      }), enemyVector.end());


    for (int i = 0; i< rockEnemyVector.size(); ++i){
      rockEnemyVector[i]->update(deltaTime);
    }

    rockEnemyVector.erase(std::remove_if(
      rockEnemyVector.begin(), rockEnemyVector.end(),
      [](const RockEnemy* rockenemy) {
          return rockenemy->isDead(); // put your condition here
      }), rockEnemyVector.end());

    cameraPos = glm::vec2(player->getPos());
}

void GameScene::render()
{
  glm::mat4 model(1.0f);
  glm::mat4 view(1.0f);

  texProgram.use();
  texProgram.setUniformMatrix4f("projection", projection);


  texProgram.setUniformMatrix4f("model", model);
  texProgram.setUniformMatrix4f("view",view);

  background->render();

  view = glm::translate(glm::mat4(1.0f), - glm::vec3(glm::vec2(cameraPos) - glm::vec2(Game::halfScreenSize) , 0));
  texProgram.setUniformMatrix4f("view", view);

  texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

  texProgram.setUniform4f("color", 0.5, 0.5f, 0.5f, 0.5f);
  backmap->render();


  texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
  map->render();

  Sprite *b = nullptr;
  if (breakingPos != NULL_POS) {
    if (breakPercent < 25) b = breakingOverlay[0];
    else if (breakPercent < 60) b = breakingOverlay[1];
    else if (breakPercent < 95) b = breakingOverlay[2];
  }

  if (b != nullptr) {
    b->setPosition(breakingPos*map->getTileSize());
    b->render();
  }

  player->render();
  for (int i = 0; i< enemyVector.size(); ++i){
    enemyVector[i]->render();
  }

  for (int i = 0; i< rockEnemyVector.size(); ++i){
    rockEnemyVector[i]->render();
  }
  //testEnemy->render();

 player->renderCrosshair();

  model = glm::translate(glm::mat4(1.0), glm::vec3(10,10,10));
  view = glm::mat4(1.0f);
  texProgram.setUniformMatrix4f("model", model);
  texProgram.setUniformMatrix4f("view",view);


  player->renderHUD();
}


void GameScene::add_rockEnemy(const glm::ivec2 &p){
  RockEnemy *newEnemy = new RockEnemy();
  newEnemy->init(glm::ivec2(0, 0), texProgram);
  glm::ivec2 spawnPos = p;
  spawnPos.y = spawnPos.y-30;
  newEnemy->setPosition(spawnPos);
  newEnemy->setTileMap(map);

  //Afegim l'enemic a la llista d'enemics
  rockEnemyVector.push_back(newEnemy);
}

void GameScene::mineBlock(float deltaTime, float speed /* = 100.0f */)
{
  glm::ivec2 mousePos = Input::instance().getMouseScreenPos();
  glm::ivec2 tilePos = screenToTile(mousePos);

  int block = map->getTile(tilePos);
  if (block == Block::BedRock) return;

  bool hitBlock = block != Block::Empty;

  if (hitBlock && tilePos != breakingPos) {
    breakPercent = 100;
    breakingPos = tilePos;
  }

  if (hitBlock) {
    breakPercent -= speed*(deltaTime/1000.0f);

    if (breakPercent <= 0) {

      if (rand()%10 == 1){
          add_rockEnemy(screenToWorld(mousePos));
      }

      map->setTile(breakingPos,0);
      player->giveItem<BlockItem>(block);
      breakPercent = 100;
      breakingPos = NULL_POS;
    }
  }
}

glm::ivec2 GameScene::worldToTile(const glm::ivec2 &p) { return p/map->getTileSize(); }
glm::ivec2 GameScene::screenToTile(const glm::ivec2 &p) { return screenToWorld(p)/map->getTileSize(); }
