#pragma once
#include "Enemy.h"
class Suzy : public Enemy
{
private:
	bool isOnWall;
	bool moveVertically;

public:
	Suzy();
	~Suzy();

	void update(float dt) override;

	void setAxis(bool vertical);
	void setWallState(bool wall);
	bool getWallState();
	bool isMovingVertically();
	void generateRandomSpawnTime();
};

