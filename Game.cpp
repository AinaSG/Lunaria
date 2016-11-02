#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "InfoScene.h"
#include "IntroScene.h"
#include "Input.h"
#include <string>

#include <SFML/Audio.hpp>


const glm::ivec2 Game::screenSize = glm::ivec2(SCREEN_WIDTH,SCREEN_HEIGHT);
const glm::ivec2 Game::halfScreenSize = Game::screenSize/2;

void Game::init()
{
	bPlay = true;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    nextScene = nullptr;
    scene = new MenuScene();
    scene->init();

    if(!text.init("fonts/OpenSans-Regular.ttf"))
        cout << "Could not load font!!!" << endl;

     if(!boldText.init("fonts/OpenSans-ExtraBold.ttf"))
        cout << "Could not load bold font!!!" << endl;

		if (!bg_music.openFromFile("sound/MachinariumSoundtrack06.ogg"))
							std::cout << "FAILED TO LOAD MUSIC" << std::endl;


		player_hit_sound_buffer = vector<sf::SoundBuffer> (2);
		bub_hit_sound_buffer = vector<sf::SoundBuffer> (5);
		rock_hit_sound_buffer = vector<sf::SoundBuffer> (3);


		for (int i = 1; i <= 5; ++i){
			string filen = "sound/bub/" + to_string(i) + ".wav";
			if (!bub_hit_sound_buffer[i-1].loadFromFile(filen)){
				std::cout << "FAILED TO LOAD BUBSOUND" << std::endl;;
			}
		}
		for (int i = 1; i <= 2; ++i){
			string filen = "sound/player/" + to_string(i) + ".wav";
			if (!player_hit_sound_buffer[i-1].loadFromFile(filen)){
				std::cout << "FAILED TO LOAD HITSOUND" << std::endl;;
			}
		}
		for (int i = 1; i <= 3; ++i){
			string filen = "sound/rockbub/" + to_string(i) + ".ogg";
			if (!rock_hit_sound_buffer[i-1].loadFromFile(filen)){
				std::cout << "FAILED TO LOAD ROCKSOUND" << std::endl;;
			}
		}

		bg_music.play();
}

bool Game::update(int deltaTime)
{
    scene->update(deltaTime);
    Input::instance().update();
    if (nextScene != nullptr) {
      delete scene;
      scene = nextScene;
      scene->init();
      nextScene = nullptr;
    }
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->render();
}

void Game::gotoIntro() {
  nextScene = new IntroScene();
}

void Game::startGame()
{
  nextScene = new GameScene();
}

void Game::gotoMenu()
{
  nextScene = new MenuScene();
}

void Game::gotoHelp()
{
  nextScene = new InfoScene("help.png");
}

void Game::gotoCredits()
{
  nextScene = new InfoScene("credits.png");
}

void Game::playPlayerHit(){
	int playthis = rand() % 2;
	player_hit_sound.setBuffer(player_hit_sound_buffer[playthis]);
	player_hit_sound.play();
}
void Game::playRockHit(){
	int playthis = rand() % 3;
	rock_hit_sound.setBuffer(rock_hit_sound_buffer[playthis]);
	rock_hit_sound.play();

}
void Game::playBubHit(){
	int playthis = rand() % 5;
	bub_hit_sound.setBuffer(bub_hit_sound_buffer[playthis]);
	bub_hit_sound.setVolume(100);
	bub_hit_sound.play();
}
