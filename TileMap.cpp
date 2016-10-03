#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include "TileMap.h"


using namespace std;

#define MAP_X 320
#define MAP_Y 58
#define TILESIZE 16
#define BLOCKSIZE 32

#define TILESHEET "images/blocks_moon.png"
#define TILESHEET_X 2
#define TILESHEET_Y 2

#define MAP_FREQ_RESOLUTION 30
#define MAP_AMPL_CONSTANT 20
#define MAP_STRETCH 120

//han de sumar 10, com mes petit, mes espai ocupa
#define SKY_PERTEN 4
#define GRO_PERTEN 4
#define LINE_PERTEN 2

#define PERCENT_NORMAL_BLOCK 95
#define PERCENT_NORMAL_AND_RARE_BLOCK 95


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}

TileMap *TileMap::newTileMap(const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap( minCoords, program);
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	//loadLevel(generateLevel());
	prepareArrays(minCoords, program);
}

TileMap::TileMap(const glm::vec2 &minCoords, ShaderProgram &program)
{
	//loadLevel(levelFile);
	loadLevel(generateLevel());
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}

string TileMap::generateLevel(){

	std::ofstream outfile( "levels/generatedLevel.txt");

	vector< vector<string> > levelmap(MAP_Y, vector<string>(MAP_X));
	for (int i=0; i<MAP_Y; ++i){
		for (int j=0; j<MAP_X; ++j){
			levelmap[i][j] = " ";
		}
	}

 	// GENERADOR DE LA LINIA DEL TERRA /////////////////////////////////
	// BASED ON: http://gamedev.stackexchange.com/a/20829 //////////////

	vector<double> frequences(MAP_FREQ_RESOLUTION);
	vector<double> amplitudes(MAP_FREQ_RESOLUTION);
	vector<double> ofsets(MAP_FREQ_RESOLUTION);

	srand( unsigned(time(NULL)));
	double u;

	std::mt19937 rng;
	rng.seed(std::random_device{}());

	for(int f = 0; f< MAP_FREQ_RESOLUTION; ++f){

		std::uniform_real_distribution<double> dist(1, 10);
		u = dist(rng);
		//u = floor(u * 100.0) / 100.0;
		frequences[f] = u;
		cout << "Frequence: " << frequences[f] << endl;
		double amplSide1 = (1-MAP_AMPL_CONSTANT) / u;
		double amplSide2 = MAP_AMPL_CONSTANT / u;
		std::uniform_real_distribution<double> dist2(amplSide1, amplSide2);
		u = dist2(rng);
		//u = floor(u * 1000.0) / 1000.0;
		amplitudes[f] = u;
		cout << "Amplitude: " << amplitudes[f] << endl;
		std::uniform_real_distribution<double> dist3(0, 6.28);
		u = dist3(rng);
		ofsets[f] = u;
		cout << "Ofset: " << ofsets[f] << endl << endl;
	}

	// EN AQUEST PUNT JA TENIM LA NOSTRA FUNCIÓ

	vector<double> map_ground (MAP_X);
	for (int k = 0; k < MAP_X; ++k){
		//En cada "columna" del mapa
		double wave_p = 0;
		//Calculem el valor de la funcio
		for (int f = 0; f < MAP_FREQ_RESOLUTION; ++f){
			wave_p = wave_p + (amplitudes[f]*sin(frequences[f]*(MAP_STRETCH*k+ofsets[f])));
		}
		cout << wave_p << endl;
		map_ground[k] = wave_p;
	}

	double map_ground_min = MAP_AMPL_CONSTANT;
	double map_ground_max = -MAP_AMPL_CONSTANT;

	for(int i = 0; i < MAP_X; ++i){
		if (map_ground[i] > map_ground_max) map_ground_max = map_ground[i];
		if (map_ground[i] < map_ground_min) map_ground_min = map_ground[i];
	}

	cout << "Max: " << map_ground_max << endl;
	cout << "Min: " << map_ground_min << endl;

	//Ara tenim la linia del terra i podem decidir on posar cada cosa
	//Jo faria: 1/4 del mapa de cel
	//					1/2 mapa linia del terra
	//          1/4 mapa sota terra

	int blocks_sky = MAP_Y/SKY_PERTEN;
	int blocks_ground = MAP_Y/GRO_PERTEN;
	int blocks_line = MAP_Y/LINE_PERTEN;

	//Asseguremnos que en total no se passa del mapa

	while (blocks_line+blocks_sky+blocks_ground > MAP_Y){
		--blocks_sky;
		cout << "oops" << endl;
	}

	while (blocks_line+blocks_sky+blocks_ground < MAP_Y){
		++blocks_ground;
		cout << "oopsies" << endl;
	}

	//Per tant entre les files 0 i blocks_sky hi ha cel
	//Entre blocks_sky+1 i blocks_sky+blocks_ground+1 hi ha ground
	//I Entre blocks_sky+blocks_ground+1 i MAP_Y hi ha terra

	cout << "Sky from: " << 0 << " to: " << blocks_sky << endl;
	cout << "Line from: " << blocks_sky+1 << " to: " << blocks_sky+blocks_ground+1 << endl;
	cout << "Ground from: " << blocks_sky+blocks_ground+1 << " to: " << MAP_Y << endl;

	int max_ground_real = blocks_sky+blocks_ground+1;
	int min_ground_real = blocks_sky+1;

	for (int i = 0; i < MAP_X; ++i){
		map_ground[i] = ((map_ground[i]-map_ground_min)/(map_ground_max-map_ground_min))*(max_ground_real-min_ground_real)+min_ground_real;
		cout << map_ground[i] << endl;
	}

for (int i= 1; i< MAP_X-1; ++i){
	map_ground[i] = map_ground[i-1]*(1-((1-cos(map_ground[i]*3.14))/2))+map_ground[i+1]*((1-cos(map_ground[i]*3.14))/2);
}

for (int i= 1; i< MAP_X-1; ++i){
	map_ground[i] = map_ground[i-1]*(1-((1-cos(map_ground[i]*3.14))/2))+map_ground[i+1]*((1-cos(map_ground[i]*3.14))/2);
}

for (int i= 1; i< MAP_X-1; ++i){
	map_ground[i] = map_ground[i-1]*(1-((1-cos(map_ground[i]*3.14))/2))+map_ground[i+1]*((1-cos(map_ground[i]*3.14))/2);
}
/*
for (int i= 1; i< MAP_X-1; ++i){
	map_ground[i] = (map_ground[i-1]+map_ground[i]+map_ground[i+1])/3;
}*/

	std::uniform_real_distribution<double> distmaterials(0, 100);
	for (int i=0; i<MAP_Y; ++i){
		for (int j=0; j<MAP_X; ++j){
			if(i >= map_ground[j]) levelmap[i][j] = "1";
			//cout << levelmap[i][j];
			//Populate rare materials

			if((i >= map_ground[j]+2)){
				//Candidate for rare materials
				if(distmaterials(rng)>PERCENT_NORMAL_BLOCK){
					//Es rare!
					levelmap[i][j] = "2";
					if(distmaterials(rng)>PERCENT_NORMAL_AND_RARE_BLOCK){
						//Es molt rare!
						levelmap[i][j] = "3";
					}
				}
			}
		}
		//cout << endl;
	}

	//ESCRIBIM EL MAPA GENERAT AL FITXER
	outfile << "TILEMAP" << "\r\n";
	outfile << MAP_X << " " << MAP_Y << "\r\n";
	outfile << TILESIZE << " " << BLOCKSIZE << "\r\n";
	outfile << TILESHEET << "\r\n";
	outfile << TILESHEET_X << " " << TILESHEET_Y << "\r\n";
	for (int i=0; i < MAP_Y; ++i){
		for(int j=0; j < MAP_X; ++j){
			outfile << levelmap[i][j];
		}
		outfile << "\r\n";
	}
	outfile.close();
	return ( "levels/generatedLevel.txt");
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else
				map[j*mapSize.x+i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%2) / tilesheetSize.x, float((tile-1)/2) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.


// [SOLID]   <-----MOVIMENT
// [SOLID]    x--> [P] <--Y0
// [SOLID]         [P] <--Y1

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = pos.x / tileSize; //sabem a sobre de quina coordenada de tile estem
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;  //hem agafat la tile maxima i minima en y en la que esta el player
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}

	return false;
}


// MOVIMENT ---->    [SOLID]
// y0--> [P] <--x    [SOLID]
// y1--> [P]         [SOLID]

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}

	return false;
}



bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x+x] != 0)
		{
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x+x] != 0)
		{
			return true;
		}
	}

	return false;
}

bool TileMap::clampMoveX(glm::ivec2 &pos, const glm::ivec2 &size, int delta) const
{
	pos.x += delta;

	//Chack for hit left or right depending on delta direction
	bool hit = (delta > 0) ? collisionMoveRight(pos, size) : collisionMoveLeft(pos, size);

	//If it didn't hit anything, we are done
	if (!hit)  return false;

	pos.x -= delta; //(pos.x + tileSize  * (delta < 0)) / tileSize;
	return true;
}

bool TileMap::clampMoveY(glm::ivec2 &pos, const glm::ivec2 &size, int delta) const
{
	pos.y += delta;

	//Chack for hit left or right depending on delta direction
	bool hit = (delta > 0) ? collisionMoveDown(pos, size) : collisionMoveUp(pos, size);

	//If it didn't hit anything, we are done
	if (!hit)  return false;

	pos.y -= delta; //(pos.y + tileSize  * (delta < 0)) / tileSize;
	return true;
}
