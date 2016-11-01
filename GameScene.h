#ifndef GAMESCENE_H
#define GAMESCENE_H
#include "TileMap.h"
#include "Enemy.h"
#include "RockEnemy.h"
#include "Scene.h"


class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene();

    void init();
    void update(int deltaTime);
    void render();

    void add_rockEnemy(const glm::ivec2 &p);
    void mineBlock(float deltaTime, float speed = 100.0f);

    glm::ivec2 worldToTile(const glm::ivec2 &p);
    glm::ivec2 screenToTile(const glm::ivec2 &p);

    TileMap *map;

    glm::ivec2 breakingPos;
    int breakPercent;

    Player *player;

    vector<Enemy*> enemyVector;
    vector<RockEnemy*> rockEnemyVector;

    void endGame() { gameOver = true; }

private:

    bool gameOver;
    void gameOverUpdate();

    TileMap *backmap;
    Sprite *background, *breakingOverlay[3];
    Sprite *inventory;

    ShaderProgram tileProgram;

    void initTileShaders();

    const glm::ivec2 NULL_POS = glm::ivec2(-1,-1);
};

#endif // GAMESCENE_H
