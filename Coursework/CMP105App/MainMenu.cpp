#include "MainMenu.h"
MainMenu::MainMenu()
{

}

MainMenu::MainMenu(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Init window, input and gameState
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;
	debugUi = dui;

	//Init buttons text strings
	startButton.setString("START GAME");
	optionButton.setString("OPTIONS");
	creditButton.setString("CREDITS");
	quitButton.setString("QUIT");

	//Create buttons
	font.loadFromFile("font/PressStart2P-vaV7.ttf");
	initText(startButton, font);
	initText(optionButton, font);
	initText(creditButton, font);
	initText(quitButton, font);

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

	//Init ALL audio musics and ALL sound effects
	audio->addMusic("sfx/Mega_Man_2_Menu.ogg", "menu");
	audio->addMusic("sfx/Megaman_1_Stage_Select.ogg", "stageSelect");
	audio->addMusic("sfx/Megaman_1_Stage_Start.ogg", "stageStart");
	audio->addMusic("sfx/Megaman_1_Cut_Man_Stage.ogg", "cutMan");
	audio->addSound("sfx/Megaman_1_Select.ogg", "select");
	audio->addSound("sfx/Megaman_1_Press.ogg", "press");
	audio->addSound("sfx/Megaman_1_Change_Selection.ogg", "changeSelection");
	audio->addSound("sfx/Megaman_1_Land.ogg", "land");
	audio->addSound("sfx/Megaman_1_Death.ogg", "death");
	audio->addSound("sfx/Megaman_1_Pause.ogg", "pause");
	audio->addSound("sfx/Megaman_1_Player_Damage.ogg", "damage");
	audio->addSound("sfx/Megaman_1_Shoot.ogg", "shoot");
	audio->addSound("sfx/Megaman_1_Door.ogg", "door");
	audio->addSound("sfx/Megaman_1_Points.ogg", "points");

	//Init trackers
	selectionTracker = 0;
	isBlinking = false;
	hasFinishedBlinking = false;
	blinkCount = 0;
	afkTimeTracker = 0;
	prevSelection = selectionTracker;
}

MainMenu::~MainMenu()
{

}

void MainMenu::update(float dt)
{
	//Play the main menu theme
	if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
	{
		audio->playMusicbyName("menu");
		audio->getMusic()->setLoop(true);
	}

	//Menu Selection update
	if (!selected)
		changeButtonHighlight();
	else
		makeSelection(dt);

	//Moving background update
	for (unsigned i = 0; i < 2; ++i)
		background[i].move(sf::Vector2f(-100, 0) * dt);
	//Instead of moving them one after another each time one is outside of the screen, reset both positions once
	//the first once is out of bounds. That will prevent de-sync
	if (background[0].getPosition().x + background[0].getSize().x <= 0)		//Reset position to the right if hidden left
	{
		background[0].setPosition(sf::Vector2f(0, 0));
		background[1].setPosition(sf::Vector2f(window->getSize().x, 0));
	}

	//Animated text update
	defonotAnim.animate(dt);
	defonotText.setTextureRect(defonotAnim.getCurrentFrame());

	//Debug infos update
	if (debugUi->isDebugging())
		debugUi->updateDebugUi();

	//Change to the intro cinematic after 30s if nothing happens in the meantime
	afkTimeTracker += dt;
	checkAfkTime();
}

void MainMenu::handleInput(float dt)
{
	//Update time passed
	timePassedTracker += dt;

	//Change the selection
	if (!selected)
		changeSelectionTracker();

	//Make a selection
	if (timePassedTracker > .2f)
		if (input->isKeyDown(sf::Keyboard::Enter) || input->isKeyDown(sf::Keyboard::F) || input->isMouseLDown())
		{
			//Selection made, play the selection sound only once
			if(!selected)
				audio->playSoundbyName("select");

			selected = true;
		}
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

	//Draw debug infos if enabled
	if (debugUi->isDebugging())
		window->draw(*debugUi->getUi());

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

void MainMenu::initText(sf::Text& txt, sf::Font& f)
{
	txt.setFont(f);
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
	blinkCount = 0;
}

void MainMenu::setButtonsToWhite()
{
	startButton.setFillColor(sf::Color::White);
	optionButton.setFillColor(sf::Color::White);
	creditButton.setFillColor(sf::Color::White);
	quitButton.setFillColor(sf::Color::White);
}

void MainMenu::changeButtonHighlight()
{
	//Change the button highlight accordingly
	switch (selectionTracker)
	{
	case 0:
		setButtonsToWhite();
		startButton.setFillColor(sf::Color::Yellow);
		break;
	case 1:
		setButtonsToWhite();
		optionButton.setFillColor(sf::Color::Yellow);
		break;
	case 2:
		setButtonsToWhite();
		creditButton.setFillColor(sf::Color::Yellow);
		break;
	case 3:
		setButtonsToWhite();
		quitButton.setFillColor(sf::Color::Yellow);
	default:
		break;
	}
}

void MainMenu::makeSelection(float dt)
{
	//Before making a selection, check if the button has correclty been blinking
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
		case 0:		changeGameState(1);			break;
		case 1:		changeGameState(2);			break;
		case 2:		changeGameState(3);			break;
		default:								break;
		//No need of case 3 since the window would have already closed
		}
	}
}

void MainMenu::changeSelectionTracker()
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

void MainMenu::checkAfkTime()
{
	if (selectionTracker == prevSelection && !selected)
	{
		if (afkTimeTracker >= 30.f)
			changeGameState(4);
	}
	else
	{
		//Selection highlight has changed, play the change sound
		if (!selected)
			audio->playSoundbyName("changeSelection");
		prevSelection = selectionTracker;
		afkTimeTracker = 0;
	}
}

void MainMenu::changeGameState(unsigned state)
{
	//Reset trackers
	prevSelection = selectionTracker;
	afkTimeTracker = 0;
	selected = false;
	timePassedTracker = 0;
	hasFinishedBlinking = false;

	//Stop music unless we are going to the option menu (2) or going back to the main menu (0)
	if (state != 2 && state != 0)
		audio->stopAllMusic();
	
	//switch the game state and change accordingly
	switch (state)
	{
	case 0:	gameState->setCurrentState(State::MENU);			break;
	case 1:	gameState->setCurrentState(State::STAGESELECT);		break;
	case 2:	gameState->setCurrentState(State::OPTION);			break;
	case 3:	gameState->setCurrentState(State::CREDITS);			break;
	case 4: gameState->setCurrentState(State::INTRO);			break;
	case 5: gameState->setCurrentState(State::TUTORIAL);		break;
	default:													break;
	}
}