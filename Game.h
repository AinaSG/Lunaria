#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include <iostream>


#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 720


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() : mouseDown(false) {}


	static Game &instance()
	{
		static Game G;

		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	void mouseHold();

	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	glm::ivec2 getMouseWorldPos();
	glm::ivec2 getMouseScreenPos();

	bool mouseDown;

private:
	bool bPlay;                       // Continue to play game?
	glm::ivec2 mousePos;
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that
	                                  // we can have access at any time
};


#endif // _GAME_INCLUDE
