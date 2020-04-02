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
	sf::Text musicVolumeTitle;
	sf::Text verticalSyncTitle;
	sf::Text debugModeTitle;

	//Buttons
	sf::Text howToPlayButton;
	sf::Text applyButton;
	sf::Text backButton;
	sf::Text arrows[2][2];						//Ex: [1][2] -> first option, right arrow
	sf::Texture checkboxTex;
	sf::RectangleShape checkboxes[2];			//we have 2 cheboxes

	//Option infos
	sf::Text frameLimitDisplay;
	unsigned frameLimit;
	sf::Text musicVolumeDisplay;
	unsigned musicVolume;
	bool verticalSync;
	bool debugMode;

	//Background image
	sf::Texture backgroundTex;
	sf::RectangleShape background;

	//Additionnal Trackers
	bool appliedSettings;				//Track whether changes have been applied or not
	bool leftArrow;						//Track for all arrows whether the right or left arrow was clicked

public:
	//Required functions
	OptionMenu(sf::RenderWindow* hwnd, Input* in, GameState* gs, DebugUi* dui);
	~OptionMenu();
	void update(float dt) override;
	void handleInput(float dt) override;
	void render() override;

private:
	//Others
	void setButtonsToWhite() override;
	void applySettings();
	void changeButtonHighlights();
	void changeSettings();
	void trackButtonSelection();
};

