#pragma once
#include "EnemyManager.h"
#include "SuperCutter.h"
#include "Player.h"
#define CUTTER_VERTICAL_VEL -900.f
class SuperCutterManager : public EnemyManager
{
private:
	sf::Texture cutterTex;
	std::vector<SuperCutter> cutters;

public:
	SuperCutterManager();
	~SuperCutterManager();

	void spawnCutter(sf::Vector2f spawnPoint, float direction);

	void updateCutters(float dt, Player& p);
	void render(sf::RenderWindow* window);

	void killAllCutters();

	void setDebugging(bool debug);
};

