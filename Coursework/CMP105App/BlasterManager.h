#pragma once
#include "Blaster.h"
#include "EnemyManager.h"
class BlasterManager : public EnemyManager
{
private:
	sf::Texture blasterTex;
	std::vector<Blaster> blasters;

public:
	BlasterManager();
	~BlasterManager();

	void spawnBlaster(sf::Vector2f spawnPoint, BlasterAimDirection bad);
	void update(float dt, Player& p);
	void render(sf::RenderWindow* window);
	void killAllBlasters();

	void setDebugging(bool debug);
};

