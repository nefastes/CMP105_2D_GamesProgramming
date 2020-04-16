#include "TileManager.h"
TileManager::TileManager()
{
	//We start with the map 0 of a level
	mapTracker = 0;

	//Load the tile sheet
	tileMap.loadTexture("custom_sprites/NES_Mega_Man_Tiles.PNG");

	for (int i = 0; i <= 34; i++)
	{
		tile.setSize(sf::Vector2f(50, 50));		//We will be able to draw 14 tiles vertically and 24 tiles horizontally
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
			tile.setTargetName("worldSolid");
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
	//Init a tracker that will determinate if the player is outside of the map
	bool isInsideMap = false;

	//Init vars
	std::vector<GameObject>* world = tileMap.getLevel();
	std::string left, top, right, bottom, middle;

	//Check for collisions
	for (unsigned i = 0; i < (unsigned)world->size(); ++i)
	{
		//Determine the tiles around the center of the sprite so we can base our physics on that
		//Will help a lot with stuff like ladders
		if (Collision::checkBoundingBox(&(*world)[i], sf::Vector2i(p.getPosition() + p.getSize() / 2.f)))
		{
			//The center has collided with a tile, he is inside the map
			isInsideMap = true;

			//Use a for loop for left and right tiles, as the array will give a result for i - 1 while you are standing
			//next to the void, we need to check if the middle is at anyone of the left hand or right hand sides tiles
			for (unsigned j = 0; j < mapSize.x * (mapSize.y - 1) - 1; j += mapSize.x)
			{
				if (i == j)
				{
					left = "none";
					break;
				}
				else left = (*world)[i - 1].getTargetname();
			}
			if (i > mapSize.x)
				top = (*world)[i - mapSize.x].getTargetname();
			else
				top = "none";
			for (unsigned j = mapSize.x - 1; j < mapSize.x * mapSize.y - 1; j += mapSize.x)
			{
				if (i == j)
				{
					right = "none";
					break;
				}
				else right = (*world)[i + 1].getTargetname();
			}
			if (i < mapSize.x * (mapSize.y - 1))
				bottom = (*world)[i + mapSize.x].getTargetname();
			else
				bottom = "none";
			middle = (*world)[i].getTargetname();
			//Neighbour tiles have been detected, assigne the to the player
			p.setNeighborsTilesTargetNames(left, top, right, bottom, middle);
		}

		//Collision detection
		if ((*world)[i].isCollider())
		{
			if (Collision::checkBoundingBox(&p, &(*world)[i]))
			{
				//If they collided, trigger the response
				p.collisionResponse(&(*world)[i]);
				
				//Only show the collision boxes of the collided tiles if debug is ON
				if (debugUi->isDebugging())
					(*world)[i].setDebugging(true);
				else
					(*world)[i].setDebugging(false);
			}
			else
			{
				//To every tile that we did not collided with, make sure their debug is turned off
				if((*world)[i].isDebugging())
					(*world)[i].setDebugging(false);
			}
		}
	}

	//If the middle of the player is not inside the map but he is climbing a ladder, load the next screen
	if (!isInsideMap && p.getLadderTracker() &&
		p.getCollisionBox().top + p.getCollisionBox().height / 2 < window->getView().getCenter().y - window->getView().getSize().y / 2)
	{
		++mapTracker;
		isInsideMap = true;
	}
	else if (!isInsideMap && p.getLadderTracker() &&
		p.getCollisionBox().top + p.getCollisionBox().height / 2 > window->getView().getCenter().y + window->getView().getSize().y / 2)
	{
		--mapTracker;
		isInsideMap = true;
	}
}

void TileManager::render()
{
	tileMap.render(window);

	//Debug tiles
	if (debugUi->isDebugging())
	{
		std::vector<GameObject>* world = tileMap.getLevel();
		for (unsigned i = 0; i < (int)world->size(); ++i)
		{
			if ((*world)[i].isDebugging())
			{
				(*world)[i].updateDebugBoxes();
				window->draw(*(*world)[i].getDebugCollisionBox());
			}
		}
	}
}

void TileManager::createMap(Maps level, unsigned section)
{
	//1 screen is composed of 24 tiles horizontally and 14 tiles vertically
	switch (level)
	{
	case Maps::TUTORIAL:
		switch (section)
		{
		case 0:
			// Map dimensions
			mapSize = sf::Vector2u(40, 14);
			map = {
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	32,	0,	0,	0,	0,	0,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	0,	0,	0,	0,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	32,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	32,	24,	24,	24,	24,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	32,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	6,	4,	6,	6,	4,	6,	32,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	7,	7,	1,	2,	2,	3,	32,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	6,	6,	6,	6,	7,	7,	7,	7,	7,	7,	32,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	8,	8,	8,	8,	7,	7,	5,	7,	7,	7,	32,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	9,	9,	9,	9,	7,	5,	24,	24,	24,	24,	24,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	24,	24,	24,	24,	24,	24,	24,	24,	24,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	7,	7,	5,	7,	7,	7,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	23,	22,	23,	22,	23,	22,	23,	22,	23,
				24,	24,	24,	24, 24, 24, 24, 24, 24, 24,	24,	24,	24,	24,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	33,	33,	33,	33,	33,	23,	22,	23,	22,	23,	22,	23,	22,	23,
				22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23
			};
			break;
		case 1:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				0,	0,	0,	32,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				0,	0,	0,	32,	6,	6,	6,	4,	4,	6,	6,	6,	6,	6,	4,	6,	6,	6,	6,	6,	6,	22,	23,	22,	
				0,	0,	0,	32,	7,	7,	7,	7,	7,	7,	7,	7,	7,	7,	7,	7,	7,	7,	7,	7,	7,	6,	23,	22,
				0,	0,	0,	32,	7,	8,	7,	7,	7,	7,	7,	7,	7,	7,	1,	2,	3,	7,	7,	7,	7,	7,	4,	22,
				0,	0,	0,	32,	7,	9,	7,	7,	7,	24,	24,	24,	32,	24,	24,	24,	7,	7,	7,	7,	7,	7,	7,	22,
				0,	0,	0,	32,	7,	7,	5,	3,	7,	7,	7,	7,	32,	7,	7,	7,	7,	7,	1,	2,	2,	3,	7,	22,
				0,	0,	0,	7,	7,	7,	7,	7,	7,	7,	7,	7,	32,	7,	8,	8,	7,	7,	7,	7,	7,	7,	7,	22,
				0,	0,	7,	7,	7,	7,	7,	8,	8,	8,	8,	7,	32,	7,	9,	9,	7,	7,	7,	7,	7,	7,	7,	22,
				24,	7,	7,	7,	7,	7,	24,	24,	24,	24,	24,	24,	32,	24,	24,	24,	24,	24,	24,	7,	7,	5,	7,	22,
				23,	7,	7,	7,	5,	7,	7,	9,	9,	9,	9,	7,	32,	7,	7,	7,	7,	7,	7,	7,	5,	7,	7,	22,
				23,	24,	5,	7,	7,	7,	7,	7,	1,	3,	7,	7,	32,	7,	7,	7,	5,	5,	7,	7,	7,	7,	24,	22,
				23,	22,	7,	7,	7,	7,	5,	7,	7,	7,	7,	7,	32,	7,	7,	7,	7,	7,	7,	7,	7,	1,	23,	22,
				23,	22,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	32,	24,	24,	24,	24,	24,	23,	22,
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	32,	22,	23,	22,	23,	22,	23,	22	
			};
			break;
		case 2:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	22,	23,	22,	23,	24,	24,	24,	24,	24,	24,	24,	24,	24,
				0,	0,	0,	32,	24,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				0,	0,	0,	32,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22
			};
			break;
		}
		break;
	case Maps::SCIMAN:

		break;
	}
}

void TileManager::buildCreatedMap(sf::Vector2f position)
{
	//Once the map has been created/declared, set it
	tileMap.resetLevel();
	tileMap.setTileMap(map, mapSize);
	tileMap.setPosition(position);
	tileMap.buildLevel();
}