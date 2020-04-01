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


	//Init buttons
	howToPlayButton.setString("HOW TO PLAY");
	backButton.setString("BACK");
	applyButton.setString("APPLY");
	for (unsigned i = 0; i < 2; ++i)
		for (unsigned j = 0; j < 2; ++j)
		{
			if (j == 0)
				arrows[i][j].setString("<");
			else
				arrows[i][j].setString(">");
			arrows[i][j].setOrigin(arrows[i][j].getGlobalBounds().width / 2, arrows[i][j].getGlobalBounds().height / 2);
		}
	checkboxTex.loadFromFile("custom_sprites/CheckBox.PNG");
	for (unsigned i = 0; i < 2; ++i)
	{
		checkboxes[i].setTexture(&checkboxTex);
		checkboxes[i].setSize(sf::Vector2f(32, 32));
		checkboxes[i].setOrigin(checkboxes[i].getSize().x / 2, checkboxes[i].getSize().y / 2);
		checkboxes[i].setTextureRect(sf::IntRect(0, 0, 8, 8));
		checkboxes[i].setPosition(sf::Vector2f(3 * window->getSize().x / 4, window->getSize().y / 5 + (6 + i * 3) * frameLimitTitle.getGlobalBounds().height));
	}
			

	//Init buttons texts
	initText(howToPlayButton, font);
	initText(backButton, font);
	initText(applyButton, font);
	for (unsigned i = 0; i < 2; ++i)
		for (unsigned j = 0; j < 2; ++j)
			initText(arrows[i][j], font);

	//Init buttons positions
	howToPlayButton.setPosition(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 5 + 12 * frameLimitTitle.getGlobalBounds().height));
	backButton.setPosition(sf::Vector2f(window->getSize().x / 4, window->getSize().y / 5 + 18 * frameLimitTitle.getGlobalBounds().height));
	applyButton.setPosition(sf::Vector2f(3 * window->getSize().x / 4, window->getSize().y / 5 + 18 * frameLimitTitle.getGlobalBounds().height));
	for (unsigned i = 0; i < 2; ++i)
		for (unsigned j = 0; j < 2; ++j)
			arrows[i][j].setPosition(sf::Vector2f(3 * window->getSize().x / 4 - 50 + 90 * j, window->getSize().y / 5 + (i * 3) * frameLimitTitle.getGlobalBounds().height));

	//Option infos displays init
	initText(frameLimitDisplay, font);

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

	//Init trackers
	selectionTracker = 0;
	timePassedTracker = 0;
	selected = false;
}

OptionMenu::~OptionMenu()
{

}

void OptionMenu::update(float dt)
{
	//Update time tracker
	timePassedTracker += 0;

	//Update option infos texts
	frameLimitDisplay.setString(std::to_string(frameLimit));
	frameLimitDisplay.setOrigin(frameLimitDisplay.getGlobalBounds().width / 2, frameLimitDisplay.getGlobalBounds().height / 2);
	frameLimitDisplay.setPosition(3 * window->getSize().x / 4, window->getSize().y / 5);

	//Check if a selection has been done and act accordingly
	if(!selected)
	{
		//Change the button highlight
		switch (selectionTracker)
		{
		case 0:
			setButtonsToWhite();
			arrows[0][0].setFillColor(sf::Color::Yellow);
			arrows[0][1].setFillColor(sf::Color::Yellow);
			break;
		case 1:
			setButtonsToWhite();
			arrows[1][0].setFillColor(sf::Color::Yellow);
			arrows[1][1].setFillColor(sf::Color::Yellow);
			break;
		case 2:
			setButtonsToWhite();
			if(!verticalSync)
				checkboxes[0].setTextureRect(sf::IntRect(0, 8, 8, 8));
			else
				checkboxes[0].setTextureRect(sf::IntRect(0, 24, 8, 8));
			break;
		case 3:
			setButtonsToWhite();
			if (!debugMode)
				checkboxes[1].setTextureRect(sf::IntRect(0, 8, 8, 8));
			else
				checkboxes[1].setTextureRect(sf::IntRect(0, 24, 8, 8));
			break;
		case 4:
			setButtonsToWhite();
			howToPlayButton.setFillColor(sf::Color::Yellow);
			break;
		case 5:
			setButtonsToWhite();
			applyButton.setFillColor(sf::Color::Yellow);
			break;
		case 6:
			setButtonsToWhite();
			backButton.setFillColor(sf::Color::Yellow);
			break;
		default:
			break;
		}
	}
	else
	{
		if (timePassedTracker > .2f)
		{
			switch (selectionTracker)
			{
			case 0:
				if (leftArrow)
				{
					if (frameLimit > 30)
						frameLimit -= 30;
				}
				else
				{
					if (frameLimit < 240)
						frameLimit += 30;
				}
			case 1:
				break;
			case 2:						//Enable vSync
				if (!verticalSync)
				{
					verticalSync = true;
					checkboxes[0].setTextureRect(sf::IntRect(0, 24, 8, 8));
				}
				else
				{
					verticalSync = false;
					checkboxes[0].setTextureRect(sf::IntRect(0, 8, 8, 8));
				}
				break;
			case 3:						//Enable Debug Mode
				if (!debugMode)
				{
					debugMode = true;
					checkboxes[1].setTextureRect(sf::IntRect(0, 24, 8, 8));
				}
				else
				{
					debugMode = false;
					checkboxes[1].setTextureRect(sf::IntRect(0, 8, 8, 8));
				}
				break;
			case 4:						//Enable Tutorial

				break;
			case 5:						//Aplly settings
				applySettings();
				break;
			case 6:
				gameState->setCurrentState(State::MENU);
				break;
			default:
				break;
			}
			selected = false;
			timePassedTracker = 0;
		}
	}
}

