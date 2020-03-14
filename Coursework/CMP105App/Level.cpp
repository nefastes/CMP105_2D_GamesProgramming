#include "Level.h"

Level::Level(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	//initialise game objects
	//Player init
	playerTex.loadFromFile("custom_sprites/Player.png");
	player.setInput(input);
	player.setWindow(window);
	player.setTexture(&playerTex);
	player.setSize(sf::Vector2f(128, 128));
	player.setOrigin(sf::Vector2f(player.getSize().x / 2, player.getSize().y / 2));
	player.setPosition(444, 0 + player.getSize().y);
	player.setVelocity(sf::Vector2f(200.f, 0));

	//Backgrounds init
	backgroundImage.loadFromFile("custom_sprites/Background.png");
	background.setTexture(&backgroundImage);
	background.setSize(sf::Vector2f(1700, window->getSize().y));
	background.setPosition(-500, 0);
	
	//Boss init
}

Level::~Level()
{

}

// handle user input
void Level::handleInput(float dt)
{
	//Handle inputs of game objects
	player.handleInput(dt);
}

// Update game objects
void Level::update(float dt)
{
	//Update game objects
	player.update(dt);
}

// Render level
void Level::render()
{
	beginDraw();

	//Draw everything to the screen
	window->draw(background);
	window->draw(player);

	endDraw();
}

// Begins rendering to the back buffer. Background colour set to light blue.
void Level::beginDraw()
{
	window->clear(sf::Color(100, 149, 237));
}

// Ends rendering to the back buffer, and swaps buffer to the screen.
void Level::endDraw()
{
	window->display();
}