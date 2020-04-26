#include "Player.h"
#include <iostream>
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
	gravity = 9.8 * sScale;

	//Player trackers
	allowControls = true;
	isMoving = false;
	isOnGround = true;				//Even though we have a spawn anim, he will be on ground when we gain controls
	isJumping = false;
	allowJump = true;
	isCollidingRight = false;
	isCollidingLeft = false;
	isFacingRight = false;

	//Collision trackers
	hasCollidedVertically = true;	//He will spawn on ground
	hasCollidedHorizontally = false;
	hasCollidedWithLadder = false;
	isTransitionning = false;

	//Init health
	health = 100;
	tempHealth = 0;
	isGainingHealth = false;
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

	//Init targetnames detections to nothing
	leftTargetname = "";
	topTargetname = "";
	rightTargetname = "";
	bottomTargetname = "";
	middleTargetname = "";

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
	if (isAlive() && allowControls)
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
	else
	{
		//isMoving = false;
		isClimbing = false;
		isClimbingDownwards = false;
		isJumping = false;
		isShooting = false;
	}
}

void Player::update(float dt)
{
	//Debug
	if (debugging)
	{
		debugSize.setSize(sf::Vector2f(getSize()));
		debugSize.setPosition(getPosition() - getOrigin());
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
		if (!isLadderAvailable && !isOnGround)
		{
			isOnLadder = false;
			changePlayerMode(1);
		}

		//Re-enable physics according to the collision trackers
		if (!hasCollidedVertically && !hasCollidedHorizontally)
		{
			//If all fails, the player is in the air not touching anything or just touching the body of a ladder
			//Need this to re-enable controls and physics in air
			isCollidingLeft = false;
			isCollidingRight = false;
			isOnGround = false;
		}
		if (!hasCollidedHorizontally)
		{
			isCollidingLeft = false;
			isCollidingRight = false;
		}
		if (hasCollidedHorizontally && !hasCollidedVertically)
			isOnGround = false;
		if (hasCollidedWithLadder && !isLadderAvailable && !hasCollidedVertically)
			isOnGround = false;
		if (!hasCollidedWithLadder)
			//Need to reset the ladder availability
			isLadderAvailable = false;

		//Calculate physics
		playerPhysics(dt);

		//Reset collision trackers if we are not in a transition
		if (!isTransitionning)
		{
			hasCollidedVertically = false;
			hasCollidedHorizontally = false;
			hasCollidedWithLadder = false;
		}
	}
}

void Player::collisionResponse(GameObject* collider)
{
	//World response
	if (collider->getTargetname() == "worldSolid")
		worldCollisions(collider);

	//Ladder response
	else if (collider->getTargetname() == "ladder")
		ladderCollisions(collider);

	//Spike response
	else if (collider->getTargetname() == "spike")
		spikeCollisions(collider);
}

