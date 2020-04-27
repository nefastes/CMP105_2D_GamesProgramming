#include "SuzyManager.h"
SuzyManager::SuzyManager()
{
	aliveSprites = 0;
	suzyTex.loadFromFile("custom_sprites/Suzy.PNG");
	tileManager = nullptr;
	audio = nullptr;
	gameState = nullptr;
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
			suzies[i].resetHealth();
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
	suzies[suzies.size() - 1].resetHealth();
	suzies[suzies.size() - 1].setCollisionBox(sf::FloatRect(0, 0, suzies[suzies.size() - 1].getSize().x, suzies[suzies.size() - 1].getSize().y));
	aliveSprites += 1;
}

void SuzyManager::update(float dt, Player& p)
{
	//Retrieve a vector of all the alive bullets shot by the player
	std::vector<Bullet*> bullets = p.getBullets();

	for (unsigned i = 0; i < suzies.size(); ++i)
	{
		//Check for a player collision
		if (suzies[i].isAlive() && Collision::checkBoundingBox(&suzies[i], &p))
		{
			//TODO: Damage the player
		}

		//Check for a bullet collision
		for (unsigned j = 0; j < bullets.size(); ++j)
		{
			if (suzies[i].isAlive() && bullets[j]->isAlive() && Collision::checkBoundingBox(&suzies[i], bullets[j]))
			{
				suzies[i].damage(1, gameState);			//Pass in gameState so we can add points if the suzy died
				bullets[j]->setAlive(false);
				audio->playSoundbyName("enemyDamage");
			}
		}

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

void SuzyManager::sendPointers(TileManager* tm, AudioManager* aud, GameState* gs)
{
	tileManager = tm;
	audio = aud;
	gameState = gs;
}

void SuzyManager::setDebugging(bool debug)
{
	for(unsigned i = 0; i < suzies.size(); ++i)
		if (suzies[i].isAlive())
			suzies[i].setDebugging(debug);
}