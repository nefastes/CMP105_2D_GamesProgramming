#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/Collision.h"
#include "Framework/GameState.h"
#include "MainMenu.h"
class OptionMenu : public MainMenu
{
private:
	//Font
	sf::Font font;

	//Option titles
	sf::Text frameLimitTitle;
	sf::Text windowSizeTitle;
	sf::Text verticalSyncTitle;
	sf::Text debugModeTitle;

	//Buttons
	sf::Text howToPlayButton;
	sf::Text applyButton;
	sf::Text backButton;

	//Option infos
	unsigned frameLimit;
	sf::VideoMode windowSize;
	bool verticalSync;
	bool debugMode;

	//Background image
	sf::Texture backgroundTex;
	sf::RectangleShape background;

public:
	//Required functions
	OptionMenu(sf::RenderWindow* hwnd, Input* in, GameState* gs);
	~OptionMenu();
	void update(float dt) override;
	void handleInput(float dt) override;
	void render() override;
};