void Player::worldCollisions(GameObject* collider)
{
	//Deltas from the center of the collision box of the tile to the center of the collisionbox of the player
	float dx = (collider->getPosition().x + collider->getSize().x / 2) - (getCollisionBox().left + getCollisionBox().width / 2);
	float dy = (collider->getPosition().y + collider->getSize().y / 2) - (getCollisionBox().top + getCollisionBox().height / 2);

	if (std::abs(dx) <= std::abs(dy))
	{
		//Bottom hit
		if (dy < 0)
		{
			//Need this line to prevent colliding with the very edge and only allow a bottom collision if it
			//the tile we bunk our head into
			if (std::abs(dx) < collider->getSize().x / 2 + getCollisionBox().width / 2 && topTargetname == "worldSolid")
			{
				velocity.y = 0;
				isJumping = false;
				allowJump = false;
			}
		}
		//Top hit
		else
		{
			//Need this line to prevent standing on the very edge and allow a vertical collision
			//We also check if the player jumps so that we prevent having a landing while the player is going upwards
			//(was happening if you would jump next to a wall)
			if (std::abs(dx) < collider->getSize().x / 2 + getCollisionBox().width / 2 && !isJumping)
			{
				//If he just landed play the sound and update the ground tracker
				if (!isOnGround)
				{
					audio->playSoundbyName("land");
					isOnGround = true;
				}
				hasCollidedVertically = true;		//We have collided vertically, note that we only set it to true for top collisions
				velocity.y = 0;
				isOnLadder = false;					//If he touches the ground on a descent, he is not on a ladder anymore
				changePlayerMode(0);				//He is now on ground, so we change the mode back to normal (0)
				//Set pos in reguard of the hitbox, the top of the hitbox is lower than the top of the player sprite (thus higher in coord since y
				//increases downwards), which is why we subtract it by the player's sprite y position to get the delta (we could use std::abs but lazy)
				//This will set the bottom of the hitbox just on top of the ground contact
				setPosition(sf::Vector2f(getPosition().x, collider->getPosition().y -
					getCollisionBox().height - (getCollisionBox().top - getPosition().y)));
			}
		}
	}
	//X axis hit
	else
	{
		//We have collided horizontally
		hasCollidedHorizontally = true;

		//If he is currently on ground, we have to make sure the player mode is correctly adapted
		if (isOnGround)
			changePlayerMode(0);

		//Only do a response for horizontal collisions if the player is not on a ladder
		//We also check if the player is further away inside than just touching the wall to avoid more calculations and bugs
		if (!isOnLadder && std::abs(dx) < collider->getSize().x / 2 + getCollisionBox().width / 2)
		{
			//Right side hit
			if (dx < 0)
			{
				isCollidingRight = true;
				isCollidingLeft = false;
				//Set the position in reguard of the hitbox placement, makes it independent of the player's size
				setPosition(sf::Vector2f(collider->getPosition().x + collider->getSize().x -
					(getCollisionBox().left - getPosition().x), getPosition().y));
			}
			//Left side hit
			else
			{
				isCollidingRight = false;
				isCollidingLeft = true;
				//Set the position in reguard of the hitbox placement, makes it independent of the player's size
				setPosition(sf::Vector2f(collider->getPosition().x - getCollisionBox().width -
					(getCollisionBox().left - getPosition().x), getPosition().y));
			}
		}
	}
}

