#include "BlasterBulletManager.h"
BlasterBulletManager::BlasterBulletManager()
{
	blasterBulletTex.loadFromFile("custom_sprites/Blaster_Bullet.PNG");

	blasterDirection = BlasterAimDirection::LEFT;
	bulletDirection = sf::Vector2f(-1, 0);

	for (unsigned i = 0; i < 9; ++i)
	{
		blasterBullets[i] = BlasterBullet();
		//Strangely, textures have to be initialised somewhere else or the pointer will be corrupted
		blasterBullets[i].setSize(sf::Vector2f(25, 25));
		blasterBullets[i].setOrigin(12.5f, 12.5f);
		blasterBullets[i].setCollisionBox(sf::FloatRect(-12.5f, -12.5f, 25, 25));
		blasterBullets[i].setAlive(false);
	}
}

BlasterBulletManager::~BlasterBulletManager()
{

}

void BlasterBulletManager::spawn3Bullets(sf::Vector2f spawnPoint)
{
	for (unsigned j = 0; j < 3; ++j)
	{
		for (unsigned i = 0; i < 9; ++i)
		{
			if (!blasterBullets[i].isAlive())
			{
				blasterBullets[i].setAlive(true);
				blasterBullets[i].setPosition(spawnPoint);
				switch (blasterDirection)
				{
				case BlasterAimDirection::LEFT:
					if (j == 0)				blasterBullets[i].setDirection(sf::Vector2f(-1, -1));
					else if (j == 1)		blasterBullets[i].setDirection(sf::Vector2f(-1, 0));
					else if (j == 2)		blasterBullets[i].setDirection(sf::Vector2f(-1, 1));
					break;
				case BlasterAimDirection::RIGHT:
					if (j == 0)				blasterBullets[i].setDirection(sf::Vector2f(1, -1));
					else if (j == 1)		blasterBullets[i].setDirection(sf::Vector2f(1, 0));
					else if (j == 2)		blasterBullets[i].setDirection(sf::Vector2f(1, 1));
					break;
				}
				break;
			}
		}
	}
}

void BlasterBulletManager::update(float dt)
{
	for (unsigned i = 0; i < 9; ++i)
	{
		//Need to initialise textures here, apparently there is a weird error with objects inheritance that will corrupt the pointer
		//of the texture, and it happens in this case. I have deeply looked to fix this but could not find anything, so
		//I'm just going to init it here
		if (blasterBullets[i].getTexture() != &blasterBulletTex)
			blasterBullets[i].setTexture(&blasterBulletTex);

		if (blasterBullets[i].isAlive())
		{
			blasterBullets[i].update(dt);

			if (blasterBullets[i].isDebugging())
				blasterBullets[i].updateDebugBoxes();
		}
	}
}

void BlasterBulletManager::renderBlasterBullets(sf::RenderWindow* window, TileManager* tm)
{
	//Death check
	for (unsigned i = 0; i < 9; ++i)
	{
		if (blasterBullets[i].isAlive())
		{
			//Kill a bullet if it goes outside of the current map
			if (blasterBullets[i].getPosition().x - blasterBullets[i].getSize().x <= tm->getMapPosition().x ||
				blasterBullets[i].getPosition().x >= tm->getMapPosition().x + tm->getMapSize().x * 50 ||
				blasterBullets[i].getPosition().y - blasterBullets[i].getSize().y <= tm->getMapPosition().y ||
				blasterBullets[i].getPosition().y >= tm->getMapPosition().y + tm->getMapSize().y * 50)
				blasterBullets[i].setAlive(false);
		}
	}

	//Render alive bullets
	for (unsigned i = 0; i < 9; ++i)
		if (blasterBullets[i].isAlive())
		{
			window->draw(blasterBullets[i]);

			if (blasterBullets[i].isDebugging())
			{
				window->draw(*blasterBullets[i].getDebugObjectSize());
				window->draw(*blasterBullets[i].getDebugCollisionBox());
			}
		}
}

void BlasterBulletManager::setBlasterDirection(BlasterAimDirection d)
{
	blasterDirection = d;
}

std::vector<BlasterBullet*> BlasterBulletManager::getAliveBullets()
{
	std::vector<BlasterBullet*> temp;

	for (unsigned i = 0; i < 9; ++i)
		if (blasterBullets[i].isAlive())
			temp.push_back(&blasterBullets[i]);

	return temp;
}

void BlasterBulletManager::setDebugging(bool debug)
{
	for (unsigned i = 0; i < 9; ++i)
		if (blasterBullets[i].isAlive())
			blasterBullets[i].setDebugging(debug);
}