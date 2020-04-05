#include "TileManager.h"
TileManager::TileManager()
{

	//Load the tile sheet
	tileMap.loadTexture("custom_sprites/NES_Mega_Man_Tiles.PNG");

	for (int i = 0; i <= 34; i++)
	{
		tile.setSize(sf::Vector2f(50, 50));		//We will be able to draw 13.5 (14) tiles vertically and 24 tiles horizontally
		tile.setCollisionBox(0, 0, 50, 50);
		//all normal non colliding tiles init
		if (i < 22)
		{
			tile.setCollider(false);
			tile.setTargetName("world");
		}
		//all normal colliding tiles init
		else if (i >= 22 && i < 31)
		{
			tile.setCollider(true);
			tile.setTargetName("world");
		}
		//all special tiles init
		else if (i >= 32)
		{
			tile.setCollider(true);	//We still want to check their collisions
			//We have to initialise their targetname manually since they may all be different
		}
		tiles.push_back(tile);
	}
	//Non Colliding background tiles
	tiles[0].setTextureRect(sf::IntRect(275, 189, 16, 16));				//Sky
	tiles[1].setTextureRect(sf::IntRect(275, 172, 16, 16));				//Grey background tile with left part of thing in the middle
	tiles[2].setTextureRect(sf::IntRect(292, 172, 16, 16));				//Grey background tile with middle part of thing in the middle
	tiles[3].setTextureRect(sf::IntRect(309, 172, 16, 16));				//Grey background tile with right part of thing in the middle
	tiles[4].setTextureRect(sf::IntRect(377, 172, 16, 16));				//Grey background square top
	tiles[5].setTextureRect(sf::IntRect(394, 172, 16, 16));				//Grey background square
	tiles[6].setTextureRect(sf::IntRect(326, 189, 16, 16));				//Grey background top
	tiles[7].setTextureRect(sf::IntRect(343, 189, 16, 16));				//Grey background
	tiles[8].setTextureRect(sf::IntRect(377, 189, 16, 16));				//Grey background window top part
	tiles[9].setTextureRect(sf::IntRect(394, 189, 16, 16));				//Grey background window bottom part

	//Non colliding Spawner tiles (tiles used to build the small building that spawns ennemies)
	tiles[10].setTextureRect(sf::IntRect(309, 206, 16, 16));			//Grey ladder tile (not a real ladder)
	tiles[11].setTextureRect(sf::IntRect(326, 206, 16, 16));			//Grey spawner left wall
	tiles[12].setTextureRect(sf::IntRect(343, 206, 16, 16));			//Grey spawner middle left
	tiles[13].setTextureRect(sf::IntRect(360, 206, 16, 16));			//Grey spawner middle right
	tiles[14].setTextureRect(sf::IntRect(377, 206, 16, 16));			//Grey spawner right wall
	tiles[15].setTextureRect(sf::IntRect(275, 223, 16, 16));			//Grey spawner left spawn area
	tiles[16].setTextureRect(sf::IntRect(292, 223, 16, 16));			//Grey spawner right spawn area
	tiles[17].setTextureRect(sf::IntRect(309, 223, 16, 16));			//Grey spawner left part of spawner deco thing
	tiles[18].setTextureRect(sf::IntRect(326, 223, 16, 16));			//Grey spawner right part of spawner deco thing
	tiles[19].setTextureRect(sf::IntRect(343, 223, 16, 16));			//Grey spawner top left
	tiles[20].setTextureRect(sf::IntRect(360, 223, 16, 16));			//Grey spawner top middle
	tiles[21].setTextureRect(sf::IntRect(377, 223, 16, 16));			//Grey spawner top right

	//Colliding tiles
	tiles[22].setTextureRect(sf::IntRect(326, 172, 16, 16));			//Secondary wall left part
	tiles[23].setTextureRect(sf::IntRect(343, 172, 16, 16));			//Secondary wall right part
	tiles[24].setTextureRect(sf::IntRect(360, 189, 16, 16));			//Primary wall
	tiles[25].setTextureRect(sf::IntRect(411, 189, 16, 16));			//Boss entrance wall vertical part
	tiles[26].setTextureRect(sf::IntRect(275, 206, 16, 16));			//Boss entrance wall left part
	tiles[27].setTextureRect(sf::IntRect(292, 206, 16, 16));			//Boss entrance wall right part
	tiles[28].setTextureRect(sf::IntRect(394, 206, 16, 16));			//Cube top left
	tiles[29].setTextureRect(sf::IntRect(411, 206, 16, 16));			//Cube top right
	tiles[30].setTextureRect(sf::IntRect(394, 223, 16, 16));			//Cube bottom left
	tiles[31].setTextureRect(sf::IntRect(411, 223, 16, 16));			//Cube bottom right

	//Special tiles
	tiles[32].setTextureRect(sf::IntRect(292, 189, 16, 16));			//Ladder
	tiles[32].setTargetName("ladder");
	tiles[33].setTextureRect(sf::IntRect(309, 189, 16, 16));			//Spike
	tiles[33].setTargetName("spike");
	tiles[34].setTextureRect(sf::IntRect(360, 172, 16, 16));			//Door
	tiles[34].setTargetName("door");

	//Set the tileMap
	tileMap.setTileSet(tiles);
}

