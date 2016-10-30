#include "MenuScene.h"
#include "Game.h"

MenuScene::MenuScene()
{

}

MenuScene::~MenuScene()
{

}

void MenuScene::init()
{
  Scene::init();
}

void MenuScene::update(int deltaTime)
{
 Game::instance().startGame();
}

void MenuScene::render()
{

}
