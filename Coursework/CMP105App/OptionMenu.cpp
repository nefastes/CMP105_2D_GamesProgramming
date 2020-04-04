#include "OptionMenu.h"
OptionMenu::OptionMenu(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Init window, input and gameState
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;
	debugUi = dui;

	//Init font
	font.loadFromFile("font/PressStart2P-vaV7.ttf");

	//Init option titles strings
	frameLimitTitle.setString("FRAME LIMIT:");
	musicVolumeTitle.setString("MUSIC VOLUME:");
	verticalSyncTitle.setString("VERTICAL SYNC:");
	debugModeTitle.setString("ENABLE DEBUG:");

	//Init option titles texts
	initText(frameLimitTitle, font);
	initText(musicVolumeTitle, font);
	initText(verticalSyncTitle, font);
	initText(debugModeTitle, font);

	//Init option titles positions
	frameLimitTitle.setPosition(sf::Vector2f(window->getSize().x / 4, window->getSize().y / 5));
	musicVolumeTitle.setPosition(sf::Vector2f(window->getSize().x / 4, window->getSize().y / 5 + 3 * frameLimitTitle.getGlobalBounds().height));
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
			arrows[i][j].setPosition(sf::Vector2f(3 * window->getSize().x / 4 - 50 + 95 * j, window->getSize().y / 5 + (i * 3) * frameLimitTitle.getGlobalBounds().height));

	//Option infos displays init
	initText(frameLimitDisplay, font);
	initText(musicVolumeDisplay, font);

	//Init background
	backgroundTex.loadFromFile("custom_sprites/megaman_optionscreen.PNG");
	background.setTexture(&backgroundTex);
	background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	background.setPosition(sf::Vector2f(0, 0));

	//Init option infos
	frameLimit = 60;
	musicVolume = 50;
	verticalSync = false;
	debugMode = false;

	//Init trackers
	selectionTracker = 0;
	prevSelection = selectionTracker;
	timePassedTracker = 0;
	selected = false;
	leftArrow = false;
	hasFinishedBlinking = false;
	appliedSettings = true;
}

OptionMenu::~OptionMenu()
{

}

void OptionMenu::update(float dt)
{
	//Update option infos texts:
	//-frame limit
	frameLimitDisplay.setString(std::to_string(frameLimit));
	frameLimitDisplay.setOrigin(frameLimitDisplay.getGlobalBounds().width / 2, frameLimitDisplay.getGlobalBounds().height / 2);
	frameLimitDisplay.setPosition(3 * window->getSize().x / 4, window->getSize().y / 5);
	//-music volume
	musicVolumeDisplay.setString(std::to_string(musicVolume));
	musicVolumeDisplay.setOrigin(musicVolumeDisplay.getGlobalBounds().width / 2, musicVolumeDisplay.getGlobalBounds().height / 2);
	musicVolumeDisplay.setPosition(3 * window->getSize().x / 4, window->getSize().y / 5 + 3 * frameLimitTitle.getGlobalBounds().height);

	//Check if a selection has been done and act accordingly
	if(!selected)
	{
		//If there was no selection, update the current button highlights and change it accordingly
		//to the selection tracker. This will just update the buttons appearance, technical changes in handleInputs()
		changeButtonHighlights();
	}
	else
	{
		//Else, a selection has been made, and therefore we must change the settings accordingly
		//to the button that was pressed (tracked by selectionTracker).
		//We must not allow any change of selection until this has been made, which is why we
		//track the selection with the bool "selected".
		//However, for the "back" and "apply" buttons we want them to blink first
		if (selectionTracker == 6)
			if (!hasFinishedBlinking)
				blinkText(backButton, dt);
			else
				changeSettings();
		else
			changeSettings();
	}

	//Debug infos update
	if (debugUi->isDebugging())
		debugUi->updateDebugUi();
}

void OptionMenu::handleInput(float dt)
{
	//Update time tracker ONLY ONCE here
	timePassedTracker += dt;

	//Change tracking of selection, only if the user did not select anything or selection responses have been finished
	if (!selected)
		trackButtonSelection();

	//Selection check (for the arrows, change effects here directly)
	//Like always, we allow this to happen every .2 seconds to avoid requiring ninja hands to click in this game
	//We also check if if the current selection highlight is not on the first or second option, since those
	//Are to be changed with arrows instead of [use]
	if (timePassedTracker > .2f)
	{
		if (selectionTracker > 1)
		{
			if (input->isKeyDown(sf::Keyboard::Enter) || input->isKeyDown(sf::Keyboard::F) || input->isMouseLDown())
			{
				//For the "back" button play a different sound that others
				if (selectionTracker != 6)
					audio->playSoundbyName("press");
				else
					audio->playSoundbyName("select");
				selected = true;
			}
		}
		else
		{
			//It can sound bizarre to have the bool leftArrow inside the check and setting it as well afterwards,
			//However it is ANDed with the mouse input, and leftArrow is being set accordingly to the mousePos in trackButtonSelection()
			if (input->isKeyDown(sf::Keyboard::Left) || input->isMouseLDown() && leftArrow)
			{
				leftArrow = true;
				selected = true;
				audio->playSoundbyName("press");
			}
			if (input->isKeyDown(sf::Keyboard::Right) || input->isMouseLDown() && !leftArrow)
			{
				leftArrow = false;
				selected = true;
				audio->playSoundbyName("press");
			}
		}
	}
}

