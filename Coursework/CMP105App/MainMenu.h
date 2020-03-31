#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/Collision.h"
#include "Framework/GameState.h"
#include "Framework/Animation.h"
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

	//Trackers
	float timePassedTracker;			//Track time
	unsigned selectionTracker;			//Track which button is highlighted
	bool selected;						//Track user selection
	bool isBlinking;					//Track a blinking selected text
	bool hasFinishedBlinking;			//Track if the blinking of the selected text is finished
	unsigned blinkCount;				//Track how many blinking have been performed

public:
	//Required functions
	MainMenu();
	MainMenu(sf::RenderWindow* hwnd, Input* in, GameState* gs);
	~MainMenu();
	virtual void update(float dt);
	virtual void handleInput(float dt);
	virtual void render();
	void beginDraw();
	void endDraw();

protected:
	//Objects from MAIN
	sf::RenderWindow* window;
	Input* input;
	GameState* gameState;

	//Other functions
	void initText(sf::Text& txt, sf::Font& f);
	void blinkText(sf::Text& txt, float dt);
};

