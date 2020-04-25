#include "Item.h"
Item::Item()
{
	isOnGround = false;
	id = 0;
	velocity.x = 0;
	velocity.y = 400;
}

Item::~Item()
{

}

void Item::update(float dt)
{
	if (!isOnGround) move(velocity * dt);

	//Update debugging collision boxes
	if (debugging)
	{
		debugSize.setSize(getSize());
		debugSize.setSize(sf::Vector2f(getSize()));
		debugSize.setPosition(getPosition() - getOrigin());
		debugCollisionBox.setSize(sf::Vector2f(getCollisionBox().width, getCollisionBox().height));
		debugCollisionBox.setPosition(sf::Vector2f(getCollisionBox().left, getCollisionBox().top));
	}
}

void Item::setId(short unsigned Id)
{
	id = Id;
}

short unsigned Item::getId()
{
	return id;
}

void Item::setGrounded(bool ground)
{
	isOnGround = ground;
}

bool Item::isGrounded()
{
	return isOnGround;
}