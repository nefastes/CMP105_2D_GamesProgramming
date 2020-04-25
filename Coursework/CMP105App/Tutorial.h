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
#include "PauseHud.h"
#include "ItemManager.h"

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

	//Create an itemManager object
	ItemManager itemManager;

	//Create the player object
	sf::Texture playerTex;
	Player player;

	//Redy message
	sf::Font font;
	sf::Text readyText;
	int readyBlinkCount;
	bool isReadyBlinking;

	//Create a text to display the score
	sf::Text scoreText;

	//Hints
	sf::Texture moveTex;
	sf::Texture jumpTex;
	sf::Texture climbTex;
	sf::Texture spikeTex;
	sf::RectangleShape hintMove;
	sf::RectangleShape hintJump;
	sf::RectangleShape hintClimb;
	sf::RectangleShape hintSpike;

	//Checkpoints positions (the tutorial only has one checkpoint, but on other levels this would be an array of checkpoints)
	sf::Vector2f checkpoint;

	//Init the death particles manager
	DeathParticleManager deathParticleManager;

	//Create the window camera
	sf::View camera;

	//Create a pause object
	PauseHud pauseUi;

	//Trackers
	float timePassedTracker;
	bool playerSpawned;
	unsigned currentMap;			//Tracks which map of the level is to be loaded

public:
	Tutorial(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui);
	~Tutorial();
	void handleInput(float dt);
	void update(float dt);
	void render();
	void handlePause(float dt);

private:
	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	//Level handling functions
	void startLevel(float dt);
	void restartLevel();

	//Item handle
	void spawnItemsInRoom(sf::Vector2f position);
};

