#include "OptionMenu.h"
OptionMenu::OptionMenu(sf::RenderWindow* hwnd, Input* in, GameState* gs)
{
	//Init window, input and gameState
	window = hwnd;
	input = in;
	gameState = gs;

	//Init font
	font.loadFromFile("font/PressStart2P-vaV7.ttf");

	//Init option titles strings
	frameLimitTitle.setString("FRAME LIMIT:");
	windowSizeTitle.setString("WINDOW SIZE:");
	verticalSyncTitle.setString("VERTICAL SYNC:");
	debugModeTitle.setString("ENABLE DEBUG:");

	//Init option titles texts
	initText(frameLimitTitle, font);
	initText(windowSizeTitle, font);
	initText(verticalSyncTitle, font);
	initText(debugModeTitle, font);

	//Init option titles positions
	frameLimitTitle.setPosition(sf::Vector2f(window->getSize().x / 4, window->getSize().y / 5));
	windowSizeTitle.setPosition(sf::Vector2f(window->getSize().x / 4, window->getSize().y / 5 + 3 * frameLimitTitle.getGlobalBounds().height));
	verticalSyncTitle.setPosition(sf::Vector2f(window->getSize().x / 4, window->getSize().y / 5 + 6 * frameLimitTitle.getGlobalBounds().height));
	debugModeTitle.setPosition(sf::Vector2f(window->getSize().x / 4, window->getSize().y / 5 + 9 * frameLimitTitle.getGlobalBounds().height));


	//Init buttons strings
	howToPlayButton.setString("HOW TO PLAY");
	backButton.setString("BACK");
	applyButton.setString("APPLY");

	//Init buttons texts
	initText(howToPlayButton, font);
	initText(backButton, font);
	initText(applyButton, font);

	//Init buttons positions
	howToPlayButton.setPosition(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 5 + 12 * frameLimitTitle.getGlobalBounds().height));
	backButton.setPosition(sf::Vector2f(window->getSize().x / 4, window->getSize().y / 5 + 18 * frameLimitTitle.getGlobalBounds().height));
	applyButton.setPosition(sf::Vector2f(3 * window->getSize().x / 4, window->getSize().y / 5 + 18 * frameLimitTitle.getGlobalBounds().height));

	//Init background
	backgroundTex.loadFromFile("custom_sprites/megaman_optionscreen.PNG");
	background.setTexture(&backgroundTex);
	background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	background.setPosition(sf::Vector2f(0, 0));

	//Init option infos
	frameLimit = 60;
	windowSize = sf::VideoMode(1200, 675);
	verticalSync = false;
	debugMode = false;
}

OptionMenu::~OptionMenu()
{

}

void OptionMenu::update(float dt)
{

}

void OptionMenu::handleInput(float dt)
{

}

// Render level
void OptionMenu::render()
{
	beginDraw();

	//Draw everything to the screen
	window->draw(background);
	window->draw(frameLimitTitle);
	window->draw(windowSizeTitle);
	window->draw(verticalSyncTitle);
	window->draw(debugModeTitle);
	window->draw(howToPlayButton);
	window->draw(backButton);
	window->draw(applyButton);

	endDraw();
}