TileManager::~TileManager()
{

}

void TileManager::update(float dt, Player& p)
{
	bool hasCollided = false;
	std::vector<GameObject>* world = tileMap.getLevel();
	for (unsigned i = 0; i < (int)world->size(); ++i)
	{
		if ((*world)[i].isCollider())
		{
			if (Collision::checkBoundingBox(&p, &(*world)[i]))
			{
				p.collisionResponse(&(*world)[i]);
				hasCollided = true;
			}
		}
	}
	if (!hasCollided) p.setStates(false, false, false);	//Need this line to re-enable controls and physics in air
}

void TileManager::render(sf::RenderWindow* window)
{
	tileMap.render(window);

	//Debug tiles
	std::vector<GameObject>* world = tileMap.getLevel();
	for (unsigned i = 0; i < (int)world->size(); ++i)
	{
		if (debugUi->isDebugging())
		{
			(*world)[i].updateDebugBoxes();
			window->draw(*(*world)[i].getDebugCollisionBox());
		}
	}
}

void TileManager::createMap(Maps mapMode)
{
	switch (mapMode)
	{
	case Maps::TUTORIAL:
		// Map dimensions
		mapSize = sf::Vector2u(40, 14);
		map = {
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,	25,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,	25,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,	25,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,	25,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,	25,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	32,	24,	24,	24,	24,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,	25,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	6,	4,	6,	6,	4,	6,	32,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,	25,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	7,	7,	1,	2,	2,	3,	32,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,	34,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	6,	6,	6,	6,	7,	7,	7,	7,	7,	7,	32,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,	34,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	8,	8,	8,	8,	7,	7,	5,	7,	7,	7,	32,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,	34,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	9,	9,	9,	9,	7,	5,	24,	24,	24,	24,	24,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	24,	24,	24,	24,	24,	24,	24,	24,	24,
			0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	7,	7,	5,	7,	7,	7,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	23,	22,	23,	22,	23,	22,	23,	22,	23,
			24,	24,	24,	24, 24, 24, 24, 24, 24, 24,	24,	24,	24,	24,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	33,	33,	33,	33,	33,	23,	22,	23,	22,	23,	22,	23,	22,	23,
			22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23
		};
		break;
	case Maps::SCIMAN:

		break;
	}

	//Once the map has been created/declared, set it
	tileMap.setTileMap(map, mapSize);
	tileMap.setPosition(sf::Vector2f(0, 0));
	tileMap.buildLevel();
}