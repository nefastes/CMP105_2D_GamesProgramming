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
	
	//Player trackers
	bool allowControls;
	bool isMoving;
	bool isOnGround;
	bool isJumping;
	bool allowJump;
	bool isCollidingRight;
	bool isCollidingLeft;

	//Ladder trackers
	bool isLadderAvailable;
	bool isClimbing;
	bool isClimbingDownwards;
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

private:
	//Other functions for the player specifically
	void changePlayerMode(unsigned mode);		//0 - Standing / 1 - Jumping / 2 - Sliding / 3 - Climbing
};

