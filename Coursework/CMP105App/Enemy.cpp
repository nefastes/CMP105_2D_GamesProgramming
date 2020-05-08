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