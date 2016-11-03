#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Scene.h"
#include "GameScene.h"
#include "MenuScene.h"
#include <iostream>
#include <SFML/Audio.hpp>


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
    return dynamic_cast<GameScene*>(instance().scene);
  }

  void init();
  bool update(int deltaTime);
  void render();

  void startGame();
  void gotoMenu();
  void gotoHelp();
  void gotoCredits();
  void gotoIntro();
  void gotoWin();

  void playPlayerHit();
  void playRockHit();
  void playBubHit();

  static const glm::ivec2 screenSize;
  static const glm::ivec2 halfScreenSize;
  Scene* scene;                      // Scene to render	                                  /
  // we can have access at any time

  Text text, boldText;

  sf::Music bg_music;

  vector<sf::SoundBuffer> player_hit_sound_buffer; // (2);
  vector<sf::SoundBuffer> bub_hit_sound_buffer; // (5);
  vector<sf::SoundBuffer> rock_hit_sound_buffer; // (3);

  sf::Sound player_hit_sound;
  sf::Sound bub_hit_sound;
  sf::Sound rock_hit_sound;

private:
  bool bPlay;                       // Continue to play game?
  Scene* nextScene;
};


#endif // _GAME_INCLUDE
