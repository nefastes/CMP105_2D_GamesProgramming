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
	Player player;

	//Create the window camera
	sf::View camera;

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
};

