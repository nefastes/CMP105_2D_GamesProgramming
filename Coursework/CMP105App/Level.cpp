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
	player.setVelocity(sf::Vector2f(200.f, 0));

	//Backgrounds init
	backgroundImage.loadFromFile("custom_sprites/Background.png");
	background.setTexture(&backgroundImage);
	background.setTextureRect(sf::IntRect(0, 0, 280, 120));
	background.setSize(sf::Vector2f(1700, window->getSize().y));
	background.setPosition(-500, 0);
	frontground.setTexture(&backgroundImage);
	frontground.setTextureRect(sf::IntRect(0, 120, 280, 120));
	frontground.setSize(sf::Vector2f(1700, window->getSize().y));
	frontground.setPosition(-500, 0);
	
	//Boss init
	bossTex.loadFromFile("custom_sprites/Boss_Phase_1.png");
	boss.setTexture(&bossTex);
	boss.setSize(sf::Vector2f(192, 192));
	boss.setOrigin(sf::Vector2f(boss.getSize().x / 2, boss.getSize().y / 2));
	boss.setPosition(1050, 360);

	//Dialog init
	dialogTex.loadFromFile("custom_sprites/TextBox.png");
	dialogBox.setTexture(&dialogTex);
	dialogBox.setSize(sf::Vector2f(window->getSize().x, window->getSize().y / 2));
	dialogBox.setPosition(0, 0);
	dialogBox.setInput(input);
}

Level::~Level()
{

}

// handle user input
void Level::handleInput(float dt)
{
	//Handle inputs of game objects
	player.handleInput(dt);
	dialogBox.handleInput(dt);
}

// Update game objects
void Level::update(float dt)
{
	//Update game objects
	player.update(dt);
	boss.update(dt);
	dialogBox.update(dt);


	//Classes updates
	if (player.isCutsceneFinished())
	{
		dialogBox.activateOnce();
		player.freezeControls(true);
	}
	if (dialogBox.isFinished())
	{
		boss.setDialogState(true);
		player.freezeControls(false);
	}
}

// Render level
void Level::render()
{
	beginDraw();

	//Draw everything to the screen
	window->draw(background);
	if(player.isBehindBackgroundFrontLayer())
		window->draw(player);
	window->draw(frontground);
	if(!player.isBehindBackgroundFrontLayer())
		window->draw(player);
	window->draw(boss);
	if (dialogBox.getActivated())
	{
		window->draw(dialogBox);
		window->draw(dialogBox.getDialog());
	}

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