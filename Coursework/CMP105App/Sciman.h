#pragma once
#include "Level.h"
#include "SuzyManager.h"
class Sciman : public Level
{
private:
	//Add any extra components to the level which are level specific here
	SuzyManager suzyManager;
	
public:
	Sciman(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui);
	~Sciman();

	void handleInput(float dt);
	void update(float dt);
	void render();

	void spawnItemsInRoom(sf::Vector2f position) override;
	void handlePause(float dt);
};

