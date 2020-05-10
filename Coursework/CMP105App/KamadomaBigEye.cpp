#include "KamadomaBigEye.h"
KamadomaBigEye::KamadomaBigEye()
{
	//Always spawns on the ground
	isOnground = true;
}

KamadomaBigEye::~KamadomaBigEye()
{

}

void KamadomaBigEye::update(float dt, Player& p)
{
	if (isAlive())
	{
		if (isOnground)
		{
			timePassedTracker += dt;
			if (timePassedTracker >= 1.f)
			{
				//Calculate the new direction
				sf::Vector2f direction = p.getPosition() - getPosition();
				
				//The direction will be used a hVel, so we give it a max value so it cant move too much too fast
				//We change it reguarding the type of enemy
				//Kamadoma
				if (getSize().x == 50)
				{
					if (direction.x > 500.f)
						direction.x = 500.f;
					else if (direction.x < -500.f)
						direction.x = -500.f;
				}
				//Big Eye
				else
				{
					if (direction.x > 200.f)
						direction.x = 200.f;
					else if (direction.x < -200.f)
						direction.x = -200.f;
				}

				//If the player is above the enemy, make him jump higher
				if (direction.y < 0)
					setVelocity(direction.x, -900.f + direction.y);
				else
					setVelocity(direction.x, -900.f);

				//Update trackers
				isOnground = false;
				timePassedTracker = 0;
			}
		}
		//Cant put it as a else or he would never be updated
		if (!isOnground)
		{
			float pos = velocity.y * dt + 0.5f * 980.f * dt;
			velocity.y += 980.f * dt;
			setPosition(getPosition().x + velocity.x * dt, getPosition().y + pos);
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

void KamadomaBigEye::setMaxHealth(short int max)
{
	maxHealth = max;
	resetHealth();
}

void KamadomaBigEye::setGroundState(bool gnd)
{
	isOnground = gnd;
}

bool KamadomaBigEye::getGroundState()
{
	return isOnground;
}