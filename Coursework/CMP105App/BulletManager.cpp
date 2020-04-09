#include "BulletManager.h"
BulletManager::BulletManager()
{
	//Load texture
	bulletTex.loadFromFile("custom_sprites/Megaman_bullet.PNG");

	//Init the bullet objects
	for (unsigned i = 0; i < 3; ++i)
	{
		bullets[i] = Bullet();
		bullets[i].setAlive(false);
		bullets[i].setTexture(&bulletTex);
		bullets[i].setSize(sf::Vector2f(32, 32));
		bullets[i].setOrigin(sf::Vector2f(16, 16));
	}
}

BulletManager::~BulletManager()
{

}

void BulletManager::spawnBullet(sf::Vector2f spawnpoint, bool right)
{
	for (unsigned i = 0; i < 3; ++i)
	{
		if (!bullets[i].isAlive())
		{
			bullets[i].setAlive(true);
			bullets[i].setPosition(spawnpoint);
			bullets[i].setBulletDirection(right);
			audio->playSoundbyName("shoot");
			return;
		}
	}
}

void BulletManager::update(float dt)
{
	//If not dead, update the bullet's position
	for (unsigned i = 0; i < 3; ++i)
		if (bullets[i].isAlive())
			bullets[i].update(dt);
}

void BulletManager::renderBullets(sf::RenderWindow* window)
{
	//Death check if the bullet is outside of the view
	for (unsigned i = 0; i < 3; ++i)
		if (bullets[i].getPosition().x < window->getView().getCenter().x - window->getView().getSize().x / 2 ||
			bullets[i].getPosition().x > window->getView().getCenter().x + window->getView().getSize().x / 2)
			bullets[i].setAlive(false);

	//Draw alive bullets
	for (unsigned i = 0; i < 3; ++i)
		if (bullets[i].isAlive())
			window->draw(bullets[i]);
}

void BulletManager::killAllBullets()
{
	for (unsigned i = 0; i < 3; ++i)
		bullets[i].setAlive(false);
}