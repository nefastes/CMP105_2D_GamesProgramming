#pragma once
#include "Framework/GameObject.h"
class Suzy : public GameObject
{
private:
	bool isOnWall;
	bool moveVertically;
	float timePassedTracker;

public:
	Suzy();
	~Suzy();

	void update(float dt) override;
	void setAxis(bool vertical);
	void setWallState(bool wall);
	bool getWallState();
	bool isMovingVertically();
};

