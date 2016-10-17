#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Input.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <sstream>
#include "ResourceManager.h"

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 10

Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	
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

    tex = ResourceManager::instance().getTexture("inventory.png");
    if (tex == nullptr) {
      std::cout << "Invere texture not found" << std::endl;
      return;
    }
    inventory = Sprite::createSprite(glm::ivec2(412,52), glm::vec2(1.0, 1.0), tex, &texProgram);

	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	currentTime = 0.0f;

    breakingPos = NULL_POS;
    breakPercent = 100;

	cameraPos = player->getPos();
}

void Scene::update(int deltaTime)
{
    currentTime += deltaTime;

    Input *input = &Input::instance();
    if (input->getMouseButton(GLUT_LEFT_BUTTON)) {
      glm::ivec2 tilePos = screenToTile(input->getMouseScreenPos());

      if (tilePos != breakingPos) {
        breakPercent = 100;
        breakingPos = (map->getTile(tilePos) != 0) ? tilePos : NULL_POS;
      }

      if (breakingPos != NULL_POS) {
        breakPercent -= 60.0*(deltaTime/1000.0f);

        if (breakPercent <= 0) {
           map->setTile(breakingPos,0);
           breakPercent = 100;
           breakingPos = NULL_POS;
        }
      }
    }

	player->update(deltaTime);

	glm::vec2 playerPos = glm::vec2(player->getPos());
	cameraPos = playerPos;
}

void Scene::render()
{
	glm::mat4 model(1.0f);
	glm::mat4 view(1.0f);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);


	texProgram.setUniformMatrix4f("model", model);
	texProgram.setUniformMatrix4f("view",view);

	background->render();

	view = glm::translate(glm::mat4(1.0f), - glm::vec3(glm::vec2(cameraPos) - glm::vec2(SCREEN_WIDTH/2,SCREEN_HEIGHT/2) , 0));
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
	

	model = glm::translate(glm::mat4(1.0), glm::vec3(10,10,10));
	view = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("model", model);
	texProgram.setUniformMatrix4f("view",view);

	inventory->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

glm::ivec2 Scene::worldToScreen(const glm::ivec2 &p) { return p - (cameraPos - Game::halfScreenSize); }
glm::ivec2 Scene::screenToWorld(const glm::ivec2 &p) { return p + (cameraPos - Game::halfScreenSize); }

glm::ivec2 Scene::worldToTile(const glm::ivec2 &p) { return p/map->getTileSize(); }
glm::ivec2 Scene::screenToTile(const glm::ivec2 &p) { return screenToWorld(p)/map->getTileSize(); }