// Render level
void OptionMenu::render()
{
	beginDraw();

	//Draw background
	window->draw(background);

	//Draw titles and displays
	window->draw(frameLimitTitle);
	window->draw(frameLimitDisplay);
	window->draw(musicVolumeTitle);
	window->draw(musicVolumeDisplay);
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

	//Debug infos update
	if (debugUi->isDebugging())
		window->draw(*debugUi->getUi());

	endDraw();
}

void OptionMenu::setButtonsToWhite()
{
	//This function name is pretty self explanatory
	//Though we do not reset checkboxes that have been set
	for (unsigned i = 0; i < 2; ++i)
		for (unsigned j = 0; j < 2; ++j)
			arrows[i][j].setFillColor(sf::Color::White);
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
	//Check if changes have been made and set a color accordingly
	if(appliedSettings)
		applyButton.setFillColor(sf::Color::White);
	else
		applyButton.setFillColor(sf::Color::Red);
}

void OptionMenu::applySettings()
{
	//Vertical sync
	if (verticalSync)
	{
		debugUi->setVerticalSync(true);
		window->setVerticalSyncEnabled(true);
		//SFML Vsync only supports up to 60 fps
		if(frameLimit > 60)
			frameLimit = 60;
	}
	else
	{
		debugUi->setVerticalSync(false);
		window->setVerticalSyncEnabled(false);
	}

	//Debug mode
	if (debugMode)
		debugUi->setDebugging(true);
	else
		debugUi->setDebugging(false);

	//Music
	debugUi->setMasterVolume(musicVolume);
	audio->getMusic()->setVolume(musicVolume);

	//Frame rate
	window->setFramerateLimit(frameLimit);

	//Update the apply check
	appliedSettings = true;
}

void OptionMenu::changeButtonHighlights()
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
		if (!verticalSync)
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

void OptionMenu::changeSettings()
{
	//Change the settings accordingly
	if (timePassedTracker > .2f)
	{
		switch (selectionTracker)
		{
		case 0:
			if (leftArrow)
			{
				if (frameLimit > 30)
					frameLimit -= 30;

				//Changes have been made, thus this tracker needs to be updated
				appliedSettings = false;
			}
			else
			{
				if (frameLimit < 240)
					frameLimit += 30;

				//Changes have been made, thus this tracker needs to be updated
				appliedSettings = false;
			}
			break;
		case 1:
			if (leftArrow)
			{
				if (musicVolume > 0)
					musicVolume -= 10;

				//Changes have been made, thus this tracker needs to be updated
				appliedSettings = false;
			}
			else
			{
				if (musicVolume < 100)
					musicVolume += 10;

				//Changes have been made, thus this tracker needs to be updated
				appliedSettings = false;
			}
			break;
		case 2:						//Enable vSync
			if (!verticalSync)
			{
				verticalSync = true;
				checkboxes[0].setTextureRect(sf::IntRect(0, 24, 8, 8));

				//Changes have been made, thus this tracker needs to be updated
				appliedSettings = false;
			}
			else
			{
				verticalSync = false;
				checkboxes[0].setTextureRect(sf::IntRect(0, 8, 8, 8));

				//Changes have been made, thus this tracker needs to be updated
				appliedSettings = false;
			}
			break;
		case 3:						//Enable Debug Mode
			if (!debugMode)
			{
				debugMode = true;
				checkboxes[1].setTextureRect(sf::IntRect(0, 24, 8, 8));

				//Changes have been made, thus this tracker needs to be updated
				appliedSettings = false;
			}
			else
			{
				debugMode = false;
				checkboxes[1].setTextureRect(sf::IntRect(0, 8, 8, 8));

				//Changes have been made, thus this tracker needs to be updated
				appliedSettings = false;
			}
			break;
		case 4:						//Enable Tutorial
			//Change the game state to the tutorial level

			break;
		case 5:						//Apply settings
			applySettings();
			break;
		case 6:
			changeGameState(0);
			break;
		default:
			break;
		}
		selected = false;
		timePassedTracker = 0;
	}
}

void OptionMenu::trackButtonSelection()
{
	//Get mouse position
	sf::Vector2i mousePos = sf::Vector2i(input->getMouseX(), input->getMouseY());

	//Allow a change of selection every .2 seconds, so that we do not need to be a ninja to select
	if (timePassedTracker > .2f)
	{
		//Keyboard selection highlight
		if (input->isKeyDown(sf::Keyboard::Up))
		{
			//Check bounds, adjust to stay in bounds
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
			//We need to use this extra bool in order to know which of the left or right arrow has been pressed
			leftArrow = true;
			selectionTracker = 0;
		}
		if (Collision::checkBoundingBox(&arrows[0][1].getGlobalBounds(), mousePos))
		{
			leftArrow = false;
			selectionTracker = 0;
		}
		if (Collision::checkBoundingBox(&arrows[1][0].getGlobalBounds(), mousePos))
		{
			leftArrow = true;
			selectionTracker = 1;
		}
		if (Collision::checkBoundingBox(&arrows[1][1].getGlobalBounds(), mousePos))
		{
			leftArrow = false;
			selectionTracker = 1;
		}
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

		//If change of selection has been made, play an audio sound
		if (prevSelection != selectionTracker)
		{
			audio->playSoundbyName("changeSelection");
			prevSelection = selectionTracker;
		}
	}
}