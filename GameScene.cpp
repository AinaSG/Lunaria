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
  initTileShaders();

  gameTime = 0.0f;

  gameOver = false;

  map = TileMap::createTileMap(tileProgram);
  backmap = TileMap::loadTileMap("levels/generatedLevel_bg.txt", map->heightmap, tileProgram);

  Texture *tex = ResourceManager::instance().getTexture("bg.png");
  if (tex == nullptr) {
    std::cout << "Background texture not found" << std::endl;
    return;
  }

  background = Sprite::createSprite(Game::screenSize, glm::vec2(1.0, 1.0), tex, &texProgram);

  tex = ResourceManager::instance().getTexture("game_over.png");
  if (tex == nullptr) {
    std::cout << "Game over texture not found" << std::endl;
    return;
  }

  gameoverSprite = Sprite::createSprite(Game::screenSize, glm::vec2(1.0, 1.0), tex, &texProgram);
  gameoverSprite->setPosition(glm::ivec2(0,0));

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
  glm::ivec2 pPos = player->getPos();
  glm::ivec2 maxClamp = map->getMapSize() * map->getTileSize() - Game::halfScreenSize;
  cameraPos = glm::clamp(player->getPos(), Game::halfScreenSize,maxClamp);
}

void GameScene::update(int deltaTime) {

  gameTime += deltaTime/1000.f;

  if (gameOver) {
    gameOverUpdate();
    return;
  }

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

  glm::ivec2 maxClamp = map->getMapSize() * map->getTileSize() - Game::halfScreenSize;
  glm::ivec2 blockOffset = glm::ivec2(map->getTileSize(), map->getTileSize());
  cameraPos = glm::clamp(player->getPos(), Game::halfScreenSize + blockOffset, maxClamp - blockOffset);
}

void GameScene::render()
{


  glm::mat4 model(1.0f);
  glm::mat4 view = translate(glm::mat4(1.0f),glm::vec3(0.5,0.5,0));

  texProgram.use();
  texProgram.setUniformMatrix4f("projection", projection);
  texProgram.setUniformMatrix4f("model", model);
  texProgram.setUniformMatrix4f("view",view);

  if (gameOver) {
   gameoverSprite->render();
   return;
  }

  view = glm::translate(view, glm::vec3(Game::halfScreenSize,0));
  view = glm::rotate(view,gameTime/300.f,glm::vec3(0,0,1));
  view = glm::scale(view, glm::vec3(1.1,1.1,1));
  view = glm::translate(view,  glm::vec3(-Game::halfScreenSize,0));

texProgram.setUniformMatrix4f("view",view);

  texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
  background->render();

  view = glm::mat4(1.0f);
  texProgram.setUniformMatrix4f("view", view);

  glm::vec3 v = glm::vec3(glm::ivec2(cameraPos) - glm::ivec2(Game::halfScreenSize),0);
  view = glm::translate(glm::mat4(1.0f), - v  + glm::vec3(0.5,0.5,0));

  tileProgram.use();
  tileProgram.setUniformMatrix4f("projection", projection);
  tileProgram.setUniformMatrix4f("model", model);
  tileProgram.setUniformMatrix4f("view",view);;

  tileProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

  tileProgram.setUniform4f("color", 0.5, 0.5f, 0.5f, 0.5f);
  tileProgram.use();
  backmap->render();

  tileProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
  map->render();

  texProgram.use();
  texProgram.setUniformMatrix4f("projection", projection);
  texProgram.setUniformMatrix4f("model", model);
  texProgram.setUniformMatrix4f("view",view);


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

  player->renderCrosshair();

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
  if (block == Block::Ruby && speed < 200) return;

  bool hitBlock = block != Block::Empty;

  if (hitBlock && tilePos != breakingPos) {
    breakPercent = 100;
    breakingPos = tilePos;
  }

  if (hitBlock) {
    breakPercent -= speed*(deltaTime/1000.0f);

    if (breakPercent <= 0) {
      map->setTile(breakingPos,0);

      if (rand()%10 == 0){
        bool spawned = false;
        for (int i = -1; i < 1; ++i) {
          for (int j = -1; j < 1; ++j) {
            bool blocked = false;
            for (int k = 0; k < 2; ++k) {
              for (int l = 0; l < 2; ++l) {

                if (map->getTile(tilePos + glm::ivec2(i,j) + glm::ivec2(k,l)) != Block::Empty) {
                  blocked = true;
                  break;
                }
              }
              if (blocked) break;
            }

            if (!blocked) {
              add_rockEnemy(glm::vec2(tilePos + glm::ivec2(i,j+2)) * (float)map->getTileSize() - glm::vec2(0,10)) ;
              spawned = true;
              break;
            }
          }
          if (spawned) break;
        }
      }


      backmap->prepareArrays();

      player->giveItem<BlockItem>(block);
      breakPercent = 100;
      breakingPos = NULL_POS;
    }
  }
}

void GameScene::initTileShaders() {
  Shader vShader, fShader;

  vShader.initFromFile(VERTEX_SHADER, "shaders/tile.vert");
  if(!vShader.isCompiled())
  {
    cout << "TILE Vertex Shader Error" << endl;
    cout << "" << vShader.log() << endl << endl;
  }
  fShader.initFromFile(FRAGMENT_SHADER, "shaders/tile.frag");
  if(!fShader.isCompiled())
  {
    cout << "TILE Fragment Shader Error" << endl;
    cout << "" << fShader.log() << endl << endl;
  }

  tileProgram.init();
  tileProgram.addShader(vShader);
  tileProgram.addShader(fShader);
  tileProgram.link();
  if(!tileProgram.isLinked())
  {
    cout << "Shader Linking Error" << endl;
    cout << "" << tileProgram.log() << endl << endl;
  }
  tileProgram.bindFragmentOutput("outColor");
  vShader.free();
  fShader.free();
}

glm::ivec2 GameScene::worldToTile(const glm::ivec2 &p) { return p/map->getTileSize(); }
glm::ivec2 GameScene::screenToTile(const glm::ivec2 &p) { return screenToWorld(p)/map->getTileSize(); }

void GameScene::gameOverUpdate()
{
  if (Input::instance().getKeyDown('r')) {
    Game::instance().startGame();
  }
}
