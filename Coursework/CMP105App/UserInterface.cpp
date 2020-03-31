#include "UserInterface.h"
UserInterface::UserInterface(sf::RenderWindow* hwnd, Input* in, GameState* gs)
{
	//Init window and input
	window = hwnd;
	input = in;
	gameState = gs;

	//Init buttons text strings
	startButton.setString("START GAME");
	optionButton.setString("OPTION");
	creditButton.setString("CREDIT");

	//Create buttons
	font.loadFromFile("font/PressStart2P-vaV7.ttf");
	initText(startButton);
	initText(optionButton);
	initText(creditButton);

	//Position buttons
	startButton.setPosition(window->getSize().x / 2, 2 * window->getSize().y / 3 - 2 * optionButton.getGlobalBounds().height);
	optionButton.setPosition(window->getSize().x / 2, 2 * window->getSize().y / 3);
	creditButton.setPosition(window->getSize().x / 2, 2 * window->getSize().y / 3 + 2 * optionButton.getGlobalBounds().height);

	//Init trackers
	selectionTracker = 0;
}

UserInterface::~UserInterface()
{

}

void UserInterface::update(float dt)
{
	switch (selectionTracker)
	{
	case 0:
		startButton.setFillColor(sf::Color::Yellow);
		optionButton.setFillColor(sf::Color::White);
		creditButton.setFillColor(sf::Color::White);
		break;
	case 1:
		startButton.setFillColor(sf::Color::White);
		optionButton.setFillColor(sf::Color::Yellow);
		creditButton.setFillColor(sf::Color::White);
		break;
	case 2:
		startButton.setFillColor(sf::Color::White);
		optionButton.setFillColor(sf::Color::White);
		creditButton.setFillColor(sf::Color::Yellow);
		break;
	default:
		break;
	}
}

void UserInterface::handleInput(float dt)
{
	//Update time passed
	timePassedTracker += dt;

	//Change the selection
	if (timePassedTracker > .2f)		//Allow a change of selection every .2 seconds, so that we do not need to be a ninja to select
	{
		//Keyboard selection highlight
		if (input->isKeyDown(sf::Keyboard::Up))
		{
			if (selectionTracker == 0) selectionTracker = 2;
			else --selectionTracker;
			timePassedTracker = 0;
		}
		if (input->isKeyDown(sf::Keyboard::Down))
		{
			if (selectionTracker == 2) selectionTracker = 0;
			else ++selectionTracker;
			timePassedTracker = 0;
		}

		//Mouse selection highlight
		sf::Vector2i mousePos = sf::Vector2i(input->getMouseX(), input->getMouseY());
		if (Collision::checkBoundingBox(&startButton.getGlobalBounds(), mousePos))
			selectionTracker = 0;
		if (Collision::checkBoundingBox(&optionButton.getGlobalBounds(), mousePos))
			selectionTracker = 1;
		if (Collision::checkBoundingBox(&creditButton.getGlobalBounds(), mousePos))
			selectionTracker = 2;
	}

	//Make a selection
	if (input->isKeyDown(sf::Keyboard::Enter) || input->isKeyDown(sf::Keyboard::F))
	{
		switch (selectionTracker)
		{
		case 0:
			gameState->setCurrentState(State::LEVEL);
			break;
		case 1:
			break;
		case 2:
			gameState->setCurrentState(State::CREDITS);
			break;
		default:
			break;
		}
	}
}

// Render level
void UserInterface::render()
{
	beginDraw();

	//Draw everything to the screen
	window->draw(startButton);
	window->draw(optionButton);
	window->draw(creditButton);

	endDraw();
}

// Begins rendering to the back buffer. Background colour set to black.
void UserInterface::beginDraw()
{
	window->clear(sf::Color(1, 1, 1));
}

// Ends rendering to the back buffer, and swaps buffer to the screen.
void UserInterface::endDraw()
{
	window->display();
}

void UserInterface::initText(sf::Text& txt)
{
	txt.setFont(font);
	txt.setCharacterSize(24);
	txt.setFillColor(sf::Color::White);
	txt.setOrigin(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2);	//Center the text alignment
}