#pragma once
#include "MainMenu.h"
class StageMenu : public MainMenu
{
private:
	//Textures
	sf::Texture backgroundTex;
	sf::Image boxesMask;
	sf::Texture boxesTex;

	//Objects
	sf::RectangleShape background;
	sf::RectangleShape selectionBoxes[6];
	sf::RectangleShape bossImages[6];
	sf::Font font;
	sf::Text bossNames[6];
	sf::RectangleShape blueLine;
	sf::Text bossName;
	sf::Text clearPointsText;
	unsigned clearPoints;

	//Boss objects
	sf::Texture scimanTex;
	GameObject sciman;
	Animation scimanIdle;
	bool bossLanded;

	//Escape text
	sf::Text escape;

	//Trackers
	bool updatedClearedLevels;		//Is used to update cleared stages only once in update

public:
	StageMenu(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui);
	~StageMenu();
	void update(float dt) override;
	void handleInput(float dt) override;
	void render() override;

private:
	void changeBoxHighlight();
	void changeBoxSelection();
	void selectStage(float dt);
	void selectBoss(float dt, GameObject* boss);
	void setButtonsToWhite() override;
	void replaceImagePixels(sf::Image& image, sf::IntRect location, sf::Color maskColor,sf::Color resultColor);
};

