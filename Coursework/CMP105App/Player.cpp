#include "Player.h"
Player::Player()
{
	//Walk Animation
	walk.addFrame(sf::IntRect(78, 8, 24, 24));
	walk.addFrame(sf::IntRect(104, 8, 24, 24));
	walk.addFrame(sf::IntRect(130, 8, 24, 24));
	walk.addFrame(sf::IntRect(104, 8, 24, 24));
	walk.setFrameSpeed(1.f / 10.f);

	//Climb animation
	climb.addFrame(sf::IntRect(222, 0, 16, 30));
	climb.addFrame(sf::IntRect(238, 0, -16, 30));
	climb.setFrameSpeed(1.f / 10.f);

	//Physics
	sScale = 100.f;
	gravity = sf::Vector2f(0, 9.8 * sScale);
	stepVelocity = sf::Vector2f(0, 0);

	//Player trackers
	allowControls = true;
	isMoving = false;
	isOnGround = false;
	isJumping = false;
	allowJump = true;
	isCollidingRight = false;
	isCollidingLeft = false;

	//Ladder trackers
	isLadderAvailable = false;
	isClimbing = false;
	isClimbingDownwards = false;
	isOnLadder = false;

	//General trackers
	timePassedTracker = 0;
	jumpKeyPressTracker = 0;

	//Rng
	rng = 0;
	isRngSet = false;

	//Others
	window = nullptr;
}

Player::~Player()
{

}

void Player::handleInput(float dt)
{
	//Update the time tracker
	timePassedTracker += dt;

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
	if (input->isKeyDown(sf::Keyboard::Space) && allowJump)
	{
		if (!isOnLadder)
		{
			//set the jump tracker, change the player mode to the jump mode (1) and update the jumpTimeTracker for every frame space is pressed
			isJumping = true;
			changePlayerMode(1);
			jumpKeyPressTracker += dt;
		}
		else
		{
			isOnLadder = false;
			changePlayerMode(1);
		}
	}
	else
	{
		isJumping = false;
		jumpKeyPressTracker = 0;
	}

	//Ladder
	if (isLadderAvailable && input->isKeyDown(sf::Keyboard::W))
	{
		isClimbing = true;
		isClimbingDownwards = false;
		isOnLadder = true;
		changePlayerMode(3);
	}
	else if (isLadderAvailable && !isOnGround && input->isKeyDown(sf::Keyboard::S))
	{
		isClimbing = true;
		isClimbingDownwards = true;
		isOnLadder = true;
		changePlayerMode(3);
	}
	else
		isClimbing = false;
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
	if (isMoving && isOnGround)
	{
		walk.animate(dt);
		setTextureRect(walk.getCurrentFrame());
	}
	else if (isOnLadder)
	{
		if(isClimbing)
			climb.animate(dt);
		setTextureRect(climb.getCurrentFrame());
	}
	else if (!isOnGround)
	{
		//If the walk anim was flipped, we have to flip the jump frame rect as well
		if (walk.getFlipped())
			setTextureRect(sf::IntRect(218, 0, -26, 30));
		else
			setTextureRect(sf::IntRect(192, 0, 26, 30));
	}
	else
	{
		//If the walk anim was flipped, we have to flip the non-moving frame texture as well
		if(walk.getFlipped())
			setTextureRect(sf::IntRect(24, 8, -24, 24));
		else
			setTextureRect(sf::IntRect(0, 8, 24, 24));

		//Init the rng for the next step
		if (!isRngSet)
		{
			//generates a number between 2.0f and 5.0f found on stackoverflow
			rng = 2 + static_cast<float> (std::rand() / static_cast<float> (RAND_MAX / (5 - 2)));
			isRngSet = true;
		}

		//If the player doesnt move after a random amount of time between 2 and 5 seconds, we change the texture rect to make megaman blink
		if (timePassedTracker > rng)
		{
			if (walk.getFlipped())
				setTextureRect(sf::IntRect(48, 8, -24, 24));
			else
				setTextureRect(sf::IntRect(24, 8, 24, 24));
			//and .2 seconds later reset to normal sprite
			if (timePassedTracker > rng + .2f)
			{
				if (walk.getFlipped())
					setTextureRect(sf::IntRect(24, 8, -24, 24));
				else
					setTextureRect(sf::IntRect(0, 8, 24, 24));

				//Finally, reset the time tracker and allow rng regeneration
				timePassedTracker = 0;
				isRngSet = false;
			}
		}
	}

	//Physics
	if (!isOnGround && !isJumping && !isOnLadder)	//Avoid doing more calculations when touches the ground or when the player jumps
	{
		//Reset jump stats
		allowJump = false;

		//Apply gravity
		sf::Vector2f pos = stepVelocity * dt + 0.5f * gravity * dt;
		stepVelocity += gravity * dt;
		setPosition(getPosition() + pos);
	}
	if (isJumping && allowJump)
	{
		//Increase the velocity each frame as long as the space key is press. When max vel is reached, reset the jump tracker
		if (jumpKeyPressTracker < .2f)
		{
			stepVelocity.y = -600.f;
			//Set the position relative to this jump vel
			setPosition(getPosition() + stepVelocity * dt);
		}
		else
		{
			isJumping = false;
			allowJump = false;
			//Not on ground anymore
			isOnGround = false;
		}
	}
	if (isClimbing)
	{
		allowJump = true;
		if(isClimbingDownwards)
			move(sf::Vector2f(0, 100.f) * dt);
		else
			move(sf::Vector2f(0, -100.f) * dt);
	}
	if (isOnGround)
		allowJump = true;
}

