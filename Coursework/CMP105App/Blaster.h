#pragma once
#include "Enemy.h"
#include "BlasterBulletManager.h"
#include "Framework/AudioManager.h"
#include "TileManager.h"
class Blaster : public Enemy
{
private:
	BlasterAimDirection blasterDirection;
	BlasterBulletManager bulletManager;

	//Tracks the state of the blaster (is it firing, resting, opening ?)
	unsigned currentMode;
	unsigned previousMode;

	float rngWaitTime;

public:
	Blaster();
	~Blaster();

	void update(float dt, AudioManager* audio);
	void setBlasterDirection(BlasterAimDirection d);
	BlasterAimDirection getBlasterDirection();
	void renderAliveBullets(sf::RenderWindow* window, TileManager* tm);
	std::vector<BlasterBullet*> getAilveBullets();
	BlasterBulletManager* getBulletManager();
};

