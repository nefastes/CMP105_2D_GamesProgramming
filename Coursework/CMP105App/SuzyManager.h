#pragma once
#include "Suzy.h"
#include "EnemyManager.h"
class SuzyManager : public EnemyManager
{
private:
	sf::Texture suzyTex;
	std::vector<Suzy> suzies;

public:
	SuzyManager();
	~SuzyManager();

	void spawnSuzy(sf::Vector2f spawnPoint, bool vertical);

	void update(float dt, Player& p);

	void render(sf::RenderWindow* window);
	void killAllSuzies();

	void setDebugging(bool debug);
};

