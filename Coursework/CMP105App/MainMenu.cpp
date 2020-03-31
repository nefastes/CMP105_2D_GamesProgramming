#include "MainMenu.h"
MainMenu::MainMenu(sf::RenderWindow* hwnd, Input* in, GameState* gs)
{
	//Init window and input
	window = hwnd;
	input = in;
	gameState = gs;

	//Init buttons text strings
	startButton.setString("START GAME");
	optionButton.setString("OPTION");
	creditButton.setString("CREDIT");
	quitButton.setString("QUIT");

	//Create buttons
	font.loadFromFile("font/PressStart2P-vaV7.ttf");
	initText(startButton);
	initText(optionButton);
	initText(creditButton);
	initText(quitButton);

	//Position buttons, relative to one of the button height (all the same, so i chose optionButton)
	startButton.setPosition(window->getSize().x / 2, 2 * window->getSize().y / 3 - 2 * optionButton.getGlobalBounds().height);
	optionButton.setPosition(window->getSize().x / 2, 2 * window->getSize().y / 3);
	creditButton.setPosition(window->getSize().x / 2, 2 * window->getSize().y / 3 + 2 * optionButton.getGlobalBounds().height);
	quitButton.setPosition(window->getSize().x / 2, 2 * window->getSize().y / 3 + 4 * optionButton.getGlobalBounds().height);

	//Init title image
	titleTex.loadFromFile("custom_sprites/megaman_titlescreen_transparent.PNG");
	titleImage.setSize(sf::Vector2f(640, 300));
	titleImage.setOrigin(sf::Vector2f(titleImage.getSize().x / 2, titleImage.getSize().y / 2));
	titleImage.setPosition(window->getSize().x / 2, window->getSize().y / 3);
	titleImage.setTexture(&titleTex);

	//Init text animation
	defoTex.loadFromFile("custom_sprites/defonot.PNG");
	defonotText.setTexture(&defoTex);
	defonotText.setSize(sf::Vector2f(252, 50));
	defonotText.setPosition(sf::Vector2f(titleImage.getPosition().x - titleImage.getSize().x / 3, titleImage.getPosition().y - titleImage.getSize().y / 2));
	defonotAnim.addFrame(sf::IntRect(0, 0, 126, 25));
	defonotAnim.addFrame(sf::IntRect(0, 25, 126, 25));
	defonotAnim.addFrame(sf::IntRect(0, 50, 126, 25));
	defonotAnim.addFrame(sf::IntRect(0, 25, 126, 25));
	defonotAnim.addFrame(sf::IntRect(0, 0, 126, 25));
	defonotAnim.addFrame(sf::IntRect(0, 75, 126, 25));
	defonotAnim.addFrame(sf::IntRect(0, 100, 126, 25));
	defonotAnim.addFrame(sf::IntRect(0, 75, 126, 25));
	defonotAnim.setFrameSpeed(1.f / 6.f);

	//Init background
	backgroundTex.loadFromFile("custom_sprites/megaman_background.PNG");
	background[0].setTexture(&backgroundTex);
	background[0].setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	background[0].setPosition(sf::Vector2f(0, 0));
	background[1].setTexture(&backgroundTex);
	background[1].setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	background[1].setPosition(sf::Vector2f(window->getSize().x, 0));

	//Init trackers
	selectionTracker = 0;
	isBlinking = false;
	hasFinishedBlinking = false;
	blinkCount = 0;
}

MainMenu::~MainMenu()
{

}

