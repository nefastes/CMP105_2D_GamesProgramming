#pragma once
#include "Suzy.h"
#include "Player.h"
#include "Bullet.h"
#include "Framework/Collision.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "TileManager.h"
class SuzyManager
{
private:
	sf::Texture suzyTex;
	std::vector<Suzy> suzies;

	unsigned aliveSprites;
	TileManager* tileManager;
	AudioManager* audio;
	GameState* gameState;

public:
	SuzyManager();
	~SuzyManager();

	void spawnSuzy(sf::Vector2f spawnpoint, bool vertical);
	void update(float dt, Player& p);
	void render(sf::RenderWindow* window);
	void killAllSuzies();

	void sendPointers(TileManager* tm, AudioManager* aud, GameState* gs);
	void setDebugging(bool debug);
};

