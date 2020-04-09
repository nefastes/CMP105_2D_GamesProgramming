#include "Player.h"
Player::Player()
{
	//Walk Animation
	walk.addFrame(sf::IntRect(78, 8, 24, 24));
	walk.addFrame(sf::IntRect(104, 8, 24, 24));
	walk.addFrame(sf::IntRect(130, 8, 24, 24));
	walk.addFrame(sf::IntRect(104, 8, 24, 24));
	walk.setFrameSpeed(1.f / 10.f);

	//Shooting and walking animation
	walkShoot.addFrame(sf::IntRect(321, 8, 31, 24));
	walkShoot.addFrame(sf::IntRect(354, 8, 31, 24));
	walkShoot.addFrame(sf::IntRect(387, 8, 31, 24));
	walkShoot.addFrame(sf::IntRect(354, 8, 31, 24));
	walkShoot.setFrameSpeed(1.f / 10.f);

	//Climb animation
	climb.addFrame(sf::IntRect(222, 0, 16, 30));
	climb.addFrame(sf::IntRect(238, 0, -16, 30));
	climb.setFrameSpeed(1.f / 10.f);

	//Teleporation animation
	teleportation.addFrame(sf::IntRect(494, 0, 23, 32));
	teleportation.addFrame(sf::IntRect(508, 0, 23, 32));
	teleportation.addFrame(sf::IntRect(494, 0, 23, 32));
	teleportation.setFrameSpeed(1.f / 20.f);
	teleportation.setLooping(false);

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
	isFacingRight = false;

	//Init health
	health = 100;
	setAlive(true);
	healthTex.loadFromFile("custom_sprites/NES _Mega_Man_Life.PNG");
	for (unsigned i = 0; i < 5; ++i)
	{
		healthBlocks[i].setTexture(&healthTex);
		healthBlocks[i].setTextureRect(sf::IntRect(0, 0, 8, 8));		//Init full health
		healthBlocks[i].setSize(sf::Vector2f(32, 32));
		healthBlocks[i].setPosition(-100, -100);						//Set them off screen first as the player did not spawn yet
	}

	//Ladder trackers
	isLadderAvailable = false;
	isClimbing = false;
	isClimbingDownwards = false;
	isFinishingClimb = false;
	isOnLadder = false;

	//Weapon tracker
	isShooting = false;
	shootTimeTracker = 0;

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

	//Only allow movement when the player is alive
	if (isAlive())
	{
		//Move horizontally
		moveH(dt);
		//Jump ?
		playerJump(dt);
		//Ladder ?
		checkLadderInputs();
		//Shooting ?
		shoot();
	}
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

	//Only do the follwing when the player is alive
	if (isAlive())
	{
		//Death check
		if (health <= 0)
			setAlive(false);

		//Update helath bar
		updateHealth();

		//Animate accordingly
		animations(dt);

		//Update any potential bullet
		bulletManager.update(dt);

		//Simple ladder check that prevents impossible scenarios
		if (!isLadderAvailable)
		{
			isOnLadder = false;
			changePlayerMode(1);
		}

		//Calculate physics
		playerPhysics(dt);
	}
}

