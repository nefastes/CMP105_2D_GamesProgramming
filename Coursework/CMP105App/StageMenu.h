#pragma once
#include "MainMenu.h"
class StageMenu : public MainMenu
{
private:
	//Textures
	sf::Texture backgroundTex;

	//Objects
	sf::RectangleShape background;

public:
	StageMenu(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui);
	~StageMenu();
	void update(float dt) override;
	void handleInput(float dt) override;
	void render() override;
};

