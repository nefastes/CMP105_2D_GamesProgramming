#include "DeathParticle.h"
DeathParticle::DeathParticle()
{
	//Set origin the the middle so it aligns with the player sprite nicely (size 64)
	setOrigin(sf::Vector2f(32, 32));

	//default direction is up
	direction = sf::Vector2f(0, 1);
}

DeathParticle::~DeathParticle()
{

}

void DeathParticle::update(float dt)
{
	//Move the particle to the give direction
	move(direction * 100.f * dt);
}

void DeathParticle::setDirection(sf::Vector2f d)
{
	direction = d;
}

sf::Vector2f DeathParticle::getDirection()
{
	return direction;
}