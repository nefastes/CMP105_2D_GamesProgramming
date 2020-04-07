#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "Player.h"
#include "Boss.h"
#include "DialogBox.h"
#include "DebugUi.h"
#include "TileManager.h"
#include "DeathParticleManager.h"

class Tutorial
{
private:
	//Stuff from Main()
	sf::RenderWindow* window;
	Input* input;
	AudioManager* audio;
	GameState* gameState;
	DebugUi* debugUi;

	//Create the tileManager object
	TileManager tileManager;

	//Create the player object
	sf::Texture playerTex;
	sf::Texture teleportationTex;
	Player player;

	//Checkpoints positions (the tutorial only has one checkpoint, but on other levels this would be an array of checkpoints)
	sf::Vector2f checkpoint;

	//Init the death particles manager
	DeathParticleManager deathParticleManager;

	//Create the window camera
	sf::View camera;

	//Trackers
	float timePassedTracker;
	bool playerSpawned;

public:
	Tutorial(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui);
	~Tutorial();
	void handleInput(float dt);
	void update(float dt);
	void render();

private:
	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	//Level handling functions
	void startLevel(float dt);
	void restartLevel();
};

