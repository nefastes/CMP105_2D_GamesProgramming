#include "Player.h"
Player::Player()
{
	//Walk anim
	walk.addFrame(sf::IntRect(0, 0, 64, 64));
	walk.addFrame(sf::IntRect(64, 0, 64, 64));
	walk.setFrameSpeed(1.f / 6.f);

	//Idle anim
	idle.addFrame(sf::IntRect(0, 0, 64, 64));
	idle.addFrame(sf::IntRect(0, 64, 64, 64));
	idle.setFrameSpeed(1.f / 6.f);

	//Physics
	sScale = 40.f;
	gravity = sf::Vector2f(0, 9.8 * sScale);
	stepVelocity = sf::Vector2f(0, 0);

	//Others
	isMoving = false;
	isOnGround = false;
	hasEnteredRoom = false;
	cutsceneTracker = 0;
	backFrontLayer = true;
	allowControls = false;
	window = nullptr;

	//Player pos init
	setPosition(175, 635);
	setTextureRect(sf::IntRect(0, 0, 64, 64));
}

Player::~Player()
{

}

void Player::update(float dt)
{
	//Animation
	if (!hasEnteredRoom)
	{
		timeTracker += dt;
		if (timeTracker >= .5f && timeTracker < 1 && cutsceneTracker < 6)
		{
			move(0, -25);
			timeTracker = 0;
			++cutsceneTracker;
		}
		switch (cutsceneTracker)
		{
		case 6:
			backFrontLayer = false;
			if (timeTracker > 1)
			{
				++cutsceneTracker;
				timeTracker = 0;
			}
			break;

		case 7:
			walk.animate(dt);
			walk.setFlipped(true);
			setTextureRect(walk.getCurrentFrame());
			move(0, 100 * dt);
			if (getPosition().y >= 550)
				++cutsceneTracker;
			break;

		case 8:
			walk.animate(dt);
			setTextureRect(walk.getCurrentFrame());
			move(100 * dt, 0);
			if (getPosition().x >= 450)
			{
				hasEnteredRoom = true;
				idle.setFlipped(true);
			}
			break;

		default:
			break;
		}
		
		
	}
	else
	{
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
		if (getPosition().y >= 550)
		{
			stepVelocity = sf::Vector2f(0, 0);
			setPosition(getPosition().x, 550);
			isOnGround = true;
		}
		else isOnGround = false;
	}
}

void Player::handleInput(float dt)
{
	if (allowControls)
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
}