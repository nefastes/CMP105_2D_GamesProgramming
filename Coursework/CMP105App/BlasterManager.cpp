#include "BlasterManager.h"
BlasterManager::BlasterManager()
{
	blasterTex.loadFromFile("custom_sprites/Blaster.PNG");
}

BlasterManager::~BlasterManager()
{

}

void BlasterManager::setSpriteDirection(BlasterAimDirection bad, unsigned location)
{
	blasters[location].setBlasterDirection(bad);
	switch (bad)
	{
	case BlasterAimDirection::LEFT:
		blasters[location].setTextureRect(sf::IntRect(51, 0, 16, 16));
		blasters[location].setCollisionBox(sf::FloatRect(22, 0, 28, 50));
		break;
	case BlasterAimDirection::RIGHT:
		blasters[location].setTextureRect(sf::IntRect(67, 0, -16, 16));
		blasters[location].setCollisionBox(sf::FloatRect(0, 0, 28, 50));
		break;
	}
}

void BlasterManager::spawnBlaster(sf::Vector2f spawnPoint, BlasterAimDirection bad)
{
	for (unsigned i = 0; i < blasters.size(); ++i)
	{
		if (!blasters[i].isAlive())
		{
			blasters[i].setPosition(spawnPoint);
			blasters[i].setAlive(true);
			blasters[i].setTexture(&blasterTex);
			blasters[i].resetHealth();
			setSpriteDirection(bad, i);
			return;
		}
	}
	blasters.push_back(Blaster());
	blasters[blasters.size() - 1].setAlive(true);
	blasters[blasters.size() - 1].setTexture(&blasterTex);
	blasters[blasters.size() - 1].setSize(sf::Vector2f(50, 50));
	blasters[blasters.size() - 1].setPosition(spawnPoint);
	blasters[blasters.size() - 1].resetHealth();
	setSpriteDirection(bad, blasters.size() - 1);
}

void BlasterManager::update(float dt, Player& p)
{
	for (unsigned i = 0; i < blasters.size(); ++i)
	{
		if (blasters[i].isAlive())
		{
			//Check player collisions, etc.
			updateEnemy(blasters[i], p, 10, 200);
		}

		//Check collision with the blaster bullets (outside of isAlive cause he might die after having fired bullets
		std::vector<BlasterBullet*> aliveBlasterBullets = blasters[i].getAilveBullets();
		for (unsigned j = 0; j < aliveBlasterBullets.size(); ++j)
			if (Collision::checkBoundingBox(aliveBlasterBullets[j], &p))
				p.damage(10);

		//Update no matter what (in the level), we check if it is alive inside (for the death frame)
		blasters[i].update(dt, audio);
	}
}

void BlasterManager::render(sf::RenderWindow* window)
{
	for (unsigned i = 0; i < blasters.size(); ++i)
	{
		//Render bullets separately cause the blaster might dies whille bullets are still alive
		if(blasters[i].getAilveBullets().size() > 0)
			blasters[i].renderAliveBullets(window, tileManager);

		if (blasters[i].isAlive() || blasters[i].getDying())
		{
			window->draw(blasters[i]);
			if (blasters[i].isDebugging())
			{
				window->draw(*blasters[i].getDebugObjectSize());
				window->draw(*blasters[i].getDebugCollisionBox());
			}
		}
	}
}

void BlasterManager::killAllBlasters()
{
	for (unsigned i = 0; i < blasters.size(); ++i)
		if (blasters[i].isAlive())
			blasters[i].setAlive(false);
}

void BlasterManager::setDebugging(bool debug)
{
	for (unsigned i = 0; i < blasters.size(); ++i)
		if (blasters[i].isAlive())
		{
			blasters[i].setDebugging(debug);
			blasters[i].updateDebugBoxes();
			blasters[i].getBulletManager()->setDebugging(debug);
		}
}