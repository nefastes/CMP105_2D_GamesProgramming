#include "Tutorial.h"
Tutorial::Tutorial(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Init stuff from Main()
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;
	debugUi = dui;

	//Send debug infos to tile manager
	tileManager.setDebugRef(debugUi);

	//Create the map
	tileManager.createMap(Maps::TUTORIAL);

	//Init player
	playerTex.loadFromFile("custom_sprites/NES _Mega_Man.PNG");
	player.setTexture(&playerTex);
	player.setSize(sf::Vector2f(50, 75));
	player.setInput(input);
	player.setWindow(window);
	player.setPosition(sf::Vector2f(100, 200));
	player.setVelocity(sf::Vector2f(200, 0));
	player.setCollisionBox(sf::FloatRect(5, 5, 45, 65));
}

Tutorial::~Tutorial()
{

}

void Tutorial::handleInput(float dt)
{
	player.handleInput(dt);
}

void Tutorial::update(float dt)
{
	player.update(dt);
	tileManager.update(dt, player);
}

// Render level
void Tutorial::render()
{
	beginDraw();

	//Draw everything to the screen
	tileManager.render(window);
	window->draw(player);

	endDraw();
}

// Begins rendering to the back buffer. Background colour set to light blue.
void Tutorial::beginDraw()
{
	window->clear(sf::Color(100, 149, 237));
}

// Ends rendering to the back buffer, and swaps buffer to the screen.
void Tutorial::endDraw()
{
	window->display();
}