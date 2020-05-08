#include "BlasterBullet.h"
BlasterBullet::BlasterBullet()
{
	direction = sf::Vector2f(-1, 0);
}

BlasterBullet::~BlasterBullet()
{

}

void BlasterBullet::update(float dt)
{
	//Move the particle to the give direction
	move(direction * 400.f * dt);
}

void BlasterBullet::setDirection(sf::Vector2f d)
{
	direction = d;
}