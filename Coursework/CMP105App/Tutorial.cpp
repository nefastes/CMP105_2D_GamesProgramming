#include "Tutorial.h"
Tutorial::Tutorial(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Configure level
	currentLevel = Maps::TUTORIAL;

	//Initialise the pointers into the level
	initLevel(hwnd, in, aud, gs, dui);

	//Init hints textures
	hintTex[0].loadFromFile("custom_sprites/Move.PNG");
	hintTex[1].loadFromFile("custom_sprites/Jump.PNG");
	hintTex[2].loadFromFile("custom_sprites/Climb.PNG");
	hintTex[3].loadFromFile("custom_sprites/Spikes.PNG");
	hintTex[4].loadFromFile("custom_sprites/Up.PNG");
	hintTex[5].loadFromFile("custom_sprites/Points.PNG");
	hintTex[6].loadFromFile("custom_sprites/25Health.PNG");
	hintTex[7].loadFromFile("custom_sprites/50Health.PNG");
	hintTex[8].loadFromFile("custom_sprites/Health.PNG");
	hintTex[9].loadFromFile("custom_sprites/Door.PNG");
	hintTex[10].loadFromFile("custom_sprites/Score.PNG");
	hintTex[11].loadFromFile("custom_sprites/Shoot.PNG");
	hintTex[12].loadFromFile("custom_sprites/Win.PNG");
	hintTex[13].loadFromFile("custom_sprites/Pause.PNG");

	//Init hints
	hints[0].setTexture(&hintTex[0]);
	hints[0].setSize(sf::Vector2f(200, 100));
	hints[0].setPosition(100, 400);
	hints[1].setTexture(&hintTex[1]);
	hints[1].setSize(sf::Vector2f(200, 100));
	hints[1].setPosition(825, 525);
	hints[2].setTexture(&hintTex[2]);
	hints[2].setSize(sf::Vector2f(200, 100));
	hints[2].setPosition(700, 100);
	hints[3].setTexture(&hintTex[3]);
	hints[3].setSize(sf::Vector2f(200, 100));
	hints[3].setPosition(1325, 400);
	hints[4].setTexture(&hintTex[4]);
	hints[4].setSize(sf::Vector2f(200, 100));
	hints[5].setTexture(&hintTex[5]);
	hints[5].setSize(sf::Vector2f(200, 100));
	hints[6].setTexture(&hintTex[6]);
	hints[6].setSize(sf::Vector2f(200, 100));
	hints[7].setTexture(&hintTex[7]);
	hints[7].setSize(sf::Vector2f(200, 100));
	hints[8].setTexture(&hintTex[8]);
	hints[8].setSize(sf::Vector2f(200, 100));
	hints[9].setTexture(&hintTex[9]);
	hints[9].setSize(sf::Vector2f(200, 100));
	hints[10].setTexture(&hintTex[10]);
	hints[10].setSize(sf::Vector2f(200, 100));
	hints[11].setTexture(&hintTex[11]);
	hints[11].setSize(sf::Vector2f(200, 100));
	hints[12].setTexture(&hintTex[12]);
	hints[12].setSize(sf::Vector2f(200, 100));
	hints[13].setTexture(&hintTex[13]);
	hints[13].setSize(sf::Vector2f(200, 100));
}

Tutorial::~Tutorial()
{

}

void Tutorial::handleInput(float dt)
{
	//handle the level inputs
	handleLevelInput(dt);

	//Add any level specific input handles here

}

void Tutorial::update(float dt)
{	
	//Update the level
	updateLevel(dt);

	//Add any level specific updates here

}

// Render level
void Tutorial::render()
{
	//DO NOT ADD anything else than this, use virtuals so you can get the order of display right

	//Render the level
	renderLevel();
}

void Tutorial::spawnItemsInRoom(sf::Vector2f position)
{
	//Spawn items & hints corresponding to the current map section
	itemManager.killAllItems();
	if (currentMap == 0)
	{
		hints[0].setPosition(position + sf::Vector2f(2 * 50, 8 * 50));
		hints[1].setPosition(position + sf::Vector2f(16.5f * 50, 10.5f * 50));
		hints[2].setPosition(position + sf::Vector2f(14 * 50, 2 * 50));
		hints[3].setPosition(position + sf::Vector2f(26.5f * 50, 8 * 50));

	}
	else if (currentMap == 1)
	{
		//This section also has items
		itemManager.spawnItem(position + sf::Vector2f(6.25f * 50, 7 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(9.25f * 50, 3 * 50), 1);
		itemManager.spawnItem(position + sf::Vector2f(15 * 50, 3 * 50), 2);
		itemManager.spawnItem(position + sf::Vector2f(18 * 50, 7 * 50), 3);

		hints[4].setPosition(position + sf::Vector2f(16.5f * 50, 5 * 50));
		hints[5].setPosition(position + sf::Vector2f(4.5f * 50, 5 * 50));
		hints[6].setPosition(position + sf::Vector2f(7.5f * 50, 1 * 50));
		hints[7].setPosition(position + sf::Vector2f(13.5f * 50, 1 * 50));

	}
	else if (currentMap == 2)
	{
		hints[8].setPosition(position + sf::Vector2f(3 * 50, 1.5f * 50));
		hints[9].setPosition(position + sf::Vector2f(18* 50, 8 * 50));
	}
	else if (currentMap == 3)
	{
		itemManager.spawnItem(position + sf::Vector2f(4 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(8 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(12 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(16 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(20 * 50, 10 * 50), 0);

		hints[10].setPosition(position + sf::Vector2f(10 * 50, 1 * 50));
	}
	else
	{
		itemManager.spawnItem(position + sf::Vector2f(16 * 50, 2 * 50), 4);
		hints[11].setPosition(position + sf::Vector2f(3 * 50, 8 * 50));
		hints[12].setPosition(position + sf::Vector2f(17 * 50, 8 * 50));
		hints[13].setPosition(position + sf::Vector2f(10 * 50, 4 * 50));
	}
}

void Tutorial::renderEnemies(sf::RenderWindow* window)
{
	//Draw hints
	switch (currentMap)
	{
	case 0:
		window->draw(hints[0]);
		window->draw(hints[1]);
		window->draw(hints[2]);
		window->draw(hints[3]);
		break;
	case 1:
		window->draw(hints[4]);
		window->draw(hints[5]);
		window->draw(hints[6]);
		window->draw(hints[7]);
		break;
	case 2:
		window->draw(hints[8]);
		window->draw(hints[9]);
		break;
	case 3:
		window->draw(hints[10]);
		break;
	case 4:
		window->draw(hints[11]);
		window->draw(hints[12]);
		window->draw(hints[13]);
		break;
	default:
		break;
	}
}

void Tutorial::handlePause(float dt)
{
	//The level class handles the pause for us
	handleLevelPause(dt);
}