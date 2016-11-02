#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "RockEnemy.h"
#include <vector>
#include "Text.h"
#include <SFML/Audio.hpp>

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{
public:
    Scene() {}
    virtual ~Scene() {}

    virtual void init();
    virtual void update(int deltaTime) = 0;
    virtual void render() = 0;

    glm::ivec2 getCameraPos() const { return cameraPos; }

	glm::ivec2 worldToScreen(const glm::ivec2 &p);
	glm::ivec2 screenToWorld(const glm::ivec2 &p);


  

protected:
	void initShaders();
    float currentTime;

    ShaderProgram texProgram;
    glm::mat4 projection;
    glm::ivec2 cameraPos;

};


#endif // _SCENE_INCLUDE
