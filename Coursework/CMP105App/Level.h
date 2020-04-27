#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "DebugUi.h"
#include "Player.h"
#include "TileManager.h"
#include "ItemManager.h"
#include "DeathParticleManager.h"
#include "PauseHud.h"


class Level{
public:
	Level();
	~Level();

protected:
	void initLevel(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui);

	void handleLevelInput(float dt);
	void updateLevel(float dt);
	void renderLevel();

	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	void startLevel(float dt);
	void resetLevel();
	void handleLevelPause(float dt);
	virtual void spawnItemsInRoom(sf::Vector2f position);

	//Spawn position
	sf::Vector2f spawnPoint;
	unsigned spawnMap;

	//Tracks which level is the be loaded
	Maps currentLevel;

	//Tracks which map of the level is to be loaded
	unsigned currentMap;

	//Create an itemManager object
	ItemManager itemManager;

	//Create the tileManager object
	TileManager tileManager;

	//Player stuff
	Player player;
	sf::Texture playerTex;

	// Default variables for level class.
	sf::RenderWindow* window;
	Input* input;
	GameState* gameState;
	AudioManager* audio;
	DebugUi* debugUi;

private:
	//Create the window camera
	sf::View camera;

	//Init the death particles manager
	DeathParticleManager deathParticleManager;

	//Create a pause object
	PauseHud pauseUi;

	//Ready message
	sf::Font font;
	sf::Text readyText;
	int readyBlinkCount;
	bool isReadyBlinking;

	//Create a text to display the score
	sf::Text scoreText;
	//Create a counter to count the score at the end of the level
	unsigned counter;

	//Trackers
	float timePassedTracker;
	bool playerSpawned;
};