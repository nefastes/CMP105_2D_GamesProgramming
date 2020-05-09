#include "Blaster.h"
Blaster::Blaster()
{
	//Enemy specific health
	maxHealth = 3;
	resetHealth();
	blasterDirection = BlasterAimDirection::LEFT;
	currentMode = 0;
	previousMode = currentMode;
	//generates a number between 3.0f and 6.0f found on stackoverflow
	rngWaitTime = 3 + static_cast<float> (std::rand() / static_cast<float> (RAND_MAX / (6 - 3)));
}

Blaster::~Blaster()
{

}

void Blaster::update(float dt, AudioManager* audio)
{
	//Update his bullets even if he's dead
	bulletManager.update(dt);

	//Enemy specific updates
	if (isAlive())
	{
		//Update time tracker and change mode every .5s
		timePassedTracker += dt;
		if (timePassedTracker >= .5f && currentMode > 0 || timePassedTracker >= rngWaitTime && currentMode == 0)
		{
			timePassedTracker = 0;
			//Upgrade the current mode
			if (currentMode < 8) ++currentMode;
			else
			{
				currentMode = 0;
				rngWaitTime = 3 + static_cast<float> (std::rand() / static_cast<float> (RAND_MAX / (6 - 3)));
			}

			//Shot on mode 4, 5 and 6, that way it only spawns once
			if (currentMode == 4 || currentMode == 5 || currentMode == 6)
			{
				audio->playSoundbyName("blasterShoot");
				switch (blasterDirection)
				{
				case BlasterAimDirection::LEFT:
					bulletManager.spawn3Bullets(sf::Vector2f(getPosition().x, getPosition().y + getSize().y / 2.f));
					break;
				case BlasterAimDirection::RIGHT:
					bulletManager.spawn3Bullets(sf::Vector2f(getPosition().x + getSize().x, getPosition().y + getSize().y / 2.f));
					break;
				}
			}

			//Check invincibility
			if (currentMode == 0) invincibility = true;
			else invincibility = false;

			//Change the texture accordingly
			switch (currentMode)
			{
			case 0:
				switch (blasterDirection)
				{
				case BlasterAimDirection::LEFT:			setTextureRect(sf::IntRect(51, 0, 16, 16));			break;
				case BlasterAimDirection::RIGHT:		setTextureRect(sf::IntRect(67, 0, -16, 16));		break;
				}
				break;
			case 1:
			case 8:
				switch (blasterDirection)
				{
				case BlasterAimDirection::LEFT:			setTextureRect(sf::IntRect(34, 0, 16, 16));			break;
				case BlasterAimDirection::RIGHT:		setTextureRect(sf::IntRect(50, 0, -16, 16));		break;
				}
				break;
			case 2:
			case 7:
				switch (blasterDirection)
				{
				case BlasterAimDirection::LEFT:			setTextureRect(sf::IntRect(17, 0, 16, 16));			break;
				case BlasterAimDirection::RIGHT:		setTextureRect(sf::IntRect(33, 0, -16, 16));		break;
				}
				break;
			case 3:
			case 4:
			case 5:
			case 6:
				switch (blasterDirection)
				{
				case BlasterAimDirection::LEFT:			setTextureRect(sf::IntRect(0, 0, 16, 16));			break;
				case BlasterAimDirection::RIGHT:		setTextureRect(sf::IntRect(16, 0, -16, 16));		break;
				}
				break;
			}
		}
	}
	else
	{
		timePassedTracker += dt;
		if (timePassedTracker >= .2f)
		{
			isDying = false;
			timePassedTracker = 0;
		}
	}
}

void Blaster::setBlasterDirection(BlasterAimDirection d)
{
	blasterDirection = d;
	bulletManager.setBlasterDirection(d);
}
BlasterAimDirection Blaster::getBlasterDirection()
{
	return blasterDirection;
}

void Blaster::renderAliveBullets(sf::RenderWindow* window, TileManager* tm)
{
	bulletManager.renderBlasterBullets(window, tm);
}

std::vector<BlasterBullet*> Blaster::getAilveBullets()
{
	return bulletManager.getAliveBullets();
}

BlasterBulletManager* Blaster::getBulletManager()
{
	return &bulletManager;
}