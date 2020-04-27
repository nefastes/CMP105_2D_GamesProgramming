#include "Sciman.h"
Sciman::Sciman(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Configure level
	currentLevel = Maps::SCIMAN;

	//Initialise the pointers into the level
	initLevel(hwnd, in, aud, gs, dui);

	//Init enemies
	suzyManager.sendPointers(&tileManager, &itemManager, audio, gameState);
	suzyManager.spawnSuzy(sf::Vector2f(39 * 50, 5 * 50), false);
	suzyManager.spawnSuzy(sf::Vector2f(39 * 50, 9 * 50), true);

	//Spawn items of section 0
	itemManager.spawnItem(sf::Vector2f(0 * 50, 4 * 50), 0);
	itemManager.spawnItem(sf::Vector2f(1 * 50, 4 * 50), 0);
	itemManager.spawnItem(sf::Vector2f(37 * 50, 1 * 50), 0);
}

Sciman::~Sciman()
{

}

void Sciman::handleInput(float dt)
{
	//handle the level inputs
	handleLevelInput(dt);

	//Add any level specific input handles here

}

void Sciman::update(float dt)
{
	//Update the level
	updateLevel(dt);

	//Add any level specific updates here
	suzyManager.update(dt, player);
	//Update the spawnpoint if progress has been made
	if (currentMap == 4)
	{
		spawnPoint = sf::Vector2f(19, 7);
		spawnMap = currentMap;
	}

	//Debugging
	if (debugUi->isDebugging())
	{
		suzyManager.setDebugging(true);
	}
}

void Sciman::render()
{
	beginDraw();

	//Render the level
	renderLevel();

	//Add any level specific renders here
	suzyManager.render(window);

	endDraw();
}

void Sciman::handlePause(float dt)
{
	//The level class handles the pause for us
	handleLevelPause(dt);
}

void Sciman::spawnItemsInRoom(sf::Vector2f position)
{
	//Spawn at level specific items locations
	itemManager.killAllItems();
	suzyManager.killAllSuzies();
	switch (currentMap)
	{
	case 0:
		itemManager.spawnItem(position + sf::Vector2f(0 * 50, 4 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(1 * 50, 4 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(37 * 50, 1 * 50), 0);
		suzyManager.spawnSuzy(position + sf::Vector2f(39 * 50, 5 * 50), false);
		suzyManager.spawnSuzy(position + sf::Vector2f(39 * 50, 9 * 50), true);
		break;
	case 1:
		itemManager.spawnItem(position + sf::Vector2f(9 * 50, 8 * 50), 1);
		break;
	case 2:
		itemManager.spawnItem(position + sf::Vector2f(23 * 50, 5 * 50), 3);
		break;
	case 3:
		itemManager.spawnItem(position + sf::Vector2f(52 * 50, 1 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(53 * 50, 1 * 50), 2);
		itemManager.spawnItem(position + sf::Vector2f(70 * 50, 9 * 50), 0);
		break;
	case 4:
		break;
	case 5:
		itemManager.spawnItem(position + sf::Vector2f(22 * 50, 6 * 50), 1);
		break;
	case 6:
		break;
	case 7:
		itemManager.spawnItem(position + sf::Vector2f(63 * 50, 8 * 50), 2);
		break;
	case 8:
		break;
	case 9:
		break;
	case 10:
		break;
	case 11:
		itemManager.spawnItem(position + sf::Vector2f(12.5f * 50, 2 * 50), 4);
		break;
	}
}