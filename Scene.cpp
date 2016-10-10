#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

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
	//map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
  	map = TileMap::createTileMap(texProgram);
	backmap = TileMap::loadTileMap("levels/generatedLevel_bg.txt", texProgram);

	backgroundImage.loadFromFile("images/bg.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH,SCREEN_HEIGHT), glm::vec2(1.0, 1.0), &backgroundImage, &texProgram);


	player = new Player();
	player->init(glm::ivec2(0, 0), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	currentTime = 0.0f;

	cameraPos = player->getPos();
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	glm::vec2 playerPos = glm::vec2(player->getPos());
	glm::vec2 playerSpeed = glm::vec2(player->getSpeed());

	glm::vec2 cameraSpeed = playerSpeed;
	//if (cameraSpeed.x > 0 && cameraPos.x < playerPos.x + SCREEN_WIDTH/30) cameraSpeed.x += 2;
	//if (cameraSpeed.x < 0 && cameraPos.x > playerPos.x - SCREEN_WIDTH/30) cameraSpeed.x -= 2;
	if((cameraPos.x < playerPos.x) && ((playerPos.x - cameraPos.x) < (SCREEN_WIDTH/15))){
		cameraSpeed.x = 0;
	}
	else if((cameraPos.x > playerPos.x) && ((cameraPos.x - playerPos.x) < (SCREEN_WIDTH/15))){
		cameraSpeed.x = 0;
	}
	else{
		cameraSpeed.x = playerSpeed.x;
	}
	if((cameraPos.y < playerPos.y) && (playerPos.y - cameraPos.y < (SCREEN_HEIGHT/15))){
		cameraSpeed.y = 0;
	}
	else if((cameraPos.y > playerPos.y) && (cameraPos.y - playerPos.y < (SCREEN_HEIGHT/15))){
		cameraSpeed.y = 0;
	}
	else{
		cameraSpeed.y = playerSpeed.y;
	}
	//glm::normalize(glm::vec2(playerPos - cameraPos)) * glm::clamp(glm::distance(playerPos, cameraPos) - 200 , 0.0f, 40.0f);
   if (glm::length(cameraSpeed) > .01f){
		cameraPos.x += cameraSpeed.x;
		cameraPos.y += cameraSpeed.y;
	}
}

void Scene::render()
{
	glm::mat4 model;
	glm::mat4 view;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);

	texProgram.setUniformMatrix4f("model", model);

	view = glm::mat4(1.0);
	texProgram.setUniformMatrix4f("view",view);
	background->render();

	view = glm::translate(glm::mat4(1.0f), - glm::vec3(glm::vec2(cameraPos) - glm::vec2(SCREEN_WIDTH/2,SCREEN_HEIGHT/2) , 0));
	texProgram.setUniformMatrix4f("view", view);

	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	texProgram.setUniform4f("color", 0.5, 0.5f, 0.5f, 0.5f);
	backmap->render();


	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	map->render();

	player->render();
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
