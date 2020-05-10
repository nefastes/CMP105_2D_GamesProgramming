#include "Sciman.h"
Sciman::Sciman(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Configure level
	currentLevel = Maps::SCIMAN;

	//Initialise the pointers into the level
	initLevel(hwnd, in, aud, gs, dui);

	//Init enemies pointers
	suzyManager.sendPointers(&tileManager, &itemManager, audio, gameState);
	blasterManager.sendPointers(&tileManager, &itemManager, audio, gameState);
	kamadomaManager.sendPointers(&tileManager, &itemManager, audio, gameState);
	superCutterManager.sendPointers(&tileManager, &itemManager, audio, gameState);

	//Init spawnables of the first section
	spawnItemsInRoom(tileManager.getMapPosition());
	

	//Spawn items of section 0
	itemManager.spawnItem(sf::Vector2f(0.25f * 50, 4.25f * 50), 0);
	itemManager.spawnItem(sf::Vector2f(1.25f * 50, 4.25f * 50), 0);
	itemManager.spawnItem(sf::Vector2f(37.25f * 50, 1.25f * 50), 0);
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
		blasterManager.setDebugging(true);
		kamadomaManager.setDebugging(true);
		superCutterManager.setDebugging(true);
	}
}

void Sciman::render()
{
	//DO NOT ADD anything else than this, use virtuals so you can get the order of display right

	//Render the level
	renderLevel();
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
	blasterManager.killAllBlasters();
	kamadomaManager.killAllKamadomas();
	superCutterManager.killAllCutters();
	//All small items are to be spawned at 1/4th positions (so that they are in the middle)
	//normal tile size items can be spawned normally
	switch (currentMap)
	{
	case 0:
		itemManager.spawnItem(position + sf::Vector2f(0.25f * 50, 4.25f * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(1.25f * 50, 4.25f * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(37.25f * 50, 1.25f * 50), 0);
		suzyManager.spawnSuzy(position + sf::Vector2f(39 * 50, 5 * 50), false);
		suzyManager.spawnSuzy(position + sf::Vector2f(39 * 50, 9 * 50), true);
		blasterManager.spawnBlaster(position + sf::Vector2f(20 * 50, 9 * 50), BlasterAimDirection::RIGHT);
		blasterManager.spawnBlaster(position + sf::Vector2f(26 * 50, 1 * 50), BlasterAimDirection::LEFT);
		blasterManager.spawnBlaster(position + sf::Vector2f(31 * 50, 1 * 50), BlasterAimDirection::RIGHT);
		break;
	case 1:
		itemManager.spawnItem(position + sf::Vector2f(9.25f * 50, 7.25f * 50), 1);
		suzyManager.spawnSuzy(position + sf::Vector2f(4 * 50, 10 * 50), true);
		suzyManager.spawnSuzy(position + sf::Vector2f(8 * 50, 11 * 50), true);
		suzyManager.spawnSuzy(position + sf::Vector2f(16 * 50, 8 * 50), false);
		suzyManager.spawnSuzy(position + sf::Vector2f(16 * 50, 9 * 50), false);
		suzyManager.spawnSuzy(position + sf::Vector2f(2 * 50, 4 * 50), false);
		suzyManager.spawnSuzy(position + sf::Vector2f(10 * 50, 1 * 50), true);
		break;
	case 2:
		itemManager.spawnItem(position + sf::Vector2f(23 * 50, 5 * 50), 3);
		suzyManager.spawnSuzy(position + sf::Vector2f(12 * 50, 9 * 50), false);
		suzyManager.spawnSuzy(position + sf::Vector2f(17 * 50, 6 * 50), true);
		suzyManager.spawnSuzy(position + sf::Vector2f(22 * 50, 4 * 50), true);
		suzyManager.spawnSuzy(position + sf::Vector2f(15 * 50, 1 * 50), true);
		suzyManager.spawnSuzy(position + sf::Vector2f(12 * 50, 2 * 50), false);
		suzyManager.spawnSuzy(position + sf::Vector2f(4 * 50, 7 * 50), false);
		suzyManager.spawnSuzy(position + sf::Vector2f(8 * 50, 8 * 50), false);
		break;
	case 3:
		itemManager.spawnItem(position + sf::Vector2f(52.25f * 50, 1.25f * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(53 * 50, 1 * 50), 2);
		itemManager.spawnItem(position + sf::Vector2f(70.25f * 50, 8.25f * 50), 0);
		blasterManager.spawnBlaster(position + sf::Vector2f(27 * 50, 6 * 50), BlasterAimDirection::LEFT);
		blasterManager.spawnBlaster(position + sf::Vector2f(50 * 50, 2 * 50), BlasterAimDirection::LEFT);
		blasterManager.spawnBlaster(position + sf::Vector2f(70 * 50, 5 * 50), BlasterAimDirection::LEFT);
		kamadomaManager.spawnKamadoma(position + sf::Vector2f(18 * 50, 10 * 50), true);
		kamadomaManager.spawnKamadoma(position + sf::Vector2f(41 * 50, 9 * 50), true);
		kamadomaManager.spawnKamadoma(position + sf::Vector2f(45 * 50, 7 * 50), true);
		kamadomaManager.spawnKamadoma(position + sf::Vector2f(45 * 50, 1 * 50), true);
		break;
	case 4:
		break;
	case 5:
		itemManager.spawnItem(position + sf::Vector2f(22.25f * 50, 6.25f * 50), 1);
		suzyManager.spawnSuzy(position + sf::Vector2f(3 * 50, 7 * 50), false);
		suzyManager.spawnSuzy(position + sf::Vector2f(7 * 50, 10 * 50), true);
		suzyManager.spawnSuzy(position + sf::Vector2f(10 * 50, 10 * 50), false);
		suzyManager.spawnSuzy(position + sf::Vector2f(10 * 50, 1 * 50), true);
		suzyManager.spawnSuzy(position + sf::Vector2f(21 * 50, 3 * 50), true);
		break;
	case 6:
		break;
	case 7:
		itemManager.spawnItem(position + sf::Vector2f(63 * 50, 7 * 50), 2);
		blasterManager.spawnBlaster(position + sf::Vector2f(57 * 50, 1 * 50), BlasterAimDirection::LEFT);
		kamadomaManager.spawnKamadoma(position + sf::Vector2f(22 * 50, 7 * 50), true);
		kamadomaManager.spawnKamadoma(position + sf::Vector2f(24 * 50, 2 * 50), true);
		kamadomaManager.spawnKamadoma(position + sf::Vector2f(65 * 50, 4 * 50), true);
		kamadomaManager.spawnKamadoma(position + sf::Vector2f(71 * 50, 9 * 50), true);
		break;
	case 8:
		suzyManager.spawnSuzy(position + sf::Vector2f(3 * 50, 4 * 50), true);
		suzyManager.spawnSuzy(position + sf::Vector2f(6 * 50, 11 * 50), false);
		suzyManager.spawnSuzy(position + sf::Vector2f(11 * 50, 3 * 50), true);
		suzyManager.spawnSuzy(position + sf::Vector2f(17 * 50, 7 * 50), false);
		break;
	case 9:
		kamadomaManager.spawnKamadoma(position + sf::Vector2f(17 * 50, 7 * 50), false);
		break;
	case 10:
		kamadomaManager.spawnKamadoma(position + sf::Vector2f(9 * 50, 5 * 50), true);
		kamadomaManager.spawnKamadoma(position + sf::Vector2f(17 * 50, 5 * 50), true);
		break;
	case 11:
		itemManager.spawnItem(position + sf::Vector2f(12.5f * 50, 2 * 50), 4);
		break;
	}
}

void Sciman::updateEnemies(float dt)
{
	cutterTimeTracker += dt;
	if (tileManager.checkCutterTrigger() && cutterTimeTracker >= .5f)
	{
		superCutterManager.spawnCutter(tileManager.getSuperCutterSpawnPoint(), tileManager.getSuperCutterDirection(player));
		cutterTimeTracker = 0;
	}

	suzyManager.update(dt, player);
	blasterManager.update(dt, player);
	kamadomaManager.update(dt, player, window->getView());
	superCutterManager.updateCutters(dt, player);
}

void Sciman::renderEnemies(sf::RenderWindow* window)
{
	suzyManager.render(window);
	blasterManager.render(window);
	kamadomaManager.render(window);
	superCutterManager.render(window);
}