void Player::collisionResponse(GameObject* collider)
{
	//Deltas from the center of the collision box of the tile to the center of the collisionbox of the player
	float dx = (collider->getPosition().x + collider->getSize().x / 2) - (getCollisionBox().left + getCollisionBox().width / 2);
	float dy = (collider->getPosition().y + collider->getSize().y / 2) - (getCollisionBox().top + getCollisionBox().height / 2);


	//Y axis hit
	if (collider->getTargetname() == "world")
	{
		if (std::abs(dx) <= std::abs(dy))
		{
			//Bottom hit
			if (dy < 0)
			{
				if (std::abs(dx) < collider->getSize().x / 2 + getCollisionBox().width / 2)	//Need this line to prevent standing on the very edge
				{
					stepVelocity = sf::Vector2f(0, 0);
				}
			}
			//Top hit
			else
			{
				if (std::abs(dx) < collider->getSize().x / 2 + getCollisionBox().width / 2)	//Need this line to prevent standing on the very edge
				{
					isOnGround = true;
					isCollidingRight = false;
					isCollidingLeft = false;
					stepVelocity.y = 0;
					isOnLadder = false;					//If he touches the ground, he is not on a ladder anymore
					changePlayerMode(0);				//He is now on ground, so we change the mode back to normal (0)
					setPosition(sf::Vector2f(getPosition().x, collider->getPosition().y - getCollisionBox().height - (getSize().y - getCollisionBox().height)));	//Set pos in reguard of the hitbox
				}
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
	//Ladder response
	else if (collider->getTargetname() == "ladder")
	{
		//Disallow standing on ladders and update the traceker
		if (getCollisionBox().left + 3 * getCollisionBox().width / 4 > collider->getPosition().x)
		{
			isOnGround = false;
			isLadderAvailable = true;
		}

		//If he is on a ladder, center the player pos at the center of the ladder tile
		if (isOnLadder)
			setPosition(collider->getPosition().x - (getCollisionBox().left - getPosition().x), getPosition().y);
	}
}


void Player::changePlayerMode(unsigned mode)
{
	//Since not all megaman sprite rects are the same size, we have to change the player size and hitbox accordingly
	//For instance, when it slides the hitbox is to be of only 1 tile vertically and more than 2 tiles horizontally
	switch (mode)
	{
	case 0:
		setSize(sf::Vector2f(75, 75));
		setCollisionBox(sf::FloatRect(10, 5, 55, 70));
		break;
	case 1:
		setSize(sf::Vector2f(78, 90));
		setCollisionBox(sf::FloatRect(12, 5, 54, 70));
		break;
	case 2:
		setSize(sf::Vector2f(75, 75));
		setCollisionBox(sf::FloatRect(10, 5, 55, 70));
		break;
	case 3:
		setSize(sf::Vector2f(64, 90));
		setCollisionBox(sf::FloatRect(8, 5, 48, 70));
		break;
	default:
		break;
	}
}