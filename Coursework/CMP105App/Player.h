#pragma once
#include "Framework/GameObject.h"
#include "Framework/AudioManager.h"
#include "Framework/Animation.h"
#include "BulletManager.h"
#include <cmath>
class Player : public GameObject
{
private:
	//main stuff
	sf::RenderWindow* window;
	AudioManager* audio;

	//Managers
	BulletManager bulletManager;

	//Animations
	Animation walk;
	Animation walkShoot;
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
	bool isFacingRight;

	//Collisions trackers, used to renable certain physics if only some or neither are set in the update function
	//After that, we reset them to be rady for the next collision tests
	bool hasCollidedVertically;
	bool hasCollidedHorizontally;
	bool hasCollidedWithLadder;
	//Tracker to prevent those upper trackers to be reset when we are in a transition
	bool isTransitionning;

	//Player health (must be int (short cause it's not gonna go over 100 and under -100, saves space)
	//cause it can go negative if he is low on health and takes damage)
	short int health;
	short int tempHealth;					//Will be used when the player gains healths
	bool isGainingHealth;
	sf::Texture healthTex;
	sf::RectangleShape healthBlocks[5];		//Will make the health bar sprite

	//Neighbour tiles tracker
	std::string leftTargetname, topTargetname, rightTargetname, bottomTargetname, middleTargetname;

	//Ladder trackers
	bool isLadderAvailable;
	bool isClimbing;
	bool isClimbingDownwards;
	bool isFinishingClimb;
	bool isOnLadder;

	//Weapon tracker
	bool isShooting;
	float shootTimeTracker;

	//Taking damage trackers
	bool invicible;
	short unsigned invicibleFrameCount;			//Will be used to track how many time to player blink while invicible
	float invicibleTimeTracker;					//Will be used to make the player blink every .2s
	bool isKnockedBack;

	//General trackers
	float timePassedTracker;
	float jumpKeyPressTracker;

	//Rng stuff
	float rng;
	bool isRngSet;

	//Physics stuff
	float sScale;
	float gravity;

public:
	Player();
	~Player();
	void update(float dt) override;
	void handleInput(float dt) override;
	void setWindow(sf::RenderWindow* hwnd) { window = hwnd; };
	void setAudio(AudioManager* aud) { audio = aud; bulletManager.setAudio(aud); };
	void collisionResponse(GameObject* collider);
	void freezeControls(bool freeze) { allowControls = !freeze; };
	bool isFrozen() { return !allowControls; };
	void setNeighborsTilesTargetNames(std::string left, std::string top, std::string right, std::string bottom, std::string middle);
	bool isTeleportAnimFinished(float dt) { teleportation.animate(dt); setTextureRect(teleportation.getCurrentFrame());  return !teleportation.getPlaying(); };
	void resetTeleportAnim() { teleportation.setPlaying(true); };
	void setHealth(short int h) { health = h; };
	bool addHealth(short int h);
	void drawHealth(sf::RenderWindow* window);
	void resetHealthPos(sf::Vector2f pos);
	void renderBullets(sf::RenderWindow* window);
	void killAllBullets() { bulletManager.killAllBullets(); };
	void resetSettings();	//Called by the level when the player dies
	bool getLadderTracker() { return isOnLadder; };
	void setTransitionning(bool t) { isTransitionning = t; };
	void setMoving(bool mov) { isMoving = mov; };
	std::vector<Bullet*> getBullets();
	void damage(short int amount);
	bool isMovingRight() { return isMoving && isFacingRight; };
	bool isMovingLeft() { return isMoving && !isFacingRight; };

private:
	//Other functions for the player specifically
	void changePlayerMode(unsigned mode);		//0 - Standing / 1 - Jumping / 2 - Sliding / 3 - Climbing / 4 - Shooting on ground / 5 - Shoot and jump / 6 - Shoot on ladder
	//Break down of input sections
	void moveH(float dt);
	void playerJump(float dt);
	void checkLadderInputs();
	void shoot();
	//Break down of update sections
	void animations(float dt);
	void playerPhysics(float dt);
	void updateHealth();
	void playerShoot();
	//Break down of collision
	void worldCollisions(GameObject* collider);
	void ladderCollisions(GameObject* collider);
	void spikeCollisions(GameObject* collider);
};

