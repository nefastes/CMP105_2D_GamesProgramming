#include "TileManager.h"
TileManager::TileManager()
{
	//We start with the map 0 of a level
	mapTracker = 0;

	//We do not have a transition at the start of the level
	transitionning = false;
	transitionType = 0;
	doorTracker = 0;
	doorClosing = false;

	//Other trackers
	timeTracker = 0;

	//Pointers so that Visual Studio is happy
	window = nullptr;
	debugUi = nullptr;
	audio = nullptr;


	//Load the tile sheet
	tileMap.loadTexture("custom_sprites/NES_Mega_Man_Tiles.PNG");

	for (int i = 0; i <= 37; i++)
	{
		tile.setSize(sf::Vector2f(50, 50));		//We will be able to draw 14 tiles vertically and 24 tiles horizontally
		tile.setCollisionBox(0, 0, 50, 50);
		//all normal non colliding tiles init
		if (i == 0)
		{
			tile.setCollider(false);
			tile.setTargetName("sky");
		}
		else if (i < 22)
		{
			tile.setCollider(false);
			tile.setTargetName("world");
		}
		//all normal colliding tiles init
		else if (i < 31)
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
	tiles[34].setTextureRect(sf::IntRect(360, 172, 16, 16));			//Door tile, triggers an opening door
	tiles[34].setTargetName("door");
	tiles[35].setTextureRect(sf::IntRect(360, 172, 16, 16));			//Door tile, normal solid tile
	tiles[35].setTargetName("worldSolid");
	tiles[36].setTextureRect(sf::IntRect(275, 189, 16, 16));			//Sky looking tile, used to trigger a transition
	tiles[36].setTargetName("transition");								//of the screen downwards if the player falls into it

	//Void tile (nothingness)
	tiles[37].setTextureRect(sf::IntRect(103, 121, 16, 16));
	tiles[37].setTargetName("none");

	//Set the tileMap
	tileMap.setTileSet(tiles);
}

TileManager::~TileManager()
{

}

void TileManager::checkItemCollision(Item& item)
{
	std::vector<GameObject>* world = tileMap.getLevel();
	for (unsigned i = 0; i < (*world).size(); ++i)
	{
		if ((*world)[i].isCollider())
		{
			if (Collision::checkBoundingBox(&item, &(*world)[i]))
			{
				float dx = (item.getPosition().x + item.getSize().x / 2.f) - ((*world)[i].getPosition().x + (*world)[i].getSize().x / 2.f);
				float dy = (item.getPosition().y + item.getSize().y / 2.f) - ((*world)[i].getPosition().y + (*world)[i].getSize().y / 2.f);

				//Check if it is a vertical collision
				if (std::abs(dx) <= std::abs(dy))
				{
					//Only set grounded if it was a top of the tile collision
					if (dy < 0)
					{
						item.setPosition(item.getPosition().x, (*world)[i].getPosition().y - item.getCollisionBox().height);
						item.setGrounded(true);
						break;
					}
				}
				//Else, horizontal collision
				else
				{
					//Left side wall hit
					if (dx < 0)
						item.setPosition((*world)[i].getPosition().x - item.getSize().x, item.getPosition().y);
					//Right side wall hit
					else
						item.setPosition((*world)[i].getPosition().x + (*world)[i].getSize().x, item.getPosition().y);
				}
			}
		}
	}
}

void TileManager::checkSuzyCollision(Suzy& suzy)
{
	std::vector<GameObject>* world = tileMap.getLevel();
	for (unsigned i = 0; i < (*world).size(); ++i)
	{
		if ((*world)[i].isCollider() && (*world)[i].getTargetname() != "ladder")
		{
			if (Collision::checkBoundingBox(&suzy, &(*world)[i]))
			{
				float dx = (suzy.getPosition().x + suzy.getSize().x / 2.f) - ((*world)[i].getPosition().x + (*world)[i].getSize().x / 2.f);
				float dy = (suzy.getPosition().y + suzy.getSize().y / 2.f) - ((*world)[i].getPosition().y + (*world)[i].getSize().y / 2.f);

				//Vertical collision
				//We want to make sure they are on the same tile position, otherwise it might collide with thiles on the left or right
				if (std::abs(dx) <= std::abs(dy) && suzy.isMovingVertically() && suzy.getPosition().x == (*world)[i].getPosition().x)
				{
					//Top collision
					if (dy < 0)
						suzy.setPosition(suzy.getPosition().x, (*world)[i].getPosition().y - suzy.getSize().y);
					//Bottom Collision
					else
						suzy.setPosition(suzy.getPosition().x, (*world)[i].getPosition().y + (*world)[i].getSize().y);

					//Collision has been made
					suzy.setWallState(true);
					suzy.setVelocity(suzy.getVelocity() * -1.f);
					break;
				}
				//We want to make sure they are on the same tile position, otherwise it might collide with thiles upper or below
				else if (suzy.getPosition().y == (*world)[i].getPosition().y)
				{
					//Left side wall hit
					if (dx < 0)
						suzy.setPosition((*world)[i].getPosition().x - suzy.getSize().x, suzy.getPosition().y);
					//Right side wall hit
					else
						suzy.setPosition((*world)[i].getPosition().x + (*world)[i].getSize().x, suzy.getPosition().y);

					//Collision has been made
					suzy.setWallState(true);
					suzy.setVelocity(suzy.getVelocity() * -1.f);
					break;
				}
			}
		}
	}
}

void TileManager::identifySuzyStartDirection(Suzy& suzy, bool vertical)
{
	std::vector<GameObject>* world = tileMap.getLevel();
	for (unsigned i = 0; i < (*world).size(); ++i)
	{
		if ((*world)[i].isCollider() && (*world)[i].getTargetname() != "ladder")
		{
			if (Collision::checkBoundingBox(&suzy, &(*world)[i]))
			{
				if (vertical)
				{
					//Collide with top side of the wall, thus we must ensure his speed will make him go up
					if ((suzy.getPosition().y + suzy.getSize().y / 2.f) - ((*world)[i].getPosition().y + (*world)[i].getSize().y / 2.f) < 0)
					{
						if (suzy.getVelocity().y > 0)
							suzy.setVelocity(suzy.getVelocity() * -1.f);
					}
					//Collides with bottom side of the wall, thus we must ensure his speed will make him go down
					else
					{
						if (suzy.getVelocity().y < 0)
							suzy.setVelocity(suzy.getVelocity() * -1.f);
					}
				}
				else
				{
					//Collides with left side of the wall, thus we must ensure his speed will make him go left
					if ((suzy.getPosition().x + suzy.getSize().x / 2.f) - ((*world)[i].getPosition().x + (*world)[i].getSize().x / 2.f) < 0)
					{
						if (suzy.getVelocity().x > 0)
							suzy.setVelocity(suzy.getVelocity() * -1.f);
					}
					//Collides with right side of the wall, thus we must ensure his speed will make him go right
					else
					{
						if (suzy.getVelocity().x < 0)
							suzy.setVelocity(suzy.getVelocity() * -1.f);
					}
				}
				suzy.setWallState(true);
				return;
			}
		}
	}
}

void TileManager::update(float dt, Player& p)
{
	//Init a tracker that will determinate if the player is outside of the map
	bool isInsideMap = false;
	//Init a tracker that will be set to true when a potential vertical transition can be triggered
	bool potentialVerticalTransition = false;
	//Init a tracker that will be set to true when we collide with a door
	bool potentialHorizontalTransition = false;

	//Init vars
	std::vector<GameObject>* world = tileMap.getLevel();
	std::string left, top, right, bottom, middle;

	//Check for collisions
	for (unsigned i = 0; i < (unsigned)world->size(); ++i)
	{
		//Determine the tiles around the center of the sprite so we can base our physics on that
		//Will help a lot with stuff like ladders
		sf::Vector2i playerCenter = sf::Vector2i(p.getCollisionBox().left + p.getCollisionBox().width / 2.f,
			p.getCollisionBox().top + p.getCollisionBox().height / 2.f);
		if (Collision::checkBoundingBox(&(*world)[i], playerCenter))
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
				//If the player collides with a ladder or a transition block, we might have a transition
				if ((*world)[i].getTargetname() == "ladder" || (*world)[i].getTargetname() == "transition")
					potentialVerticalTransition = true;
				else if ((*world)[i].getTargetname() == "door")
					potentialHorizontalTransition = true;

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

	//Top transition: If the middle of the player is not inside the map but he is climbing a ladder, load the next screen
	if (!isInsideMap && p.getLadderTracker() &&
		p.getCollisionBox().top + p.getCollisionBox().height / 2 < window->getView().getCenter().y - window->getView().getSize().y / 2)
	{
		++mapTracker;
		isInsideMap = true;
		transitionning = true;
		transitionType = 1;
	}
	//Bottom transition: If the middle of the player is not inside the map but he is colliding with a transition type tile
	//(ladder, transition), load previous screen
	else if (!isInsideMap && potentialVerticalTransition && mapTracker > 0 &&
		p.getCollisionBox().top + p.getCollisionBox().height / 2 > window->getView().getCenter().y + window->getView().getSize().y / 2)
	{
		--mapTracker;
		isInsideMap = true;
		transitionning = true;
		transitionType = 2;
	}
	//Else, if we collide with a door, open the door and load next screen
	else if (potentialHorizontalTransition)
	{
		//Increase the time tracker
		timeTracker += dt;

		//horizontal transition, in both cases we freeze the player
		p.freezeControls(true);
		p.setMoving(false);
		p.setTransitionning(true);

		//Identify the top tile of the door position
		for (unsigned i = 0; i < world->size(); ++i)
		{
			if ((*world)[i].getTargetname() == "door")
			{
				doorPos = i;
				break;
			}
		}

		//There are only two openable doors in each level, just need to know which
		//TODO: add switch Maps::TUTORIAL
		//In the tutorial level, doors are available in the map 2 and 3
		if (timeTracker >= .1f && !doorClosing)
		{
			//See if we are opening from outside or inside, so that the open anim replaces the corresponding tiles
			if((*world)[doorPos - 1].getTargetname() == "sky")		openDoor(doorPos, false);
			if((*world)[doorPos - 1].getTargetname() == "world")	openDoor(doorPos, true);
		}
	}
	else if (doorClosing)
	{
		//Close the door from tile i = doorPos - 23, when it is finished re-enable player controls
		if (closeDoor(doorPos - 23))
		{
			p.freezeControls(false);
			p.setTransitionning(false);
		}
	}
	//If not, he is not transitionning thus we reset the transition type
	else if (!transitionning && transitionType != 0) transitionType = 0;
}

void TileManager::render()
{
	if(!transitionning)
		tileMap.render(window, false);
	else
		tileMap.render(window, true);

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
				0,	0,	0,	0,	0,	0,	0,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	32,	0,	0,	0,	0,	0,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	0,	0,	0,	0,	0,	0,
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
				0,	0,	7,	7,	7,	7,	7,	7,	7,	7,	7,	7,	32,	7,	9,	9,	7,	7,	7,	7,	7,	7,	7,	22,
				24,	7,	7,	7,	7,	7,	24,	24,	24,	24,	24,	24,	32,	24,	24,	24,	24,	24,	24,	7,	7,	5,	7,	22,
				23,	7,	7,	7,	5,	7,	7,	7,	7,	7,	7,	7,	32,	7,	7,	7,	7,	7,	7,	7,	5,	7,	7,	22,
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
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	22,	23,	22,	23,	24,	24,	24,	24,	24,	24,	24,	24,	24,
				0,	0,	0,	32,	24,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				36,	36,	36,	32,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22
			};
			break;
		case 3:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,
				6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	4,	4,	4,	34,
				7,	7,	7,	7,	7,	5,	7,	7,	7,	7,	5,	7,	7,	7,	7,	7,	7,	7,	5,	5,	5,	5,	5,	34,
				5,	7,	7,	7,	7,	7,	7,	7,	7,	7,	5,	7,	7,	7,	7,	7,	5,	5,	5,	5,	5,	5,	5,	34,
				7,	7,	7,	5,	7,	7,	1,	7,	7,	7,	7,	7,	7,	7,	5,	5,	5,	5,	5,	5,	5,	5,	5,	34,
				24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37
			};
			break;
		case 4:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				23,	22,	23,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	22,	23,	22,
				23,	22,	4,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	4,	23,	22,
				23,	4,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	4,	22,
				23,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				23,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				23,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				4,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				24,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				23,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	22,
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22
			};
			break;
		}
		break;
	case Maps::SCIMAN:
		switch (section)
		{
		case 0:
			// Map dimensions
			mapSize = sf::Vector2u(48, 14);
			map = {
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	22,	23,	22,	23,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	0,	0,	0,	0,	0,	0,	0,	32,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	32,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	22,	23,	32,	24,	24,	24,	24,	24,	24,	24,	22,
				28,	29,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	22,	23,	32,	6,	6,	6,	6,	4,	4,	23,	22,
				30,	31,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	4,	22,	23,	32,	7,	1,	2,	2,	3,	24,	23,	22,
				24,	24,	24,	24,	24,	32,	0,	0,	0,	0,	0,	0,	24,	24,	32,	24,	24,	24,	24,	0,	0,	0,	0,	24,	24,	24,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	7,	22,	23,	32,	7,	7,	7,	5,	7,	22,	23,	22,
				6,	6,	6,	4,	6,	32,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	0,	0,	0,	7,	6,	23,	24,	24,	24,	24,	24,	32,	22,	23,	22,
				7,	7,	7,	7,	7,	32,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	22,	23,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	7,	4,	6,	6,	6,	6,	6,	32,	22,	23,	22,
				7,	7,	1,	3,	7,	32,	0,	0,	0,	0,	0,	0,	0,	0,	32,	28,	29,	22,	23,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	5,	7,	7,	5,	7,	7,	5,	32,	22,	23,	22,
				5,	7,	7,	7,	7,	32,	0,	0,	0,	0,	0,	0,	0,	0,	32,	30,	31,	22,	23,	22,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	7,	1,	7,	7,	5,	7,	5,	32,	22,	23,	22,
				24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	22,	23,	22,	23,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	22,	23,	22,
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23, 22
			};
			break;
		case 1:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	32,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				23,	22,	23,	6,	4,	6,	6,	4,	4,	6,	6,	22,	32,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				23,	22,	23,	7,	7,	7,	7,	7,	7,	7,	7,	7,	32,	22,	6,	6,	6,	6,	6,	6,	6,	6,	23,	22,
				23,	22,	6,	7,	7,	8,	7,	7,	7,	7,	7,	7,	32,	22,	1,	2,	3,	7,	7,	7,	7,	7,	4,	22,
				23,	22,	7,	7,	7,	9,	7,	7,	24,	24,	24,	24,	24,	22,	7,	7,	7,	7,	7,	7,	7,	7,	7,	22,
				23,	6,	1,	7,	7,	7,	5,	5,	23,	22,	23,	22,	23,	22,	7,	7,	7,	7,	1,	2,	2,	3,	7,	22,
				23,	7,	7,	7,	7,	7,	24,	24,	23,	7,	7,	7,	23,	22,	8,	8,	7,	7,	7,	7,	7,	24,	32,	22,
				23,	5,	7,	7,	7,	7,	23,	22,	23,	7,	7,	7,	23,	22,	9,	9,	7,	7,	7,	7,	7,	22,	32,	22,
				23,	32,	24,	24,	24,	24,	23,	22,	23,	24,	24,	32,	23,	22,	24,	24,	1,	5,	24,	24,	24,	22,	32,	22,
				23,	32,	23,	22,	23,	22,	23,	22,	23,	22,	23,	32,	23,	22,	23,	22,	7,	7,	23,	22,	23,	22,	32,	22,
				23,	32,	5,	7,	7,	7,	7,	7,	1,	3,	7,	32,	5,	7,	7,	7,	5,	5,	7,	7,	23,	22,	32,	22,
				23,	32,	7,	7,	7,	7,	5,	7,	7,	7,	7,	32,	7,	7,	7,	7,	7,	7,	7,	7,	23,	22,	32,	22,
				23,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	23,	22,	32,	22,
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	22,	22,	23,	22,	23,	22,	32,	22
			};
			break;
		case 2:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				0,	0,	0,	32,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				0,	0,	0,	32,	23,	22,	23,	22,	23,	22,	23,	22,	23,	6,	6,	6,	6,	6,	4,	4,	23,	22,	23,	22,
				0,	0,	0,	32,	23,	22,	23,	22,	23,	22,	23,	22,	4,	7,	7,	8,	8,	8,	8,	7,	6,	22,	23,	22,
				0,	0,	0,	6,	23,	22,	23,	22,	23,	6,	6,	6,	7,	7,	7,	9,	9,	9,	9,	7,	7,	6,	23,	22,
				0,	0,	0,	7,	4,	22,	23,	22,	6,	7,	7,	7,	5,	7,	7,	7,	7,	7,	7,	7,	7,	7,	4,	4,
				24,	24,	7,	1,	3,	6,	4,	6,	7,	7,	7,	7,	5,	7,	24,	24,	24,	24,	24,	32,	1,	7,	7,	7,
				23,	22,	7,	5,	7,	7,	7,	7,	7,	7,	5,	7,	7,	7,	23,	22,	6,	6,	6,	32,	7,	7,	23,	22,
				23,	22,	24,	24,	7,	7,	5,	5,	7,	24,	24,	24,	24,	24,	23,	6,	7,	7,	7,	32,	7,	7,	23,	22,
				23,	22,	23,	22,	7,	5,	7,	7,	7,	22,	23,	22,	4,	6,	6,	7,	7,	7,	7,	32,	7,	5,	23,	22,
				23,	22,	23,	22,	24,	24,	24,	24,	24,	22,	23,	22,	7,	8,	8,	7,	1,	2,	3,	32,	7,	7,	23,	22,
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	7,	9,	9,	7,	7,	24,	24,	24,	24,	24,	23,	22,
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	7,	7,	7,	7,	7,	22,	23,	22,	23,	22,	23,	22,
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	32,	24,	24,	24,	24,	22,	23,	22,	23,	22,	23,	22,
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	32,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22
			};
			break;
		case 3:
			// Map dimensions
			mapSize = sf::Vector2u(72, 14);
			map = {
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	22,	23,	22,	23,	22,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	23,	23,	22,	23,	22,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	24,	0,	0,	0,	0,	24,	24,	24,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	23,	22,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	22,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	32,	24,	24,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	19,	20,	20,	20,	20,	21,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	19,	20,	20,	20,	20,	21,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	11,	10,	15,	16,	10,	14,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	24,	0,	0,	0,	11,	10,	15,	16,	10,	14,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	11,	10,	12,	13,	10,	14,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	22,	23,	22,	23,	24,	0,	0,	11,	10,	12,	13,	10,	14,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	0,	11,	10,	17,	18,	10,	14,	0,	0,	0,	0,	0,	0,	0,	32,	28,	29,	0,	0,	0,	0,	24,	24,	24,	24,	24,	24,	23,	22,	23,	22,	23,	22,	24,	0,	11,	10,	17,	18,	10,	14,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	22,	23,	22,	23,	24,	24,	24,	24,	24,	24,	24,	24,	24,	0,	0,	0,	0,	0,	32,	30,	31,	0,	0,	0,	24,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	24,	24,	24,	24,	24,	24,	24,	24,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	22,	23,	22,	23,	22,
				0,	0,	0,	32,	24,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	24,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	23,	22,
				36,	36,	36,	32,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	24,	24,	33,	33,	33,	24,	24,	22,	23,	22,	23,	22,	23,	22
			};
			break;
		case 4:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,	24,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	22,	23,	22,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	23,	22,	23,	22,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	22,	23,	22,	23,	22,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	22,	23,	22,	23,	22,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	22,	23,	22,	23,	22,	23,	22,
				36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	32,	22,	23,	22,	23,	22,	23,	22
			};
			break;
		case 5:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				23,	22,	23,	22,	23,	22,	23,	6,	6,	6,	6,	6,	6,	6,	6,	4,	4,	4,	6,	22,	23,	22,	23,	22,
				23,	22,	23,	22,	23,	22,	6,	7,	7,	7,	7,	7,	5,	7,	7,	7,	7,	5,	7,	6,	6,	22,	23,	22,
				6,	6,	6,	6,	23,	22,	7,	7,	7,	24,	24,	24,	24,	24,	24,	7,	1,	2,	7,	7,	7,	6,	6,	22,
				7,	8,	8,	8,	6,	22,	3,	7,	5,	6,	4,	22,	23,	22,	23,	7,	7,	7,	7,	7,	8,	7,	5,	22,
				5,	9,	9,	9,	7,	22,	24,	7,	7,	7,	7,	6,	23,	22,	23,	24,	24,	24,	24,	7,	9,	7,	7,	22,
				7,	7,	7,	7,	7,	22,	23,	24,	7,	7,	5,	7,	23,	22,	23,	22,	23,	22,	23,	5,	7,	7,	7,	22,
				24,	24,	24,	7,	7,	22,	23,	22,	24,	24,	24,	32,	23,	22,	23,	22,	23,	22,	23,	32,	24,	24,	24,	22,
				23,	22,	23,	7,	7,	6,	23,	22,	23,	22,	23,	32,	23,	22,	23,	22,	23,	22,	23,	32,	6,	6,	6,	34,
				23,	22,	23,	7,	7,	7,	6,	22,	23,	22,	23,	32,	6,	4,	23,	22,	23,	22,	23,	32,	7,	7,	7,	34,
				23,	22,	23,	7,	5,	7,	7,	6,	23,	22,	6,	32,	7,	7,	6,	22,	23,	22,	23,	32,	7,	1,	3,	34,
				23,	22,	23,	24,	7,	7,	1,	2,	2,	2,	3,	32,	7,	28,	29,	22,	23,	22,	23,	32,	7,	7,	7,	34,
				23,	22,	23,	22,	24,	7,	7,	7,	7,	7,	7,	32,	7,	30,	31,	22,	23,	22,	23,	24,	24,	24,	24,	24,
				23,	22,	23,	22,	23,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	22
			};
			break;
		case 6:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				23,	22,	0,	0,	32,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
				23,	22,	33,	33,	32,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
				23,	22,	23,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	32,	0,
				23,	22,	23,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,
				23,	22,	23,	22,	0,	0,	0,	24,	0,	0,	0,	0,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,
				23,	22,	23,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	0,	0,	0,	0,	0,	0,	32,	0,
				23,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	24,	24,
				23,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	0,	0,	24,	0,	0,	0,	0,	24,	24,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	33,	33,	22,	33,	33,	33,	33,	23,	22,	24,	0,
				24,	24,	24,	24,	33,	33,	33,	24,	33,	33,	33,	24,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	24,
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22
			};
			break;
		case 7:
			// Map dimensions
			mapSize = sf::Vector2u(72, 14);
			map = {
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	22,	0,	0,	0,	0,	0,	0,	0,	32,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	32,	23,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	32,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	6,	6,	32,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	7,	5,	32,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	32,	24,	24,	24,	24,	0,	0,	0,	0,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	19,	20,	20,	20,	20,	21,	0,	0,	0,	0,	0,	0,	7,	7,	32,	23,	33,	33,	33,	33,	24,	24,	32,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	22,	23,	0,	0,	0,	0,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	11,	10,	15,	16,	10,	14,	0,	0,	0,	0,	0,	0,	1,	7,	32,	23,	24,	24,	24,	24,	22,	23,	32,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	0,	0,	22,	23,	24,	0,	0,	0,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	11,	10,	12,	13,	10,	14,	0,	0,	0,	0,	0,	24,	24,	24,	24,	23,	22,	23,	22,	23,	22,	23,	32,	0,	0,	0,	0,	0,	0,	19,	20,	20,	20,	20,	21,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	28,	29,	22,	23,	22,	0,	0,	0,	0,	0,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	11,	10,	17,	18,	10,	14,	0,	0,	24,	24,	24,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	32,	0,	0,	0,	0,	0,	0,	11,	10,	15,	16,	10,	14,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	32,	30,	31,	22,	23,	22,	24,	0,	0,	0,	0,
				0,	0,	0,	0,	32,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	24,	24,	24,	0,	0,	0,	0,	11,	10,	12,	13,	10,	14,	0,	0,	0,	24,	24,	24,	0,	0,	0,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,
				0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	24,	24,	0,	0,	11,	10,	17,	18,	10,	14,	0,	0,	24,	22,	23,	22,	33,	33,	33,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				0,	0,	0,	0,	32,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	23,	22,	23,	22,	24,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				33,	33,	36,	36,	32,	36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	36,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22
			};
			break;
		case 8:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				0,	0,	32,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				0,	0,	32,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				0,	0,	32,	22,	23,	22,	23,	22,	23,	22,	23,	22,	0,	0,	0,	0,	23,	22,	23,	22,	23,	0,	23,	22,
				0,	0,	32,	22,	23,	22,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	22,	23,	22,	0,	0,	0,	22,
				0,	0,	32,	0,	23,	22,	23,	22,	23,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,
				0,	0,	0,	0,	0,	0,	23,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	0,	0,	0,	0,	24,	32,	24,	22,
				24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	23,	22,	24,	0,	0,	0,	23,	32,	23,	22,
				23,	24,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	22,	23,	22,	23,	24,	24,	24,	23,	32,	23,	22,
				23,	22,	23,	24,	0,	0,	0,	0,	0,	0,	0,	0,	24,	22,	23,	22,	23,	22,	23,	22,	23,	32,	23,	22,
				23,	22,	23,	22,	24,	24,	0,	0,	0,	0,	0,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	32,	23,	22,
				0,	22,	23,	22,	23,	22,	0,	0,	0,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	32,	23,	22,
				0,	0,	23,	22,	23,	22,	0,	0,	0,	22,	23,	22,	23,	22,	23,	0,	0,	0,	23,	22,	23,	32,	23,	22,
				0,	0,	0,	0,	23,	36,	36,	36,	36,	22,	23,	22,	23,	22,	0,	0,	0,	0,	0,	22,	23,	32,	23,	22
			};
			break;
		case 9:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,	25,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	27,	25,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	24,
				0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	22,	23,	22,
				24,	24,	32,	24,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	24,	24,	23,	22,	23,	22,
				23,	22,	32,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	24,	22,	23,	22,	23,	22,	23,	22,
				23,	22,	32,	22,	24,	24,	24,	0,	0,	0,	0,	24,	24,	24,	24,	24,	23,	22,	23,	22,	23,	22,	23,	22,
				23,	22,	32,	22,	23,	22,	23,	24,	24,	24,	24,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				0,	22,	32,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				36,	36,	32,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22
			};
			break;
		case 10:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,
				6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	6,	4,	4,	4,	34,
				7,	7,	7,	7,	7,	5,	7,	7,	7,	7,	5,	7,	7,	7,	7,	7,	7,	7,	5,	5,	5,	5,	5,	34,
				5,	7,	7,	7,	7,	7,	7,	7,	7,	7,	5,	7,	7,	7,	7,	7,	5,	5,	5,	5,	5,	5,	5,	34,
				7,	7,	7,	5,	7,	7,	1,	7,	7,	7,	7,	7,	7,	7,	5,	5,	5,	5,	5,	5,	5,	5,	5,	34,
				24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,
				37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37,	37
			};
			break;
		case 11:
			// Map dimensions
			mapSize = sf::Vector2u(24, 14);
			map = {
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,
				23,	22,	23,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	4,	22,	23,	22,
				4,	4,	4,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	4,	23,	22,
				5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	4,	22,
				5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				24,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				23,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				23,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				23,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				23,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				23,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	5,	22,
				23,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	24,	22,
				23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22,	23,	22
			};
			break;
		}
		break;
	}
}

