#include "Enemy.h"
#include "Blaster.h"
Enemy::Enemy()
{
	timePassedTracker = 0;
	maxHealth = 1;
	health = maxHealth;
	isDying = false;
	invincibility = false;
	deathParticleTex.loadFromFile("custom_sprites/Enemy_Death_Particle.PNG");
}

Enemy::~Enemy()
{

}

void Enemy::damage(short int amount)
{
	if(!invincibility)
		health -= amount;
	if (health <= 0)
	{
		//Now dead
		setAlive(false);

		//We still want to display the dead frame for a couple ms
		setTexture(&deathParticleTex);
		if (getSize() != sf::Vector2f(50, 50))
		{
			setPosition(getPosition() + getSize() / 2.f - sf::Vector2f(50, 50) / 2.f);
			setSize(sf::Vector2f(50, 50));
		}
		setTextureRect(sf::IntRect(0, 0, 16, 16));

		//Adjust trackers
		isDying = true;
		timePassedTracker = 0;
	}
}

void Enemy::resetHealth()
{
	health = maxHealth;
}

bool Enemy::getDying()
{
	return isDying;
}

bool Enemy::isInvincible()
{
	return invincibility;
}