#include "Suzy.h"
Suzy::Suzy()
{
	isOnWall = false;
	moveVertically = false;
	maxHealth = 15;
	resetHealth();
	setVelocity(200, 200);
}

Suzy::~Suzy()
{

}

void Suzy::update(float dt)
{
	if (isAlive())
	{
		if (isOnWall)
		{
			timePassedTracker += dt;
			setTextureRect(sf::IntRect(17, 0, 16, 16));
			if (timePassedTracker >= .5f)
			{
				setTextureRect(sf::IntRect(34, 0, 16, 16));
				if (timePassedTracker >= 2.5f)
				{
					setTextureRect(sf::IntRect(17, 0, 16, 16));
					if (timePassedTracker >= 3.f)
					{
						setTextureRect(sf::IntRect(0, 0, 16, 16));
						isOnWall = false;
						timePassedTracker = 0;
					}
				}
			}
		}
		//Cant put it as a else or he would never be updated
		if (!isOnWall)
		{
			if (!moveVertically)
				move(sf::Vector2f(velocity.x, 0) * dt);
			else
				move(sf::Vector2f(0, velocity.y) * dt);
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

void Suzy::setAxis(bool vertical)
{
	moveVertically = vertical;
}

void Suzy::setWallState(bool wall)
{
	isOnWall = wall;
}

bool Suzy::getWallState()
{
	return isOnWall;
}

bool Suzy::isMovingVertically()
{
	return moveVertically;
}