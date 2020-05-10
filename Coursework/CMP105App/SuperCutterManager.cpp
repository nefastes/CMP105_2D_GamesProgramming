#include "SuperCutterManager.h"
SuperCutterManager::SuperCutterManager()
{
	cutterTex.loadFromFile("custom_sprites/SuperCutter.PNG");
}

SuperCutterManager::~SuperCutterManager()
{

}

void SuperCutterManager::spawnCutter(sf::Vector2f spawnPoint, float direction)
{
	for (unsigned i = 0; i < cutters.size(); ++i)
	{
		if (!cutters[i].isAlive())
		{
			cutters[i].setPosition(spawnPoint);
			cutters[i].setAlive(true);
			cutters[i].setTexture(&cutterTex);
			cutters[i].setTextureRect(sf::IntRect(0, 0, 20, 20));
			cutters[i].resetHealth();
			cutters[i].setVelocity(direction, CUTTER_VERTICAL_VEL);
			return;
		}
	}
	cutters.push_back(SuperCutter());
	cutters[cutters.size() - 1].setAlive(true);
	cutters[cutters.size() - 1].setTexture(&cutterTex);
	cutters[cutters.size() - 1].setSize(sf::Vector2f(50, 50));
	cutters[cutters.size() - 1].setCollisionBox(sf::FloatRect(5, 0, 45, 50));
	cutters[cutters.size() - 1].setTextureRect(sf::IntRect(0, 0, 20, 20));
	cutters[cutters.size() - 1].setPosition(spawnPoint);
	cutters[cutters.size() - 1].resetHealth();
	cutters[cutters.size() - 1].setVelocity(direction, CUTTER_VERTICAL_VEL);
}

void SuperCutterManager::updateCutters(float dt, Player& p)
{
	for (unsigned i = 0; i < cutters.size(); ++i)
	{
		//Update no matter what (when in the level), we check if she is alive inside (for the death frame)
		cutters[i].update(dt);
		//Update states
		if (cutters[i].isAlive())
		{
			//Check collisions, bullet hits, etc.
			updateEnemy(cutters[i], p, 12, 500);

			if (cutters[i].isDebugging())
				cutters[i].updateDebugBoxes();
		}
	}
}

void SuperCutterManager::render(sf::RenderWindow* window)
{
	for (unsigned i = 0; i < cutters.size(); ++i)
	{
		//Death check (dies if it goes under the screen)
		if (cutters[i].getPosition().y > window->getView().getCenter().y + window->getView().getSize().y / 2.f)
			cutters[i].setAlive(false);

		//Render alive cutters
		if (cutters[i].isAlive())
		{
			window->draw(cutters[i]);
			if (cutters[i].isDebugging())
			{
				window->draw(*cutters[i].getDebugObjectSize());
				window->draw(*cutters[i].getDebugCollisionBox());
			}
		}
	}
}

void SuperCutterManager::killAllCutters()
{
	for (unsigned i = 0; i < cutters.size(); ++i)
		if (cutters[i].isAlive())
			cutters[i].setAlive(false);
}

void SuperCutterManager::setDebugging(bool debug)
{
	for (unsigned i = 0; i < cutters.size(); ++i)
		if (cutters[i].isAlive())
			cutters[i].setDebugging(debug);
}