void Player::collisionResponse(GameObject* collider)
{
	//Deltas from the center of the collision box of the tile to the center of the collisionbox of the player
	float dx = (collider->getPosition().x + collider->getSize().x / 2) - (getCollisionBox().left + getCollisionBox().width / 2);
	float dy = (collider->getPosition().y + collider->getSize().y / 2) - (getCollisionBox().top + getCollisionBox().height / 2);


	//Y axis hit
	if (collider->getTargetname() == "worldSolid")
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
				if (std::abs(dx) < collider->getSize().x / 2 + getCollisionBox().width / 2 && topTargetname != "worldSolid")	//Need this line to prevent standing on the very edge
				{
					//If he just landed play the sound and update the ground tracker
					if (!isOnGround)
					{
						audio->playSoundbyName("land");
						isOnGround = true;
					}
					isCollidingRight = false;
					isCollidingLeft = false;
					stepVelocity.y = 0;
					isOnLadder = false;					//If he touches the ground, he is not on a ladder anymore
					changePlayerMode(0);				//He is now on ground and not shooting, so we change the mode back to normal (0)
					setPosition(sf::Vector2f(getPosition().x, collider->getPosition().y - getCollisionBox().height - (getSize().y - getCollisionBox().height)));	//Set pos in reguard of the hitbox height minus the diff between the size and the hitbox height
				}
			}
		}
		//X axis hit
		else
		{
			//Only do a response for horizontal collisions if the player is not on a ladder
			if (!isOnLadder)
			{
				//Right side hit
				if (dx < 0)
				{
					isCollidingRight = true;
					isCollidingLeft = false;
					isOnGround = false;
					setPosition(sf::Vector2f(collider->getPosition().x + collider->getSize().x - ((getSize().x - getCollisionBox().width) / 2), getPosition().y));	//Set pos in reguard of the hitbox
				}
				//Left side hit
				else
				{
					isCollidingRight = false;
					isCollidingLeft = true;
					isOnGround = false;
					setPosition(sf::Vector2f(collider->getPosition().x - (getCollisionBox().width + ((getSize().x - getCollisionBox().width) / 2)), getPosition().y));	//Set pos in reguard of the hitbox
				}
			}
		}
	}
	//Ladder response
	else if (collider->getTargetname() == "ladder")
	{
		//Disallow standing on ladders unless it's the top one and update the tracker
		//Enables ladder physics, climbing on a ladder, if 3/4th of the player colbox touches the ladder tile
		if (getCollisionBox().left +  3 * getCollisionBox().width / 4 > collider->getPosition().x && topTargetname == "ladder")
		{
			isOnGround = false;
			isLadderAvailable = true;
		}

		//Finish the climb if the player is on the last tile before the end of the tile to make it more realistic
		if (getCollisionBox().top + getCollisionBox().height / 2 < collider->getPosition().y && isFinishingClimb && !isClimbingDownwards)
		{
			changePlayerMode(0);
			isOnLadder = false;
			isFinishingClimb = false;
			isOnGround = true;
			setPosition(sf::Vector2f(getPosition().x, collider->getPosition().y - getCollisionBox().height));
		}

		//If he is on a ladder, center the player pos at the center of the ladder tile
		if (isOnLadder)
			setPosition(collider->getPosition().x - (getCollisionBox().left - getPosition().x), getPosition().y);


		//If the player is on top of the ladder and begin descend, we do the reverse of the climb finish
		//Which is put the player position down to half of it's hitbox height and align horizontally with the ladder tile to center it
		//This should only happen on the last tile of the ladder which is why we check for a "world" tile on top of it
		if (getCollisionBox().top + getCollisionBox().height / 2 < collider->getPosition().y && isFinishingClimb && isClimbingDownwards && topTargetname == "world")
			setPosition(sf::Vector2f(collider->getPosition().x - (getCollisionBox().left - getPosition().x), collider->getPosition().y - getCollisionBox().height / 2));

		//Disable the ladder availability if the player is standing on top of it, in order to prevent try to climb up of nothing
		if (topTargetname == "world" && isOnGround && !isFinishingClimb)
			isLadderAvailable = false;
	}
	//Spike response
	else if (collider->getTargetname() == "spike")
	{
		//Kill the player
		health = 0;
		setAlive(false);
	}
}


void Player::changePlayerMode(unsigned mode)
{
	//Since not all megaman sprite rects are the same size, we have to change the player size and hitbox accordingly
	//For instance, when it slides the hitbox is to be of only 1 tile vertically and more than 2 tiles horizontally
	switch (mode)
	{
	case 0:
		if (!isShooting)
		{
			setSize(sf::Vector2f(75, 75));
			setCollisionBox(sf::FloatRect(10, 5, 55, 70));
		}
		else
		{
			setSize(sf::Vector2f(97, 75));
			setCollisionBox(sf::FloatRect(21, 5, 55, 70));
		}
		break;
	case 1:
		if (!isShooting)
		{
			setSize(sf::Vector2f(78, 90));
			setCollisionBox(sf::FloatRect(12, 5, 54, 70));
		}
		else
		{
			setSize(sf::Vector2f(84, 90));
			setCollisionBox(sf::FloatRect(15, 5, 54, 70));
		}
		break;
	case 2:
		//Unused yet
		break;
	case 3:
		if (!isShooting)
		{
			setSize(sf::Vector2f(64, 90));
			setCollisionBox(sf::FloatRect(8, 5, 48, 70));
		}
		else
		{

		}
		break;
	default:
		break;
	}
}

void Player::setNeighborsTilesTargetNames(std::string left, std::string top, std::string right, std::string bottom)
{
	leftTargetname = left;
	topTargetname = top;
	rightTargetname = right;
	bottomTargetname = bottom;
}