void OptionMenu::handleInput(float dt)
{
	//Update time tracker
	timePassedTracker += dt;

	//Get mouse position
	sf::Vector2i mousePos = sf::Vector2i(input->getMouseX(), input->getMouseY());

	//Change selection
	if (!selected)
	{
		if (timePassedTracker > .2f)		//Allow a change of selection every .2 seconds, so that we do not need to be a ninja to select
		{
			//Keyboard selection highlight
			if (input->isKeyDown(sf::Keyboard::Up))
			{
				if (selectionTracker == 0) selectionTracker = 6;
				else --selectionTracker;
				timePassedTracker = 0;
			}
			if (input->isKeyDown(sf::Keyboard::Down))
			{
				if (selectionTracker == 6) selectionTracker = 0;
				else ++selectionTracker;
				timePassedTracker = 0;
			}
			//Mouse selection highlight
			if (Collision::checkBoundingBox(&arrows[0][0].getGlobalBounds(), mousePos))
			{
				leftArrow = true;
				selectionTracker = 0;
			}
			if (Collision::checkBoundingBox(&arrows[0][1].getGlobalBounds(), mousePos))
			{
				leftArrow = false;
				selectionTracker = 0;
			}
			if (Collision::checkBoundingBox(&arrows[1][0].getGlobalBounds(), mousePos) || Collision::checkBoundingBox(&arrows[1][1].getGlobalBounds(), mousePos))
				selectionTracker = 1;
			if (Collision::checkBoundingBox(&checkboxes[0].getGlobalBounds(), mousePos))
				selectionTracker = 2;
			if (Collision::checkBoundingBox(&checkboxes[1].getGlobalBounds(), mousePos))
				selectionTracker = 3;
			if (Collision::checkBoundingBox(&howToPlayButton.getGlobalBounds(), mousePos))
				selectionTracker = 4;
			if (Collision::checkBoundingBox(&applyButton.getGlobalBounds(), mousePos))
				selectionTracker = 5;
			if (Collision::checkBoundingBox(&backButton.getGlobalBounds(), mousePos))
				selectionTracker = 6;
		}
	}

	//Selection check (for the arrows, change effects here directly)
	if (timePassedTracker > .2f)
		if (input->isKeyDown(sf::Keyboard::Enter) || input->isKeyDown(sf::Keyboard::F) || input->isMouseLDown())
			selected = true;
}

// Render level
void OptionMenu::render()
{
	beginDraw();

	//Draw background
	window->draw(background);

	//Draw titles
	window->draw(frameLimitTitle);
	window->draw(frameLimitDisplay);
	window->draw(windowSizeTitle);
	window->draw(verticalSyncTitle);
	window->draw(debugModeTitle);

	//Draw buttons
	for (unsigned i = 0; i < 2; ++i)
	{
		for (unsigned j = 0; j < 2; ++j)
			window->draw(arrows[i][j]);
		window->draw(checkboxes[i]);
	}
	window->draw(howToPlayButton);
	window->draw(backButton);
	window->draw(applyButton);

	endDraw();
}

void OptionMenu::setButtonsToWhite()
{
	for (unsigned i = 0; i < 2; ++i)
	{
		for (unsigned j = 0; j < 2; ++j)
			arrows[i][j].setFillColor(sf::Color::White);
	}
	if(!verticalSync)
		checkboxes[0].setTextureRect(sf::IntRect(0, 0, 8, 8));
	else
		checkboxes[0].setTextureRect(sf::IntRect(0, 16, 8, 8));
	if(!debugMode)
		checkboxes[1].setTextureRect(sf::IntRect(0, 0, 8, 8));
	else
		checkboxes[1].setTextureRect(sf::IntRect(0, 16, 8, 8));
	howToPlayButton.setFillColor(sf::Color::White);
	backButton.setFillColor(sf::Color::White);
	applyButton.setFillColor(sf::Color::White);
}

void OptionMenu::applySettings()
{
	//Vertical sync
	if (verticalSync)
		window->setVerticalSyncEnabled(true);
	else
		window->setVerticalSyncEnabled(false);

	//Debug mode
	if (debugMode);
	else;

	//Frame rate
	window->setFramerateLimit(frameLimit);
}