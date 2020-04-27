#pragma once
#include "Framework/GameObject.h"
#include "Framework/GameState.h"
class Suzy : public GameObject
{
private:
	bool isOnWall;
	bool moveVertically;
	float timePassedTracker;
	short int health;
	bool isDying;

public:
	Suzy();
	~Suzy();

	void update(float dt) override;
	void setAxis(bool vertical);
	void setWallState(bool wall);
	bool getWallState();
	bool isMovingVertically();
	void damage(short int amount, GameState* gs);
	void resetHealth();

	//Need this function so we can display a death frame for a couple ms
	bool getDying();
};

