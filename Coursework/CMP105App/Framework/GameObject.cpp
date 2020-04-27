#include "GameObject.h"

GameObject::GameObject()
{
	//Init global debug infos
	debugging = false;
	debugSize.setSize(sf::Vector2f(getSize()));
	debugSize.setOutlineColor(sf::Color::White);
	debugSize.setOutlineThickness(1);
	debugSize.setFillColor(sf::Color::Transparent);
	debugCollisionBox.setSize(sf::Vector2f(getCollisionBox().width, getCollisionBox().height));
	debugCollisionBox.setOutlineColor(sf::Color::Red);
	debugCollisionBox.setFillColor(sf::Color::Transparent);
	debugCollisionBox.setOutlineThickness(1);

	//Init input pointer
	input = nullptr;
}

GameObject::~GameObject()
{

}

// Override this function to provide input handling for the derived class
void GameObject::handleInput(float dt)
{
}

// override this function to provide functionality
void GameObject::update(float dt)
{
}

// Sets the velocity of the sprite
void GameObject::setVelocity(sf::Vector2f vel)
{
	velocity = vel;
}
void GameObject::setVelocity(float vx, float vy)
{
	velocity.x = vx;
	velocity.y = vy;
}

// get sprite velocity
sf::Vector2f GameObject::getVelocity()
{
	return velocity;
}

// Returns collision box + position, giving the collision box in the correct position
sf::FloatRect GameObject::getCollisionBox() {
	
	return sf::FloatRect(collisionBox.left + getPosition().x, collisionBox.top + getPosition().y, collisionBox.width, collisionBox.height); 
}

// Reponse function, what the sprite does based on collision
// Colliding object is passed in for information
// e.g. compare sprite positions to determine new velocity direction.
// e.g. checking sprite type (world, enemy, bullet etc) so response is based on that.
void GameObject::collisionResponse(GameObject * collider)
{
}

void GameObject::setCollisionBoxColor(sf::Color color)
{
	debugCollisionBox.setOutlineColor(color);
}

void GameObject::updateDebugBoxes()
{
	debugSize.setSize(sf::Vector2f(getSize()));
	debugSize.setPosition(getPosition());
	debugCollisionBox.setSize(sf::Vector2f(getCollisionBox().width, getCollisionBox().height));
	debugCollisionBox.setPosition(getCollisionBox().left, getCollisionBox().top);
}