void TileManager::buildCreatedMap(sf::Vector2f position)
{
	//Once the map has been created/declared, set it
	tileMap.inverseLevel();
	tileMap.resetLevel();
	tileMap.setTileMap(map, mapSize);
	tileMap.setPosition(position);
	tileMap.buildLevel();
}

void TileManager::openDoor(unsigned topTile, bool inside)
{
	//Get the level
	std::vector<GameObject>* world = tileMap.getLevel();

	//The door tiles in this room are located in positions 191, 215, 239 and 263
	//Replace each tile with air 1 by one
	switch (doorTracker)
	{
	case 0:
		if (audio->getSound("door")->getStatus() == sf::SoundSource::Stopped)
		{
			if(inside)
				(*world)[topTile + 72].setTextureRect(sf::IntRect(394, 172, 16, 16));
			else
				(*world)[topTile + 72].setTextureRect(sf::IntRect(275, 189, 16, 16));
			audio->playSoundbyName("door");
			++doorTracker;
		}
		break;
	case 1:
		if (audio->getSound("door")->getStatus() == sf::SoundSource::Stopped)
		{
			if(inside)
				(*world)[topTile + 48].setTextureRect(sf::IntRect(394, 172, 16, 16));
			else
				(*world)[topTile + 48].setTextureRect(sf::IntRect(275, 189, 16, 16));
			audio->playSoundbyName("door");
			++doorTracker;
		}
		break;
	case 2:
		if (audio->getSound("door")->getStatus() == sf::SoundSource::Stopped)
		{
			if(inside)
				(*world)[topTile + 24].setTextureRect(sf::IntRect(394, 172, 16, 16));
			else
				(*world)[topTile + 24].setTextureRect(sf::IntRect(275, 189, 16, 16));
			audio->playSoundbyName("door");
			++doorTracker;
		}
		break;
	case 3:
		if (audio->getSound("door")->getStatus() == sf::SoundSource::Stopped)
		{
			if(inside)
				(*world)[topTile].setTextureRect(sf::IntRect(377, 172, 16, 16));
			else
				(*world)[topTile].setTextureRect(sf::IntRect(275, 189, 16, 16));
			audio->playSoundbyName("door");
			++doorTracker;
		}
		break;
	default:
		if (audio->getSound("door")->getStatus() == sf::SoundSource::Stopped)
		{
			++mapTracker;
			transitionning = true;
			transitionType = 3;
			timeTracker = 0;
			doorTracker = 0;
		}
		break;
	}
}

