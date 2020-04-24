#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/GameState.h"
#include "Framework/AudioManager.h"
#include "Framework/Input.h"
#include "Framework/Collision.h"
class PauseHud
{
private:
	//Pointers
	GameState* gameState;
	sf::RenderWindow* window;
	AudioManager* audio;
	Input* input;

	//Trackers for selection
	unsigned selectionTracker;
	float timePassedTracker;

	//Pause menu
	sf::Texture menuTex;
	sf::RectangleShape menu;

	//Create some buttons
	sf::Font font;
	sf::Text resume;
	sf::Text mainMenu;

	//Player infos
	sf::Text lives;

public:
	PauseHud();
	~PauseHud();

	void sendPointers(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);

	void handleInput(float dt);
	void updatePosition();
	void render();
};

