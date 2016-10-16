#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Simplex.h"
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

enum Block {
  Empty         = 0,
  Rock          = 1,
  BlueMaterial  = 2,
  RedMaterial   = 3
};

class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *loadTileMap(const string &levelFile, ShaderProgram &program);
	static TileMap *createTileMap(ShaderProgram &program);
	static TileMap *createBackground(const TileMap* orig, ShaderProgram &program);


	TileMap(const string &levelFile, ShaderProgram &program);
	TileMap(ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }
	glm::ivec2 getMapSize() const { return mapSize; }
	int *getMap() const { return map; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size) const;

	bool clampMoveX(glm::ivec2 &pos, const glm::ivec2 &size, int delta) const;
	bool clampMoveY(glm::ivec2 &pos, const glm::ivec2 &size, int delta) const;

	int *tile(glm::ivec2 coords) { return &map[coords.x+coords.y*mapSize.x]; }
	int *tile(int x, int y) { return &map[x+y*mapSize.x]; }

	int getTile(glm::ivec2 coords);
	int getTile(int x, int y);

	int setTile(glm::ivec2 coords, int tile);
	int setTile(int x, int y, int tile);

	void removeTile(glm::ivec2 coords);
	void removeTile(int x, int y);

private:
	bool loadLevel(const string &levelFile);
	void prepareArrays();
	string generateLevel();
	string improvedLevelGenerator();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	ShaderProgram program;


};


#endif // _TILE_MAP_INCLUDE