void Player::moveH(float dt)
{
	//This fucntion moves the player horizontally
	if (input->isKeyDown(sf::Keyboard::Right) || input->isKeyDown(sf::Keyboard::D))
	{
		if (!isCollidingLeft && !isOnLadder)	//Prevent going right if it touches the left of a wall
		{
			//walk
			isMoving = true;
			move(velocity.x * dt, 0);
		}
		isFacingRight = true;
		walk.setFlipped(true);
		walkShoot.setFlipped(true);
	}
	else if (input->isKeyDown(sf::Keyboard::Left) || input->isKeyDown(sf::Keyboard::A))
	{
		if (!isCollidingRight && !isOnLadder)	//Prevent going left if it touches the right of a wall
		{
			//walk but reversed
			isMoving = true;
			move(-velocity.x * dt, 0);
		}
		isFacingRight = false;
		walk.setFlipped(false);
		walkShoot.setFlipped(false);
	}
	else
	{
		//idle
		isMoving = false;
	}
}

void Player::playerJump(float dt)
{
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
}

void Player::checkLadderInputs()
{
	//Ladder
	if (isLadderAvailable && topTargetname != "world" && input->isKeyDown(sf::Keyboard::W))
	{
		isFinishingClimb = false;
		isClimbing = true;
		isClimbingDownwards = false;
		isOnLadder = true;
		isShooting = false;
		changePlayerMode(3);
	}
	else if (isLadderAvailable && topTargetname == "world" && input->isKeyDown(sf::Keyboard::W))
	{
		isFinishingClimb = true;
		isClimbing = true;
		isClimbingDownwards = false;
		isOnLadder = true;
		isOnGround = false;
		isShooting = false;
		changePlayerMode(3);
	}
	else
		isClimbing = false;
	//After OR: allows the user to climb down a ladder from the top of it
	if (isLadderAvailable && !isOnGround && input->isKeyDown(sf::Keyboard::S) || isOnGround && bottomTargetname == "ladder" && input->isKeyDown(sf::Keyboard::S))
	{
		if (topTargetname == "world")
			isFinishingClimb = true;
		else
			isFinishingClimb = false;
		isClimbing = true;
		isClimbingDownwards = true;
		isOnLadder = true;
		isOnGround = false;
		isShooting = false;
		setOrigin(0, 0);
		changePlayerMode(3);
	}
}

void Player::shoot()
{
	if (input->isKeyDown(sf::Keyboard::F) || input->isKeyDown(sf::Keyboard::Enter) || input->isMouseLDown())
	{
		isShooting = true;
		shootTimeTracker = 0;
		if (timePassedTracker > .2f)
		{
			playerShoot();
			timePassedTracker = 0;
		}
	}
}

void Player::animations(float dt)
{
	//Update the shoot anim time tracker
	shootTimeTracker += dt;
	//If the player has not clicked after 1 second, reset the shooting tracker
	if (shootTimeTracker >= 1.f)
	{
		isShooting = false;
		setOrigin(0, 0);
	}

	//Animation
	if (isMoving && isOnGround && !isShooting)
	{
		walk.animate(dt);
		setTextureRect(walk.getCurrentFrame());
	}
	else if (isMoving && isOnGround && isShooting)
	{
		walkShoot.animate(dt);
		setTextureRect(walkShoot.getCurrentFrame());
	}
	else if (isOnLadder && !isShooting)
	{
		if (!isFinishingClimb)
		{
			if (isClimbing)
				climb.animate(dt);
			setTextureRect(climb.getCurrentFrame());
		}
		else
			setTextureRect(sf::IntRect(239, 0, 16, 30));
	}
	else if (isOnLadder && isShooting)
	{
		if(isFacingRight)
			setTextureRect(sf::IntRect(478, 0, -24, 30));
		else
			setTextureRect(sf::IntRect(454, 0, 24, 30));
	}
	else if (!isOnGround && !isShooting)
	{
		if (isFacingRight)
			setTextureRect(sf::IntRect(218, 0, -26, 30));
		else
			setTextureRect(sf::IntRect(192, 0, 26, 30));
	}
	else if (!isOnGround && isShooting)
	{
		if (isFacingRight)
			setTextureRect(sf::IntRect(451, 0, -30, 30));
		else
			setTextureRect(sf::IntRect(421, 0, 30, 30));
	}
	else
	{
		if (!isShooting)
		{
			//If the walk anim was flipped, we have to flip the non-moving frame texture as well
			if (isFacingRight)
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
		else
		{
			if(isFacingRight)
				setTextureRect(sf::IntRect(320, 8, -31, 24));
			else
				setTextureRect(sf::IntRect(289, 8, 31, 24));
		}
	}
}

void Player::playerPhysics(float dt)
{
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
		if (isClimbingDownwards)
			move(sf::Vector2f(0, 100.f) * dt);
		else
			move(sf::Vector2f(0, -100.f) * dt);
	}
	if (isOnGround)
		allowJump = true;
}

