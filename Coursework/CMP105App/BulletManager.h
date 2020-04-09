#pragma once
#include "Bullet.h"
#include "Framework/AudioManager.h"
class BulletManager
{
private:
	//Maximum of 3 bullet at a time in megaman games
	Bullet bullets[3];
	//Bullet texture
	sf::Texture bulletTex;
	//Audio manager to play the shoot sound when spawned
	AudioManager* audio;

public:
	BulletManager();
	~BulletManager();

	void spawnBullet(sf::Vector2f spawnpoint, bool right);
	void update(float dt);
	void renderBullets(sf::RenderWindow* window);
	void killAllBullets();

	void setAudio(AudioManager* aud) { audio = aud; };
};

