#pragma once
#include "Framework/GameObject.h"
class Enemy : public GameObject
{
protected:
	float timePassedTracker;
	short int maxHealth;
	bool isDying;
	bool invincibility;

private:
	short int health;
	sf::Texture deathParticleTex;

public:
	Enemy();
	~Enemy();

	void damage(short int amount);
	void resetHealth();

	//Need this function so we can display a death frame for a couple ms
	bool getDying();
	bool isInvincible();
};