void Player::ladderCollisions(GameObject* collider)
{
	//Deltas from the center of the collision box of the tile to the center of the collisionbox of the player
	float dx = (collider->getPosition().x + collider->getSize().x / 2) - (getCollisionBox().left + getCollisionBox().width / 2);
	float dy = (collider->getPosition().y + collider->getSize().y / 2) - (getCollisionBox().top + getCollisionBox().height / 2);

	//We have collided with a ladder
	hasCollidedWithLadder = true;

	//Enables ladder physics, climbing on a ladder, if at least 3/4th of the player colbox touches the ladder tile
	if (getCollisionBox().left + 3 * getCollisionBox().width / 4 >= collider->getPosition().x && (middleTargetname == "ladder" || bottomTargetname == "ladder"))
		isLadderAvailable = true;
	else
		isLadderAvailable = false;

	if (!isOnLadder)
	{
		//Vertical hit
		if (std::abs(dx) <= std::abs(dy))
		{
			if (dy >= 0)
			{
				//If the player is on top of the ladder and begin descend, we do the reverse of the climb finish
				//Which is put the player position down to half of it's hitbox height and align horizontally with the ladder tile to center it
				//This should only happen on the last tile of the ladder which is why we check for a "world" or "sky" tile on top of it
				if (isClimbingDownwards && isLadderAvailable && bottomTargetname == "ladder" &&
					(middleTargetname == "world" || middleTargetname == "sky"))
				{
					//He is no on the ladder, adjust falgs
					isOnGround = false;
					isFinishingClimb = true;
					isOnLadder = true;
					changePlayerMode(3);
					//And adjust the postion to make it more realistic
					setPosition(sf::Vector2f(collider->getPosition().x - (getCollisionBox().left - getPosition().x),
						collider->getPosition().y - 3 * getCollisionBox().height / 4));
				}

				//The player stands on top of the ladder tile (only on the last one, which means that
				//The bottom tagetname MUST be a ladder and the middle targetname MUST be world otherwise it
				//means that he is anywhere else but on top of it
				else if (bottomTargetname == "ladder" && (middleTargetname == "world" || middleTargetname == "sky"))
				{
					hasCollidedVertically = true;		//We have collided vertically, note that we only set it to true for top collisions
					if (!isOnGround)
					{
						audio->playSoundbyName("land");
						isOnGround = true;
					}
					velocity.y = 0;
					isOnLadder = false;					//If he touches the ground on a descent, he is not on a ladder anymore
					changePlayerMode(0);				//He is now on ground and not shooting, so we change the mode back to normal (0)
					//Set pos in reguard of the hitbox, the top of the hitbox is lower than the top of the player sprite (thus higher in coord since y
					//increases downwards), which is why we subtract it by the player's sprite y position to get the delta (we could use std::abs but lazy)
					//This will set the bottom of the hitbox just on top of the ground contact
					setPosition(sf::Vector2f(getPosition().x, collider->getPosition().y -
						getCollisionBox().height - (getCollisionBox().top - getPosition().y)));
				}
			}
		}
	}
	else
	{
		//Determine if he is about to finish climbing, so we can change the sprite to the finish climb
		//And activate checks to finish the climb once he reached a certain position
		//If the middle targetname is world or sky, it means his feet are at the last tile of the ladder
		if (middleTargetname == "world" || middleTargetname == "sky")
			isFinishingClimb = true;
		else
			isFinishingClimb = false;

		//If he is on a ladder, center the player pos at the center of the ladder tile and adjust hitbox, size and stuff
		changePlayerMode(3);
		setPosition(collider->getPosition().x - (getCollisionBox().left - getPosition().x), getPosition().y);

		//Finish the climb if the player is on the last tile before the end of the tile to make it more accurate
		if (getCollisionBox().top + 3 * getCollisionBox().height / 4 < collider->getPosition().y &&
			isLadderAvailable && isFinishingClimb && !isClimbingDownwards)
		{
			hasCollidedVertically = true;		//We have collided vertically, note that we only set it to true for top collisions
			isOnGround = true;
			velocity.y = 0;
			isOnLadder = false;					//If he is on top, he is not on a ladder anymore
			isFinishingClimb = false;			//The climb has just been finished
			isClimbing = false;					//He is not climbing anymore
			isLadderAvailable = true;			//We are now on top of the last ladder tile, the ladder is still available
			changePlayerMode(0);				//He is now on ground and not shooting, so we change the mode back to normal (0)
			setPosition(sf::Vector2f(getPosition().x, collider->getPosition().y -
				getCollisionBox().height - (getCollisionBox().top - getPosition().y)));
		}
	}
}

void Player::spikeCollisions(GameObject* collider)
{
	//only kill the player if 1/4 of the sprite size box is touching it
	if (getPosition().y + 3 * getSize().y / 4 > collider->getPosition().y && !isOnLadder)
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
		//On Ground
	case 0:
		setCollisionBox(sf::FloatRect(15, 5, 45, 70));
		if (!isShooting)
		{
			setOrigin(0, 0);
			setSize(sf::Vector2f(75, 75));
		}
		else
		{
			if (isFacingRight && !isMoving)
				setOrigin(-10, 0);
			else if (isFacingRight && isMoving)
				setOrigin(0, 0);
			else if (!isFacingRight && !isMoving)
				setOrigin(30, 0);
			else
				setOrigin(20, 0);
			setSize(sf::Vector2f(97, 75));
		}
		break;
		//In air / Jumping
	case 1:
		setCollisionBox(sf::FloatRect(15, 5, 45, 70));
		if (!isShooting)
		{
			setOrigin(0, 0);
			setSize(sf::Vector2f(78, 90));
		}
		else
		{
			if (isFacingRight)
				setOrigin(0, 0);
			else
				setOrigin(12, 0);
			setSize(sf::Vector2f(90, 90));
		}
		break;
		//Sliding
	case 2:
		//Unused yet
		break;
		//On Ladder
	case 3:
		setCollisionBox(sf::FloatRect(7, 5, 45, 70));
		if (!isShooting)
		{
			setOrigin(0, 0);
			setSize(sf::Vector2f(60, 90));
		}
		else
		{
			if (isFacingRight)
				setOrigin(0, 0);
			else
				setOrigin(30, 0);
			setSize(sf::Vector2f(90, 90));
		}
		break;
	default:
		break;
	}
}

