#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Scene.h"
#include "GameScene.h"
#include <iostream>

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 720


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{
public:

  Game() {}

	static Game &instance()
	{
		static Game G;
		return G;
	}

    static GameScene* gameScene() {
      return static_cast<GameScene*>(instance().scene);
    }

	void init();
	bool update(int deltaTime);
	void render();

    static const glm::ivec2 screenSize;
    static const glm::ivec2 halfScreenSize;
    Scene* scene;                      // Scene to render	                                  /
    // we can have access at any time

    Text text, boldText;
private:
	bool bPlay;                       // Continue to play game?
	};


#endif // _GAME_INCLUDE
