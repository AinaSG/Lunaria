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
	glm::ivec2 getCameraPos() { return cameraPos; };

	TileMap *map;

private:
	void initShaders();

private:
	
	TileMap  *backmap;
	Player *player;

	Sprite *background;
	Texture backgroundImage;

	Sprite *inventory;
	Texture inventoryImage;

	Sprite *itemHolder;
	
	float currentTime;
	
	ShaderProgram texProgram;
	glm::mat4 projection;
	glm::ivec2 cameraPos;
};


#endif // _SCENE_INCLUDE
