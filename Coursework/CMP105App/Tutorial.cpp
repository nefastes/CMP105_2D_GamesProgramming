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
	player.setSize(sf::Vector2f(75, 75));
	player.setInput(input);
	player.setWindow(window);
	player.setPosition(sf::Vector2f(100, 200));
	player.setVelocity(sf::Vector2f(200, 0));
	player.setCollisionBox(sf::FloatRect(10, 5, 55, 70));
	player.setCollisionBoxColor(sf::Color::Red);

	//Init camera
	camera = window->getView();
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

	//Set the camera relatively to the player's position
	//the 50 is because of the tile size
	if (player.getPosition().x >= 0 + camera.getSize().x / 2 && player.getPosition().x <= tileManager.getMapSize().x * 50 - camera.getSize().x / 2)
		camera.move(sf::Vector2f(player.getPosition().x - camera.getCenter().x, 0));

	//Update debug infos
	if (debugUi->isDebugging())
	{
		debugUi->updateDebugUi();
		player.setDebugging(true);
	}
}

// Render level
void Tutorial::render()
{
	beginDraw();

	//Set the window view
	window->setView(camera);

	//Draw everything to the screen
	tileManager.render(window);
	window->draw(player);

	//Draw debug infos
	if (debugUi->isDebugging())
	{
		window->draw(*debugUi->getUi());
		window->draw(*player.getDebugObjectSize());
		window->draw(*player.getDebugCollisionBox());
	}

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