void Player::setNeighborsTilesTargetNames(std::string left, std::string top, std::string right, std::string bottom, std::string middle)
{
	leftTargetname = left;
	topTargetname = top;
	rightTargetname = right;
	bottomTargetname = bottom;
	middleTargetname = middle;
}

void Player::moveH(float dt)
{
	//This fucntion moves the player horizontally
	if (input->isKeyDown(sf::Keyboard::D))
	{
		//Prevent going right if it touches the left of a wall or the side of the screen
		if (!isCollidingLeft && !isOnLadder &&
			getCollisionBox().left + getCollisionBox().width <= window->getView().getCenter().x + window->getView().getSize().x / 2)
		{
			//walk
			isMoving = true;
			move(velocity.x * dt, 0);
		}
		isFacingRight = true;
	}
	else if (input->isKeyDown(sf::Keyboard::A))
	{
		//Prevent going left if it touches the right of a wall or the left of the screen
		if (!isCollidingRight && !isOnLadder &&
			getCollisionBox().left >= window->getView().getCenter().x - window->getView().getSize().x / 2)
		{
			//walk but reversed
			isMoving = true;
			move(-velocity.x * dt, 0);
		}
		isFacingRight = false;
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
			//No longer on the ground not matter what, this MUST be false
			isOnGround = false;
			changePlayerMode(1);
			jumpKeyPressTracker += dt;
		}
		else
		{
			//The player jumps out of the ladder, thus is is NOT on a ladder and NOT on the ground
			//We also reset the velocity in case it was not 0 before jumping on the ladder, so it always drops the same
			isOnLadder = false;
			isClimbing = false;
			isClimbingDownwards = false;
			isOnGround = false;
			velocity.y = 0;
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
	if (isLadderAvailable && input->isKeyDown(sf::Keyboard::W))
	{
		if ((!isOnGround && !isJumping || isFinishingClimb) && topTargetname != "ladder" || topTargetname == "ladder")
		{
			//Set up the trackers to climb the ladder
			isClimbing = true;
			isClimbingDownwards = false;
			isOnLadder = true;
			//The player will be on the ladder, therefore must NOT be jumping (makes you fly otherwise)
			isJumping = false;
			//We are now climbing, prevent or cancel any shooting
			isShooting = false;
		}
	}
	//After OR: allows the user to climb down a ladder from the top of it
	else if (isLadderAvailable && input->isKeyDown(sf::Keyboard::S))
	{
		if ((isOnGround || isFinishingClimb) || !isOnGround && middleTargetname == "ladder" && !isFinishingClimb)
		{
			//Set up the trackers to climb the ladder
			isClimbing = true;
			isClimbingDownwards = true;
			//Only activate the ladder flag if he was not on ground before, so we can adjust his position proprely
			if(!isOnGround)
				isOnLadder = true;
			//The player will be on the ladder, therefore must NOT be jumping (makes you fly otherwise)
			isJumping = false;
			//We are now climbing, prevent or cancel any shooting
			isShooting = false;
		}
	}
	else
	{
		//Need this line to disable climbing if the user doesnt want to climb (remove this and it makes very funny stuff)
		isClimbing = false;
	}
}

void Player::shoot()
{
	if (input->isKeyDown(sf::Keyboard::Enter) ||input->isMouseLDown())
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
	}

	//Animation
	if (isMoving && isOnGround && !isShooting)
	{
		if (isFacingRight)
			walk.setFlipped(true);
		else
			walk.setFlipped(false);

		walk.animate(dt);
		setTextureRect(walk.getCurrentFrame());
	}
	else if (isMoving && isOnGround && isShooting)
	{
		if(isFacingRight)
			walkShoot.setFlipped(true);
		else
			walkShoot.setFlipped(false);

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
	//Avoid doing more calculations when touches the ground or when the player jumps
	if (!isOnGround && !isJumping && !isOnLadder && allowControls)
	{
		//Reset jump stats in case ?
		allowJump = false;

		//Apply gravity
		sf::Vector2f pos = sf::Vector2f(0, velocity.y) * dt + 0.5f * sf::Vector2f(0, gravity) * dt;
		velocity.y += gravity * dt;
		setPosition(getPosition() + pos);
	}
	if (isJumping && allowJump)
	{
		//Set the velocity to 600 upwards until it is no longer allowed to jump (after .2 second)
		if (jumpKeyPressTracker < .2f)
		{
			velocity.y = -600.f;
			//Set the position relative to this jump vel
			setPosition(getPosition() + sf::Vector2f(0, velocity.y) * dt);
		}
		else
		{
			isJumping = false;
			allowJump = false;
		}
	}
	//Cannot climb when shoot in original games
	if (isClimbing && !isShooting)
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
	//Check if we are regenerating health
	if (isGainingHealth && health < tempHealth && timePassedTracker >= .05f)
	{
		health += 5;						//Increment health by 5 (he can only gain up to 25 or 50 health)
		timePassedTracker = 0;				//Reset time tracker to get ready for the next gain
		audio->playSoundbyName("health");	//Sound effect
		freezeControls(true);				//Will stuck the player in place, just like in the original game
		isMoving = false;					//Will disable a moving animation
		if (health >= tempHealth)
		{
			//Just to make sure we do not exceed to desired amount
			health = tempHealth;
			//Reset trackers, re-enable controls
			tempHealth = 0;
			isGainingHealth = false;
			freezeControls(false);
		}
	}

	//Set the position of the health bar relatively to the view
	sf::Vector2i topLeft = sf::Vector2i(window->getView().getCenter() - window->getView().getSize() / 2.f);
	for(int i = 0; i < 5; ++i)
		healthBlocks[i].setPosition(topLeft.x + 2 * healthBlocks[i].getSize().x, topLeft.y + (5 - i) * healthBlocks[i].getSize().y);		//Set the healthbar vertically

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

void Player::addHealth(short int h)
{
	//Change the health and store old one so we can animate gaining health inside updateHealth()
	//Only allow to change the health if it is not already full
	if (health < 100)
	{
		if (health > 100 - h) tempHealth = 100;
		else tempHealth = health + h;
		isGainingHealth = true;
	}
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

void Player::resetSettings()
{
	//Reset the origin
	setOrigin(0, 0);
	//Reset player trackers
	isMoving = false;
	isOnGround = true;				//Even though we have a spawn anim, he will be on ground when we gain controls
	isJumping = false;
	allowJump = true;
	isCollidingRight = false;
	isCollidingLeft = false;
	isFacingRight = false;
	//Reset health
	health = 100;
	//Reset ladder trackers
	isLadderAvailable = false;
	isClimbing = false;
	isClimbingDownwards = false;
	isFinishingClimb = false;
	isOnLadder = false;
	//Reset weapon tracker
	isShooting = false;
	shootTimeTracker = 0;
	//Reset general trackers
	timePassedTracker = 0;
	jumpKeyPressTracker = 0;
	//Reset rng
	rng = 0;
	isRngSet = false;
	//Collision trackers
	hasCollidedVertically = true;	//He will spawn on ground
	hasCollidedHorizontally = false;
	hasCollidedWithLadder = false;
	isTransitionning = false;
}