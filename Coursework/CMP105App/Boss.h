#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
class Boss : public GameObject
{
private:
	//Phase 1 stuff
	Animation transform;
	Animation taunt;
	Animation idlePhase1;
	Animation intoAttackPhase1;
	Animation attackIdlePhase1_1;
	Animation attackIdlePhase1_2;
	Animation attackPhase1;

	void bossPhase1(float dt);
	void nextAttackPhase1();
	void attack1_1();
	void attack1_2();
	void attack1_3();

	//Phase 2 anims


	//Phase 3 anims


	//Phase tracker
	int phase;

	//State trackers
	bool taunted;				//for the animation starting the fight
	int animMode;				//Is also the attack mode: 0 - idle / 1 - preparing attack / 2 - preparing attack idle / 3 - attacking / 4 - going back to idle from attack / 5 - transform / 6 - taunt
	bool dialogOver;			//will be set to true whenever the dialog at the beginning is over
	float oldTime;				//time tracker

	//Other
	sf::RenderWindow* window;
	bool attackDone;
	bool hasAttacked() { return attackDone; };

public:
	Boss();
	~Boss();
	void update(float dt) override;
	void setWindow(sf::RenderWindow* hwnd) { window = hwnd; };
	void setDialogState(bool state);
};