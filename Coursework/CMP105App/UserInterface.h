#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/Collision.h"
#include "Framework/GameState.h"
class UserInterface
{
private:
	//Objects from MAIN
	sf::RenderWindow* window;
	Input* input;
	GameState* gameState;

	//Other objects
	sf::Font font;
	sf::Text startButton;
	sf::Text optionButton;
	sf::Text creditButton;

	//Trackers
	float timePassedTracker;
	unsigned selectionTracker;

public:
	//Required functions
	UserInterface(sf::RenderWindow* hwnd, Input* in, GameState* gs);
	~UserInterface();
	void update(float dt);
	void handleInput(float dt);
	void render();
	void beginDraw();
	void endDraw();

	//Other functions
	void initText(sf::Text& txt);
};

