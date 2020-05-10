#pragma once
#include "Level.h"
#include "SuzyManager.h"
#include "BlasterManager.h"
#include "KamadomaBigEyeManager.h"
#include "SuperCutterManager.h"
class Sciman : public Level
{
private:
	//Add any extra components to the level which are level specific here
	SuzyManager suzyManager;
	BlasterManager blasterManager;
	KamadomaBigEyeManager kamadomaManager;
	SuperCutterManager superCutterManager;

	//Time tracker to spawn cutters every .5s
	float cutterTimeTracker;
	
public:
	Sciman(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui);
	~Sciman();

	void handleInput(float dt);
	void update(float dt);
	void render();

	void spawnItemsInRoom(sf::Vector2f position) override;
	void updateEnemies(float dt) override;	
	void renderEnemies(sf::RenderWindow* window) override;
	void handlePause(float dt);
};