void Player::updateHealth()
{
	//Set the position of the health bar relatively to the view
	sf::Vector2f topLeft = sf::Vector2f(window->getView().getCenter() - window->getView().getSize() / 2.f);
	for(int i = 0; i < 5; ++i)
		healthBlocks[i].setPosition(topLeft.x + healthBlocks[i].getSize().x, topLeft.y + (5 - i) * healthBlocks[i].getSize().y);		//Set the healthbar vertically

	//Determine the last block to display full health and which amount of health it should display
	//For instance: with health = 50, the last two blocks are still full health and the middle block has half
	int blockNumber = (int)health / 20;
	if (blockNumber == 5) blockNumber = 4;					//if it was 5 it means the player has 100 hp, 99 hp wil give 4, so we need to adjust to stay in bound of the array
	int blockHealth = (int)health - 20 * blockNumber;		//will give us the exact health remaining
	int blockBars = (int)blockHealth / 4;					//will give how how much bars is to be displayed

	//Display all block full health
	for (int i = 0; i <= blockNumber; ++i)
		healthBlocks[i].setTextureRect(sf::IntRect(0, 0, 8, 8));

	//Display the black with non full health
	switch(blockBars)
	{
	case 0:			healthBlocks[blockNumber].setTextureRect(sf::IntRect(0, 36, 8, 8));				break;
	case 1:			healthBlocks[blockNumber].setTextureRect(sf::IntRect(0, 27, 8, 8));				break;
	case 2:			healthBlocks[blockNumber].setTextureRect(sf::IntRect(0, 18, 8, 8));				break;
	case 3:			healthBlocks[blockNumber].setTextureRect(sf::IntRect(0, 9, 8, 8));				break;
	case 4:			healthBlocks[blockNumber].setTextureRect(sf::IntRect(0, 0, 8, 8));				break;
	default:																						break;
	}

	//Display all remaining blocks with no health
	for (int i = 4; i > blockNumber; --i)
		healthBlocks[i].setTextureRect(sf::IntRect(0, 36, 8, 8));
}

void Player::drawHealth(sf::RenderWindow* window)
{
	for (unsigned i = 0; i < 5; ++i)
		window->draw(healthBlocks[i]);
}

void Player::resetHealthPos(sf::Vector2f pos)
{
	//This function sets all the health blocks to the give coordinates
	//Used to put it all off screen
	for(unsigned i = 0; i < 5; ++i)
		healthBlocks[i].setPosition(pos);
}

void Player::playerShoot()
{
	//This fucntion spawns a bullet at the correct place and plays the shoot sound
	if (isOnGround)
	{
		if(isFacingRight)
			bulletManager.spawnBullet(sf::Vector2f(getPosition().x + getSize().x, getPosition().y + getSize().y / 2), true);
		else
			bulletManager.spawnBullet(sf::Vector2f(getPosition().x, getPosition().y + getSize().y / 2), false);
	}
	else if (!isOnGround && !isOnLadder)
	{
		if (isFacingRight)
			bulletManager.spawnBullet(sf::Vector2f(getPosition().x + getSize().x, getPosition().y + getSize().y / 3), true);
		else
			bulletManager.spawnBullet(sf::Vector2f(getPosition().x, getPosition().y + getSize().y / 3), false);
	}
	else if (isOnLadder)
	{
		if (isFacingRight)
			bulletManager.spawnBullet(sf::Vector2f(getPosition().x + getSize().x, getPosition().y + getSize().y / 3), true);
		else
			bulletManager.spawnBullet(sf::Vector2f(getPosition().x, getPosition().y + getSize().y / 3), false);
	}
}

void Player::renderBullets(sf::RenderWindow* window)
{
	bulletManager.renderBullets(window);
}