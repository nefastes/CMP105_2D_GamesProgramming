#include "SuzyManager.h"
SuzyManager::SuzyManager()
{
	aliveSprites = 0;
	suzyTex.loadFromFile("custom_sprites/Suzy.PNG");
	tileManager = nullptr;
}

SuzyManager::~SuzyManager()
{

}

void SuzyManager::spawnSuzy(sf::Vector2f spawnPoint, bool vertical)
{
	for (unsigned i = 0; i < suzies.size(); ++i)
	{
		if (!suzies[i].isAlive())
		{
			suzies[i].setPosition(spawnPoint);
			suzies[i].setAxis(vertical);
			suzies[i].setAlive(true);
			aliveSprites += 1;
			return;
		}
	}
	suzies.push_back(Suzy());
	suzies[suzies.size() - 1].setAlive(true);
	suzies[suzies.size() - 1].setTexture(&suzyTex);
	suzies[suzies.size() - 1].setSize(sf::Vector2f(50, 50));
	suzies[suzies.size() - 1].setCollisionBox(sf::FloatRect(0, 0, 50, 50));
	suzies[suzies.size() - 1].setTextureRect(sf::IntRect(34, 0, 16, 16));
	suzies[suzies.size() - 1].setPosition(spawnPoint);
	suzies[suzies.size() - 1].setAxis(vertical);
	suzies[suzies.size() - 1].setCollisionBox(sf::FloatRect(0, 0, suzies[suzies.size() - 1].getSize().x, suzies[suzies.size() - 1].getSize().y));
	aliveSprites += 1;
}

void SuzyManager::update(float dt, Player& p)
{
	for (unsigned i = 0; i < suzies.size(); ++i)
	{
		//Check for a player collision
		if (suzies[i].isAlive() && Collision::checkBoundingBox(&suzies[i], &p))
		{
			//TODO: Damage the player
		}

		//Update normally
		if (suzies[i].isAlive())
		{
			suzies[i].update(dt);

			if (!suzies[i].getWallState())
				tileManager->checkSuzyCollision(suzies[i]);

			if (suzies[i].isDebugging())
				suzies[i].updateDebugBoxes();
		}
	}
}

void SuzyManager::render(sf::RenderWindow* window)
{
	for (unsigned i = 0; i < suzies.size(); ++i)
	{
		if (suzies[i].isAlive())
		{
			window->draw(suzies[i]);
			if (suzies[i].isDebugging())
			{
				window->draw(*suzies[i].getDebugCollisionBox());
				window->draw(*suzies[i].getDebugObjectSize());
			}
		}

	}
}

void SuzyManager::killAllSuzies()
{
	for (unsigned i = 0; i < suzies.size(); ++i)
		if (suzies[i].isAlive())
			suzies[i].setAlive(false);
}

void SuzyManager::sendPointers(TileManager* tm)
{
	tileManager = tm;
}

void SuzyManager::setDebugging(bool debug)
{
	for(unsigned i = 0; i < suzies.size(); ++i)
		if (suzies[i].isAlive())
			suzies[i].setDebugging(debug);
}