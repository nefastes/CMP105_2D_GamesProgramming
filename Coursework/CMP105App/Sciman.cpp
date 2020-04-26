#include "Sciman.h"
Sciman::Sciman(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Configure level
	currentLevel = Maps::SCIMAN;

	//Initialise the pointers into the level
	initLevel(hwnd, in, aud, gs, dui);
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
}

void Sciman::render()
{
	//Render the level
	renderLevel();

	//Add any level specific renders here

}

void Sciman::handlePause(float dt)
{
	//The level class handles the pause for us
	handleLevelPause(dt);
}

void Sciman::spawnItemsInRoom(sf::Vector2f position)
{
	//Spawn at level specific items locations

}