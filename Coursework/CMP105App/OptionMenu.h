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
	sf::Text arrows[2][2];						//[1][2] -> first option right arrow
	sf::Texture checkboxTex;
	sf::RectangleShape checkboxes[2];			//we have 2 cheboxes

	//Option infos
	sf::Text frameLimitDisplay;
	unsigned frameLimit;
	sf::VideoMode windowSize;
	bool verticalSync;
	bool debugMode;

	//Background image
	sf::Texture backgroundTex;
	sf::RectangleShape background;

	//Trackers
	unsigned selectionTracker;
	float timePassedTracker;
	bool selected;
	bool leftArrow;

public:
	//Required functions
	OptionMenu(sf::RenderWindow* hwnd, Input* in, GameState* gs);
	~OptionMenu();
	void update(float dt) override;
	void handleInput(float dt) override;
	void render() override;

	//Others
	void setButtonsToWhite() override;
	void applySettings();
};

