#include "Player.h"
Player::Player()
{
	//Walk anim
	walk.addFrame(sf::IntRect(0, 0, 64, 64));
	walk.addFrame(sf::IntRect(64, 0, 64, 64));
	walk.setFrameSpeed(1.f / 10.f);

	//Idle anim
	idle.addFrame(sf::IntRect(0, 0, 64, 64));
	idle.addFrame(sf::IntRect(0, 64, 64, 64));
	idle.setFrameSpeed(1.f / 10.f);

	//Physics
	sScale = 40.f;
	gravity = sf::Vector2f(0, 9.8 * sScale);
	stepVelocity = sf::Vector2f(0, 0);

	//Others
	isMoving = false;
	isOnGround = false;
	window = nullptr;
}

Player::~Player()
{

}

void Player::update(float dt)
{
	//Animation
	if (isMoving)
	{
		walk.animate(dt);
		setTextureRect(walk.getCurrentFrame());
	}
	else
	{
		idle.animate(dt);
		setTextureRect(idle.getCurrentFrame());
	}

	//Physics
	sf::Vector2f pos = stepVelocity * dt + 0.5f * gravity * dt;
	stepVelocity += gravity * dt;
	setPosition(getPosition() + pos);
	if (getPosition().y >= window->getSize().y - getSize().y / 2)
	{
		stepVelocity = sf::Vector2f(0, 0);
		setPosition(getPosition().x, window->getSize().y - getSize().y / 2);
		isOnGround = true;
	}
	else isOnGround = false;
}

void Player::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::Right) || input->isKeyDown(sf::Keyboard::D))
	{
		//walk
		isMoving = true;
		move(velocity.x * dt, 0);
		walk.setFlipped(true);
		idle.setFlipped(true);
	}
	else if (input->isKeyDown(sf::Keyboard::Left) || input->isKeyDown(sf::Keyboard::A))
	{
		//walk but reversed
		isMoving = true;
		move(-velocity.x * dt, 0);
		walk.setFlipped(false);
		idle.setFlipped(false);
	}
	else
	{
		//idle
		isMoving = false;
	}

	if (input->isKeyDown(sf::Keyboard::Space) && isOnGround)
	{
		stepVelocity = sf::Vector2f(0, -600.f);
		isOnGround = false;
	}
}