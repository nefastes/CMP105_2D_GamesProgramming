#include "Tutorial.h"
Tutorial::Tutorial(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Init stuff from Main()
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;
	debugUi = dui;

	//Create the map
	tileManager.createMap(Maps::TUTORIAL);

}

Tutorial::~Tutorial()
{

}

void Tutorial::handleInput(float dt)
{

}

void Tutorial::update(float dt)
{

}

// Render level
void Tutorial::render()
{
	beginDraw();

	//Draw everything to the screen
	tileManager.render(window);

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