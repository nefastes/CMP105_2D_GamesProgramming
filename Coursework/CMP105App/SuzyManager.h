#pragma once
#include "Suzy.h"
#include "Player.h"
#include "Framework/Collision.h"
#include "TileManager.h"
class SuzyManager
{
private:
	sf::Texture suzyTex;
	std::vector<Suzy> suzies;

	unsigned aliveSprites;
	TileManager* tileManager;

public:
	SuzyManager();
	~SuzyManager();

	void spawnSuzy(sf::Vector2f spawnpoint, bool vertical);
	void update(float dt, Player& p);
	void render(sf::RenderWindow* window);
	void killAllSuzies();

	void sendPointers(TileManager* tm);
	void setDebugging(bool debug);
};

