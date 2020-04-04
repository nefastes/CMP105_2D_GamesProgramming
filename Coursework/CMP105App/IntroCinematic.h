#pragma once
#include <SFML/Graphics.hpp>
#include "Framework/Input.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "Framework/AudioManager.h"
#include "DebugUi.h"
#include "Framework/Animation.h"
class IntroCinematic
{
private:
	//Stuff from Main()
	sf::RenderWindow* window;
	Input* input;
	AudioManager* audio;
	GameState* gameState;
	DebugUi* debugUi;

	//Textures
	sf::Texture logoTex;
	sf::Texture houseBackgroundTex;
	sf::Texture houseFrontgroundTex;
	sf::Texture confictBackgroundTex;
	sf::Texture conflictWilyTex;
	sf::Texture conflictLightTex;
	sf::Texture fortressBackgroundTex;
	sf::Texture fortressFrontgroundTex;
	sf::Texture fortressLightningTex;
	sf::Texture megamanTex;
	sf::Texture laserTex;

	//Shapes
	sf::RectangleShape logoDisplay;
	sf::RectangleShape houseBackground;
	sf::RectangleShape houseFrontground;
	sf::RectangleShape conflictBackground;
	sf::RectangleShape conflictWily;
	sf::RectangleShape conflictLight;
	sf::RectangleShape fortressBackground;
	sf::RectangleShape fortressFrontground;
	sf::RectangleShape fortressLightning;
	sf::RectangleShape finalMegaman;
	sf::RectangleShape finalLaser;

	//Sentences
	sf::Font font;
	sf::Text message;
	std::string sentence[4] = {
		"Once upon a time, homes and families were to\nstay quarantined as a deadly virus was\nruling the streets.",
		"This outbreak was the result of a bad rivalery\nbetween two old friends: the Dr.Light and\nthe Dr.Wily.",
		"Dr.Wily had found an antidote, but refused\nto give it to the people as they were mocking\nhis genius before. Instead, he'd built a\nfortress and robots to hide.",
		"Dr.Light could not let this happen, and created\nMegaMan, a robot whose only purpose was to\ndefeat the evil Dr.Wily and get the antitode."
	};

	//Animations
	Animation lightningBolt;

	//Cinematic trackers
	bool logo;					//Tracks if the logo has been displayed
	bool house;					//Tracks if the house scene has been displayed
	bool conflict;				//Tracks if the conflict has been displayed
	bool fortress;				//Tracks if the fortress has been displayed
	bool megaman;				//Tracks if the final scene has been displayed
	bool megaFront;				//Tracks whether megaman is to be drawn behind or in front of the black fading screen
	bool megaJump;				//Changed to true when we want to change the megaman texture rect to the jump one

	//Trackers
	float timePassedTracker;	//Tracks the time

	//Variables and trackers for the fade screen
	uint8_t alpha;			//Initialise an aplha variable that we will change over time (starts transparent as we want a fade in
	bool hasFadeIn;			//Bool to check if the fade in has been made
	bool isFadingIn;		//Bool to check if a transition is currently being made
	sf::RectangleShape blackScreen;
	float fadeTimeTracker;	//Track the time for fading specifically
	float lightningTimeTracker;	//Track the time for the lightning bolts specifically

public:
	IntroCinematic(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* ui);
	~IntroCinematic();

	void handleInput(float dt);
	void update(float dt);
	void render();

private:
	void beginDraw();
	void endDraw();
	void updateHouse(float dt);
	void updateConflict(float dt);
	void updateFortress(float dt);
	void updateFinalScreen(float dt);
	void fadeToBlackTransition(float transiTime, float dt, bool fadeIn);
	void changeGameStateToMenu();
};

