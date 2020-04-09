#include "Bullet.h"
Bullet::Bullet()
{
	isFacingRight = false;
}

Bullet::~Bullet()
{

}

void Bullet::update(float dt)
{
	if (isFacingRight)
		move(sf::Vector2f(600, 0) * dt);
	else
		move(sf::Vector2f(-600, 0) * dt);
}

void Bullet::setBulletDirection(bool right)
{
	isFacingRight = right;
}