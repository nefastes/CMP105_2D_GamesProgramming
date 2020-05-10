#pragma once
#include "Enemy.h"
#include "Framework/Animation.h"
class SuperCutter : public Enemy
{
private:
	Animation cutterAnim;

public:
	SuperCutter();
	~SuperCutter();

	void update(float dt) override;
};

