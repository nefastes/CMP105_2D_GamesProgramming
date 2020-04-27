#pragma once
#include "Framework/GameObject.h"
class Item : public GameObject
{
private:
	bool isOnGround;
	//0 = points / 1 = 25% health / 2 = 50% health / 3 = 1 Up / 4 = finish level
	short unsigned id;

public:
	Item();
	~Item();

	void update(float dt) override;
	void setId(short unsigned Id);
	short unsigned getId();
	void setGrounded(bool ground);
	bool isGrounded();
};

