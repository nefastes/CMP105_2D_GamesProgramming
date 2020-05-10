#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "Framework/AudioManager.h"
#include "DebugUi.h"
#define SCROLL_SPEED -55.f
class CreditCinematic
{
private:
	//Stuff from Main()
	sf::RenderWindow* window;
	Input* input;
	AudioManager* audio;
	GameState* gameState;
	DebugUi* debugUi;

	//Fake credit texts & images
	sf::Font font;

	sf::Text fakeTitle;
	sf::Text fakeSubTitle;
	
	sf::Text leadDesignerTitle;
	sf::Text leadDesignerName;

	sf::Text artistTitle;
	sf::Text artistName;

	sf::Text leadProgramerTitle;
	sf::Text leadProgramerName;

	sf::Text cakeTitle;
	sf::Texture cakeTex;
	sf::RectangleShape cake;

	sf::Text leadTesterTitle;
	sf::Text leadTesterName;

	sf::Text publisherTitle;
	sf::Texture infogramesLogoTex;
	sf::RectangleShape infogramesLogo;

	//Time tracker
	float timePassedTracker;

	//Private function
	void initTitle(sf::Text& t, std::string m);
	void initSub(sf::Text& t, std::string m);
	void resetPositions();

public:
	CreditCinematic(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* ui);
	~CreditCinematic();

	void handleInput(float dt);
	void update(float dt);
	void render();

	void beginDraw();
	void endDraw();
};

