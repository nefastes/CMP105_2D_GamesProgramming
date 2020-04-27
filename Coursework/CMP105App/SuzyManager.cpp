#include "SuzyManager.h"
SuzyManager::SuzyManager()
{
	aliveSprites = 0;
	suzyTex.loadFromFile("custom_sprites/Suzy.PNG");
	tileManager = nullptr;
	itemManager = nullptr;
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
			//First check if the collision does NOT happen on the very edge of both collision boxes,
			//we do not want a collision in this case
			if( p.getCollisionBox().left + p.getCollisionBox().width != suzies[i].getCollisionBox().left &&
				p.getCollisionBox().left != suzies[i].getCollisionBox().left + suzies[i].getCollisionBox().width &&
				p.getCollisionBox().top + p.getCollisionBox().height != suzies[i].getCollisionBox().top &&
				p.getCollisionBox().top != suzies[i].getCollisionBox().top + suzies[i].getCollisionBox().height)
				p.damage(25);
		}

		//Check for a bullet collision
		for (unsigned j = 0; j < bullets.size(); ++j)
		{
			if (suzies[i].isAlive() && bullets[j]->isAlive() && Collision::checkBoundingBox(&suzies[i], bullets[j]))
			{
				suzies[i].damage(1);
				bullets[j]->setAlive(false);
				audio->playSoundbyName("enemyDamage");

				//Check if we killed the suzy
				if (!suzies[i].isAlive())
				{
					//Add score
					gameState->addGlobalScore(500);

					//Drop a random item
					unsigned random = rand() % 100 + 1;
					if (random <= 5)
						itemManager->spawnItem(suzies[i].getPosition(), 3);
					else if (random <= 15)
						itemManager->spawnItem(suzies[i].getPosition(), 2);
					else if (random <= 30)
						itemManager->spawnItem(suzies[i].getPosition(), 1);
					else if (random <= 50)
						itemManager->spawnItem(suzies[i].getPosition(), 0);
				}
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

void SuzyManager::sendPointers(TileManager* tm, ItemManager* im, AudioManager* aud, GameState* gs)
{
	tileManager = tm;
	itemManager = im;
	audio = aud;
	gameState = gs;
}

void SuzyManager::setDebugging(bool debug)
{
	for(unsigned i = 0; i < suzies.size(); ++i)
		if (suzies[i].isAlive())
			suzies[i].setDebugging(debug);
}