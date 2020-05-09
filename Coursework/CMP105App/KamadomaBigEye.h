#pragma once
#include "Enemy.h"
#include "Player.h"
class KamadomaBigEye : public Enemy
{
private:
	bool isOnground;

public:
	KamadomaBigEye();
	~KamadomaBigEye();

	void update(float dt, Player& p);
	
	void setMaxHealth(short int max);
	void setGroundState(bool gnd);
	bool getGroundState();
};

