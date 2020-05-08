#include "SuzyManager.h"
SuzyManager::SuzyManager()
{
	suzyTex.loadFromFile("custom_sprites/Suzy.PNG");
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
			suzies[i].setTexture(&suzyTex);
			suzies[i].setTextureRect(sf::IntRect(34, 0, 16, 16));
			suzies[i].resetHealth();
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
	suzies[suzies.size() - 1].resetHealth();
}

void SuzyManager::update(float dt, Player& p)
{
	for (unsigned i = 0; i < suzies.size(); ++i)
	{
		//Check collisions, bullet hits, etc.
		updateEnemy(suzies[i], p, 25, 500);

		//Update no matter what, we check if she is alive inside (for the death frame)
		suzies[i].update(dt);
		//Update states
		if (suzies[i].isAlive())
		{
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
		if (suzies[i].isAlive() || suzies[i].getDying())
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

void SuzyManager::setDebugging(bool debug)
{
	for(unsigned i = 0; i < suzies.size(); ++i)
		if (suzies[i].isAlive())
			suzies[i].setDebugging(debug);
}