#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/Collision.h"
#include "Framework/GameState.h"
#include "Framework/Animation.h"
#include "Framework/AudioManager.h"
#include "DebugUi.h"
class MainMenu
{
private:
	//Buttons
	sf::Font font;
	sf::Text startButton;
	sf::Text optionButton;
	sf::Text creditButton;
	sf::Text quitButton;

	//Title screen image
	sf::Texture titleTex;
	sf::RectangleShape titleImage;

	//Title friendly reminder
	sf::Texture defoTex;
	sf::RectangleShape defonotText;
	Animation defonotAnim;

	//Background
	sf::Texture backgroundTex;
	sf::RectangleShape background[2];

public:
	//Required functions
	MainMenu();
	MainMenu(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui);
	~MainMenu();
	virtual void update(float dt);
	virtual void handleInput(float dt);
	virtual void render();

protected:
	//Draw functions
	void beginDraw();
	void endDraw();

	//Objects from MAIN
	sf::RenderWindow* window;
	Input* input;
	AudioManager* audio;
	GameState* gameState;
	DebugUi* debugUi;

	//Since we (may) use those trackers in multiple menus, they must go inside protected
	//Trackers
	float timePassedTracker;			//Track time
	unsigned selectionTracker;			//Track which button is highlighted
	bool selected;						//Track user selection
	bool isBlinking;					//Track a blinking selected text
	unsigned blinkCount;				//Track how many blinking have been performed
	bool hasFinishedBlinking;			//Track if the blinking of the selected text is finished
	float afkTimeTracker;				//Track the time were nothing happens in the menu, used to change to the intro cinematic after 30s
	unsigned prevSelection;				//Store the selectionTracker of the previous frame

	//Other functions
	void initText(sf::Text& txt, sf::Font& f);
	void blinkText(sf::Text& txt, float dt);
	virtual void setButtonsToWhite();
	void changeGameState(unsigned state);		//0 = main menu, 1 = stage select, 2 = options, 3 = credits, 4 = intro, 5 = tutorial

private:
	void changeButtonHighlight();
	void makeSelection(float dt);
	void changeSelectionTracker();
	void checkAfkTime();
};

