#include "Player.h"
Player::Player()
{
	//Walk
	walk.addFrame(sf::IntRect(78, 8, 24, 24));
	walk.addFrame(sf::IntRect(94, 8, 24, 24));
	walk.addFrame(sf::IntRect(120, 8, 24, 24));
	walk.setFrameSpeed(1.f / 10.f);

	//Physics
	sScale = 40.f;
	gravity = sf::Vector2f(0, 9.8 * sScale);
	stepVelocity = sf::Vector2f(0, 0);

	//Others
	allowControls = true;
	isMoving = false;
	isOnGround = false;
	isCollidingRight = false;
	isCollidingLeft = false;
	prevTime = 0;
	window = nullptr;
}

Player::~Player()
{

}

void Player::update(float dt)
{
	//Debug
	if (debugging)
	{
		debugSize.setSize(sf::Vector2f(getSize()));
		debugSize.setPosition(getPosition());
		debugCollisionBox.setSize(sf::Vector2f(getCollisionBox().width, getCollisionBox().height));
		debugCollisionBox.setPosition(sf::Vector2f(getCollisionBox().left, getCollisionBox().top));
	}

	//Animation
	if (isMoving)
	{
		walk.animate(dt);
		setTextureRect(walk.getCurrentFrame());
	}

	//Physics
	if (!isOnGround)	//Avoid doing more calculations when touches the ground
	{
		sf::Vector2f pos = stepVelocity * dt + 0.5f * gravity * dt;
		stepVelocity += gravity * dt;
		setPosition(getPosition() + pos);
	}

	//default reset pos
	if (getPosition().y > window->getSize().y + getSize().y) setPosition(sf::Vector2f(300, 100));
}

void Player::handleInput(float dt)
{
	prevTime += dt;
	if (input->isKeyDown(sf::Keyboard::Right) || input->isKeyDown(sf::Keyboard::D))
	{
		if (!isCollidingLeft)	//Prevent going right if it touches the left of a wall
		{
			//walk
			isMoving = true;
			move(velocity.x * dt, 0);
			walk.setFlipped(true);
		}
	}
	else if (input->isKeyDown(sf::Keyboard::Left) || input->isKeyDown(sf::Keyboard::A))
	{
		if (!isCollidingRight)	//Prevent going left if it touches the right of a wall
		{
			//walk but reversed
			isMoving = true;
			move(-velocity.x * dt, 0);
			walk.setFlipped(false);
		}
	}
	else
	{
		//idle
		isMoving = false;
	}

	//Jump
	if (input->isKeyDown(sf::Keyboard::Space) && isOnGround)
	{
		stepVelocity = sf::Vector2f(0, -600.f);
		isOnGround = false;
	}
}

void Player::collisionResponse(GameObject* collider)
{
	//Deltas from the center of the collision box of the tile to the center of the collisionbox of the player
	float dx = (collider->getPosition().x + collider->getSize().x / 2) - (getCollisionBox().left + getCollisionBox().width / 2);
	float dy = (collider->getPosition().y + collider->getSize().y / 2) - (getCollisionBox().top + getCollisionBox().height / 2);


	//Y axis hit
	if (std::abs(dx) <= std::abs(dy))
	{
		if (std::abs(dx) < collider->getSize().x / 2 + getCollisionBox().width / 2)	//Need this line to prevent standing on the very edge
		{
			isOnGround = true;
			isCollidingRight = false;
			isCollidingLeft = false;
			stepVelocity.y = 0;
			setPosition(sf::Vector2f(getPosition().x, collider->getPosition().y - getCollisionBox().height - (getSize().y - getCollisionBox().height)));	//Set pos in reguard of the hitbox
		}
	}
	//X axis hit
	else
	{
		//Right side hit
		if (dx < 0)
		{
			isCollidingRight = true;
			isCollidingLeft = false;
			setPosition(sf::Vector2f(collider->getPosition().x + collider->getSize().x - ((getSize().x - getCollisionBox().width) / 2), getPosition().y));	//Set pos in reguard of the hitbox
		}
		//Left side hit
		else
		{
			isCollidingRight = false;
			isCollidingLeft = true;
			setPosition(sf::Vector2f(collider->getPosition().x - (getCollisionBox().width + ((getSize().x - getCollisionBox().width) / 2)), getPosition().y));	//Set pos in reguard of the hitbox
		}
	}
}