void MainMenu::update(float dt)
{
	//Menu Selection update
	if (!selected)
	{
		//Change the button highlight
		switch (selectionTracker)
		{
		case 0:
			startButton.setFillColor(sf::Color::Yellow);
			optionButton.setFillColor(sf::Color::White);
			creditButton.setFillColor(sf::Color::White);
			quitButton.setFillColor(sf::Color::White);
			break;
		case 1:
			startButton.setFillColor(sf::Color::White);
			optionButton.setFillColor(sf::Color::Yellow);
			creditButton.setFillColor(sf::Color::White);
			quitButton.setFillColor(sf::Color::White);
			break;
		case 2:
			startButton.setFillColor(sf::Color::White);
			optionButton.setFillColor(sf::Color::White);
			creditButton.setFillColor(sf::Color::Yellow);
			quitButton.setFillColor(sf::Color::White);
			break;
		case 3:
			startButton.setFillColor(sf::Color::White);
			optionButton.setFillColor(sf::Color::White);
			creditButton.setFillColor(sf::Color::White);
			quitButton.setFillColor(sf::Color::Yellow);
		default:
			break;
		}
	}
	else
	{
		if (!hasFinishedBlinking)
		{
			//Blink the selection, or close the window immediately, accordingly
			switch (selectionTracker)
			{
			case 0:			blinkText(startButton, dt);			break;
			case 1:			blinkText(optionButton, dt);		break;
			case 2:			blinkText(creditButton, dt);		break;
			case 3:			window->close();					break;
			default:											break;
			}
		}
		else
		{
			//When Blinking is finished, change the game state accordingly
			switch (selectionTracker)
			{
			case 0:			gameState->setCurrentState(State::LEVEL);		break;
			case 1:			gameState->setCurrentState(State::OPTION);		break;
			case 2:			gameState->setCurrentState(State::CREDITS);		break;
			default:														break;		//No need of case 3 since the window would have already closed
			}
		}
	}

	//Moving background update
	for (unsigned i = 0; i < 2; ++i)
	{
		background[i].move(sf::Vector2f(-100, 0) * dt);
		if (background[i].getPosition().x + background[i].getSize().x <= 0)		//Reset position to the right if hidden left
		{
			background[i].setPosition(sf::Vector2f(window->getSize().x, 0));
			background[i].move(sf::Vector2f(-100, 0) * dt);						//need to move it again or we will have a black line
		}
	}

	//Animated text update
	defonotAnim.animate(dt);
	defonotText.setTextureRect(defonotAnim.getCurrentFrame());
}

void MainMenu::handleInput(float dt)
{
	//Update time passed
	timePassedTracker += dt;

	//Change the selection
	if (!selected)
	{
		if (timePassedTracker > .2f)		//Allow a change of selection every .2 seconds, so that we do not need to be a ninja to select
		{
			//Keyboard selection highlight
			if (input->isKeyDown(sf::Keyboard::Up))
			{
				if (selectionTracker == 0) selectionTracker = 3;
				else --selectionTracker;
				timePassedTracker = 0;
			}
			if (input->isKeyDown(sf::Keyboard::Down))
			{
				if (selectionTracker == 3) selectionTracker = 0;
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
			if (Collision::checkBoundingBox(&quitButton.getGlobalBounds(), mousePos))
				selectionTracker = 3;
		}
	}

	//Make a selection
	if (input->isKeyDown(sf::Keyboard::Enter) || input->isKeyDown(sf::Keyboard::F) || input->isMouseLDown()) selected = true;
}

// Render level
void MainMenu::render()
{
	beginDraw();

	//Draw everything to the screen
	for (unsigned i = 0; i < 2; ++i) window->draw(background[i]);
	window->draw(titleImage);
	window->draw(defonotText);
	window->draw(startButton);
	window->draw(optionButton);
	window->draw(creditButton);
	window->draw(quitButton);

	endDraw();
}

// Begins rendering to the back buffer. Background colour set to black.
void MainMenu::beginDraw()
{
	window->clear(sf::Color(1, 1, 1));
}

// Ends rendering to the back buffer, and swaps buffer to the screen.
void MainMenu::endDraw()
{
	window->display();
}

void MainMenu::initText(sf::Text& txt)
{
	txt.setFont(font);
	txt.setCharacterSize(24);
	txt.setFillColor(sf::Color::White);
	txt.setOrigin(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2);	//Center the text alignment
}

void MainMenu::blinkText(sf::Text& txt, float dt)
{
	if (blinkCount < 5)
	{
		timePassedTracker += dt;
		if (timePassedTracker > .4f)
		{
			if (!isBlinking)
			{
				txt.setFillColor(sf::Color::Transparent);
				isBlinking = true;
				timePassedTracker = 0;
				++blinkCount;
			}
			else
			{
				txt.setFillColor(sf::Color::Yellow);
				isBlinking = false;
				timePassedTracker = 0;
			}
		}
		return;
	}
	hasFinishedBlinking = true;
}