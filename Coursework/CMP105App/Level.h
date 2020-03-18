#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "Player.h"
#include "Background.h"
#include "FrontGround.h"
#include "Boss.h"
#include "DialogBox.h"
#include <string>
#include <iostream>


class Level{
public:
	Level(sf::RenderWindow* hwnd, Input* in);
	~Level();

	void handleInput(float dt);
	void update(float dt);
	void render();

private:
	// Default functions for rendering to the screen.
	void beginDraw();
	void endDraw();

	// Default variables for level class.
	sf::RenderWindow* window;
	Input* input;
	GameState* gameState;
	AudioManager* audio;

	//Player stuff
	Player player;
	sf::Texture playerTex;

	//Backgrounds stuff
	Background background;
	FrontGround frontground;
	sf::Texture backgroundImage;

	//Boss stuff
	Boss boss;
	sf::Texture bossTex;

	//Dialog stuff
	DialogBox dialogBox;
	sf::Texture dialogTex;
};