bool TileManager::closeDoor(unsigned topTile)
{
	//Get the level
	std::vector<GameObject>* world = tileMap.getLevel();

	//Close the doore
	switch (doorTracker)
	{
	case 3:
		if (audio->getSound("door")->getStatus() == sf::SoundSource::Stopped)
		{
			(*world)[topTile + 72].setTextureRect(sf::IntRect(360, 172, 16, 16));
			(*world)[topTile + 72].setCollider(true);
			(*world)[topTile + 72].setTargetName("worldSolid");
			audio->playSoundbyName("door");
			++doorTracker;
		}
		break;
	case 2:
		if (audio->getSound("door")->getStatus() == sf::SoundSource::Stopped)
		{
			(*world)[topTile + 48].setTextureRect(sf::IntRect(360, 172, 16, 16));
			(*world)[topTile + 48].setCollider(true);
			(*world)[topTile + 48].setTargetName("worldSolid");
			audio->playSoundbyName("door");
			++doorTracker;
		}
		break;
	case 1:
		if (audio->getSound("door")->getStatus() == sf::SoundSource::Stopped)
		{
			(*world)[topTile + 24].setTextureRect(sf::IntRect(360, 172, 16, 16));
			(*world)[topTile + 24].setCollider(true);
			(*world)[topTile + 24].setTargetName("worldSolid");
			audio->playSoundbyName("door");
			++doorTracker;
		}
		break;
	case 0:
		if (audio->getSound("door")->getStatus() == sf::SoundSource::Stopped)
		{
			(*world)[topTile].setTextureRect(sf::IntRect(360, 172, 16, 16));
			(*world)[topTile].setCollider(true);
			(*world)[topTile].setTargetName("worldSolid");
			audio->playSoundbyName("door");
			++doorTracker;
		}
		break;
	default:
		if (audio->getSound("door")->getStatus() == sf::SoundSource::Stopped)
		{
			transitionning = false;
			doorClosing = false;
			doorTracker = 0;
			return true;
		}
		break;
	}
	return false;
}