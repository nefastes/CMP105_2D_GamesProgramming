#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
#include <cmath>
class Player : public GameObject
{
private:
	//main stuff
	sf::RenderWindow* window;

	//Animations
	Animation walk;
	Animation climb;
	Animation teleportation;
	
	//Player trackers
	bool allowControls;
	bool isMoving;
	bool isOnGround;
	bool isJumping;
	bool allowJump;
	bool isCollidingRight;
	bool isCollidingLeft;

	//Player health (must be int (short cause it's not gonna go over 100 and under -100, saves space)
	//cause it can go negative if he is low on health and takes damage)
	short int health;

	//Neighbour tiles tracker
	std::string leftTargetname, topTargetname, rightTargetname, bottomTargetname;

	//Ladder trackers
	bool isLadderAvailable;
	bool isClimbing;
	bool isClimbingDownwards;
	bool isFinishingClimb;
	bool isOnLadder;

	//General trackers
	float timePassedTracker;
	float jumpKeyPressTracker;

	//Rng stuff
	float rng;
	bool isRngSet;

	//Physics stuff
	float sScale;
	sf::Vector2f gravity;
	sf::Vector2f stepVelocity;

public:
	Player();
	~Player();
	void update(float dt) override;
	void handleInput(float dt) override;
	void setWindow(sf::RenderWindow* hwnd) { window = hwnd; };
	void collisionResponse(GameObject* collider);
	void setStates(bool left, bool right, bool ground) { isCollidingLeft = left; isCollidingRight = right; isOnGround = ground; };
	void setLadderAvailable(bool available) { isLadderAvailable = available; }
	void freezeControls(bool freeze) { allowControls = !freeze; };
	void setNeighborsTilesTargetNames(std::string left, std::string top, std::string right, std::string bottom);
	bool isTeleportAnimFinished(float dt) { teleportation.animate(dt);  return !teleportation.getPlaying(); };
	void setHealth(short int h) { health = h; };

private:
	//Other functions for the player specifically
	void changePlayerMode(unsigned mode);		//0 - Standing / 1 - Jumping / 2 - Sliding / 3 - Climbing
	//Break down of input sections
	void moveH(float dt);
	void playerJump(float dt);
	void checkLadderInputs();
	//Break down of update sections
	void animations(float dt);
	void playerPhysics(float dt);
};

