#pragma once
#include "BlasterBullet.h"
#include "TileManager.h"

enum class BlasterAimDirection { LEFT, RIGHT };

class BlasterBulletManager
{
private:
	sf::Texture blasterBulletTex;

	sf::Vector2f bulletDirection;

	BlasterAimDirection blasterDirection;

	//Can only fire up to 9 bullets
	BlasterBullet blasterBullets[9];
public:
	BlasterBulletManager();
	~BlasterBulletManager();

	void spawn3Bullets(sf::Vector2f spawnPoint);	//fires 3 bullets at a time
	void update(float dt);
	void renderBlasterBullets(sf::RenderWindow* window, TileManager* tm);
	void setBlasterDirection(BlasterAimDirection d);
	std::vector<BlasterBullet*> getAliveBullets();
	void killAllBullets();
	void setDebugging(bool debug);
};

