#pragma once
#include "MainMenu.h"
class StageMenu : public MainMenu
{
private:
	//Textures
	sf::Texture backgroundTex;
	sf::Texture boxesTex;

	//Objects
	sf::RectangleShape background;
	sf::RectangleShape selectionBoxes[6];
	sf::RectangleShape bossImages[6];
	sf::Font font;
	sf::Text bossNames[6];

public:
	StageMenu(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui);
	~StageMenu();
	void update(float dt) override;
	void handleInput(float dt) override;
	void render() override;

private:
	void changeBoxHighlight();
	void changeBoxSelection();
	void selectStage();
	void setButtonsToWhite() override;
};

