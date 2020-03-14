#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
class Boss : public GameObject
{
private:
	//Phase 1 anims
	Animation transform;
	Animation taunt;
	Animation idlePhase1;
	Animation intoAttackPhase1;
	Animation attackIdlePhase1_1;
	Animation attackIdlePhase1_2;
	Animation attackPhase1;

	//Phase 2 anims


	//Phase 3 anims


	//Other
	sf::RenderWindow* window;

public:
	Boss();
	~Boss();
	void update(float dt) override;
	void setWindow(sf::RenderWindow* hwnd) { window = hwnd; };
};