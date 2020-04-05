#pragma once
#include "Framework/GameObject.h"
#include "Framework/TileMap.h"

//Enum to know with map (stage) we want to create
enum class Maps { TUTORIAL, SCIMAN };

class TileManager
{
private:
	//Main tile map object
	TileMap tileMap;

	//Tile objects
	GameObject tile;						//Will contain a unique tile
	std::vector<GameObject> tiles;			//Will store all the different tiles

	//Map creation variables
	sf::Vector2u mapSize;				//Map dimensions
	std::vector<int> map;				//the tile map

public:
	TileManager();
	~TileManager();
	void render(sf::RenderWindow* window);
	void createMap(Maps mapMode);
};

