#include "PauseHud.h"
PauseHud::PauseHud()
{
	//Init tracker
	selectionTracker = 0;
	timePassedTracker = 0;

	//Init pointers
	window = nullptr;
	gameState = nullptr;
	audio = nullptr;
	input = nullptr;

	//Init menu
	menuTex.loadFromFile("custom_sprites/Megaman_2_PauseMenu.PNG");
	menu.setTexture(&menuTex);
	menu.setSize(sf::Vector2f(200, 320));
	menu.setOrigin(sf::Vector2f(menu.getSize().x, 0));		//Set the origin top right, since it will be drawn from the top right corner

	//Init menu options
	font.loadFromFile("font/PressStart2P-vaV7.ttf");
	resume.setFont(font);
	resume.setString("RESUME");
	resume.setCharacterSize(24);
	resume.setFillColor(sf::Color::Yellow);
	resume.setOrigin(resume.getGlobalBounds().width / 2.f, resume.getGlobalBounds().height / 2.f);
	mainMenu.setFont(font);
	mainMenu.setString("MENU");
	mainMenu.setCharacterSize(24);
	mainMenu.setFillColor(sf::Color::White);
	mainMenu.setOrigin(mainMenu.getGlobalBounds().width / 2.f, mainMenu.getGlobalBounds().height / 2.f);

	//Player infos
	lives.setFont(font);
	lives.setString("UP: ");
	lives.setCharacterSize(24);
	lives.setFillColor(sf::Color::White);
	lives.setOrigin(lives.getGlobalBounds().width / 2.f, lives.getGlobalBounds().height / 2.f);
}

PauseHud::~PauseHud()
{

}

void PauseHud::sendPointers(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud)
{
	window = hwnd;
	gameState = gs;
	audio = aud;
	input = in;
}

void PauseHud::handleInput(float dt)
{
	timePassedTracker += dt;

	if (input->isKeyDown(sf::Keyboard::Up) && timePassedTracker >= .2f)
	{
		if (selectionTracker < 1) ++selectionTracker;
		else selectionTracker = 0;
		timePassedTracker = 0;
		audio->playSoundbyName("changeSelection");
	}
	if (input->isKeyDown(sf::Keyboard::Down) && timePassedTracker >= .2f)
	{
		if (selectionTracker > 0) --selectionTracker;
		else selectionTracker = 1;
		timePassedTracker = 0;
		audio->playSoundbyName("changeSelection");
	}
	if (Collision::checkBoundingBox(&resume.getGlobalBounds(), sf::Vector2i(input->getMouseX(), input->getMouseY())))
	{
		selectionTracker = 0;
		audio->playSoundbyName("changeSelection");
	}
	if (Collision::checkBoundingBox(&mainMenu.getGlobalBounds(), sf::Vector2i(input->getMouseX(), input->getMouseY())))
	{
		selectionTracker = 1;
		audio->playSoundbyName("changeSelection");
	}
	if ((input->isKeyDown(sf::Keyboard::Enter) || input->isMouseLDown()) && timePassedTracker >= .2f)
	{
		if (selectionTracker == 0)
		{
			audio->resumeAllMusic();
			audio->resumeAllSounds();
			audio->playSoundbyName("pause");
			gameState->setCurrentState(gameState->getPreviousState());
			timePassedTracker = 0;
		}
		else
		{
			audio->stopAllSounds();
			audio->stopAllMusic();
			gameState->setCurrentState(State::MENU);
			timePassedTracker = 0;
			selectionTracker = 0;
		}
	}

	//Update the highlight colors
	if (selectionTracker == 0)
	{
		resume.setFillColor(sf::Color::Yellow);
		mainMenu.setFillColor(sf::Color::White);
	}
	else
	{
		resume.setFillColor(sf::Color::White);
		mainMenu.setFillColor(sf::Color::Yellow);
	}
}

void PauseHud::updatePosition()
{
	//Set the menu position to the correct location
	menu.setPosition(window->getView().getCenter().x + window->getView().getSize().x / 2.f - 50,
		window->getView().getCenter().y - window->getView().getSize().y / 2.f + 50);

	resume.setPosition(menu.getPosition().x - menu.getSize().x / 2.f, menu.getPosition().y + menu.getSize().y / 4.f);
	mainMenu.setPosition(menu.getPosition().x - menu.getSize().x / 2.f, menu.getPosition().y + 2 * menu.getSize().y / 4.f);
	lives.setString("UP:" + std::to_string(gameState->getGlobalLives()));
	lives.setPosition(menu.getPosition().x - menu.getSize().x / 2.f, menu.getPosition().y + 3 * menu.getSize().y / 4.f);
}

void PauseHud::render()
{
	window->draw(menu);
	window->draw(resume);
	window->draw(mainMenu);
	window->draw(lives);
}