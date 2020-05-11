#pragma once
#include "Level.h"

class Tutorial : public Level
{
private:
	//Hints
	sf::Texture hintTex[14];
	sf::RectangleShape hints[14];

public:
	Tutorial(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui);
	~Tutorial();

	void handleInput(float dt);
	void update(float dt);
	void render();

	void handlePause(float dt);

private:
	void spawnItemsInRoom(sf::Vector2f position) override;
	void renderEnemies(sf::RenderWindow* window) override;
};

