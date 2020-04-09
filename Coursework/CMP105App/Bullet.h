#pragma once
#include "Framework/GameObject.h"
class Bullet : public GameObject
{
private:
	bool isFacingRight;

public:
	Bullet();
	~Bullet();
	void update(float dt) override;
	void setBulletDirection(bool right);
};

