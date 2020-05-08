#include "BlasterManager.h"
BlasterManager::BlasterManager()
{
	blasterTex.loadFromFile("custom_sprites/Blaster.PNG");
}

BlasterManager::~BlasterManager()
{

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
			blasters[i].setBlasterDirection(bad);
			switch (bad)
			{
			case BlasterAimDirection::LEFT:			blasters[i].setTextureRect(sf::IntRect(51, 0, 16, 16));			break;
			case BlasterAimDirection::RIGHT:		blasters[i].setTextureRect(sf::IntRect(67, 0, -16, 16));		break;
			}
			return;
		}
	}
	blasters.push_back(Blaster());
	blasters[blasters.size() - 1].setAlive(true);
	blasters[blasters.size() - 1].setTexture(&blasterTex);
	blasters[blasters.size() - 1].setSize(sf::Vector2f(50, 50));
	blasters[blasters.size() - 1].setCollisionBox(sf::FloatRect(22, 0, 28, 50));
	blasters[blasters.size() - 1].setPosition(spawnPoint);
	blasters[blasters.size() - 1].resetHealth();
	blasters[blasters.size() - 1].setBlasterDirection(bad);
	switch (bad)
	{
	case BlasterAimDirection::LEFT:			blasters[blasters.size() - 1].setTextureRect(sf::IntRect(51, 0, 16, 16));		break;
	case BlasterAimDirection::RIGHT:		blasters[blasters.size() - 1].setTextureRect(sf::IntRect(67, 0, -16, 16));		break;
	}
}

void BlasterManager::update(float dt, Player& p)
{
	for (unsigned i = 0; i < blasters.size(); ++i)
	{
		//Check collisions, etc.
		updateEnemy(blasters[i], p, 5, 200);

		//Check collision with the blaster bullets
		std::vector<BlasterBullet*> aliveBlasterBullets = blasters[i].getAilveBullets();
		for (unsigned j = 0; j < aliveBlasterBullets.size(); ++j)
			if (Collision::checkBoundingBox(aliveBlasterBullets[j], &p))
				p.damage(5);

		//Update no matter what, we check if it is alive inside (for the death frame)
		blasters[i].update(dt, audio);
	}
}

void BlasterManager::render(sf::RenderWindow* window)
{
	for (unsigned i = 0; i < blasters.size(); ++i)
	{
		if (blasters[i].isAlive() || blasters[i].getDying())
		{
			window->draw(blasters[i]);
			blasters[i].renderAliveBullets(window);
			if (blasters[i].isDebugging())
			{
				window->draw(*blasters[i].getDebugCollisionBox());
				window->draw(*blasters[i].getDebugObjectSize());
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
			blasters[i].setDebugging(debug);
}