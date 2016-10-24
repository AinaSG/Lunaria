#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
    void render();
    glm::ivec2 getCameraPos() const { return cameraPos; }

    glm::ivec2 worldToScreen(const glm::ivec2 &p);
    glm::ivec2 screenToWorld(const glm::ivec2 &p);

    glm::ivec2 worldToTile(const glm::ivec2 &p);
    glm::ivec2 screenToTile(const glm::ivec2 &p);

	TileMap *map;

    glm::ivec2 breakingPos;
    int breakPercent;
    const glm::ivec2 NULL_POS = glm::ivec2(-1,-1);

    void mineBlock(float deltaTime, float speed = 100.0f);

private:
	void initShaders();

    TileMap *backmap;
	Player *player;

    Sprite *background, *breakingOverlay[3];

	Sprite *itemHolder;
	
	float currentTime;
	
    ShaderProgram texProgram;
	glm::mat4 projection;
	glm::ivec2 cameraPos;
};


#endif // _SCENE_INCLUDE
