#include "StageMenu.h"
StageMenu::StageMenu(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Init things from main()
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;
	debugUi = dui;

	//Init textures
	backgroundTex.loadFromFile("custom_sprites/megaman_stage_select.PNG");
	backgroundTex.setRepeated(true);
	boxesTex.loadFromFile("custom_sprites/NES_Mega_Man_Stage_Select.PNG");

	//Init Backgrounds
	background.setTexture(&backgroundTex);
	background.setTextureRect(sf::IntRect(0, 0, window->getSize().x /2 - 25, window->getSize().y / 2));
	background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	background.setPosition(0, 0);
	blueLine.setSize(sf::Vector2f(window->getSize().x, window->getSize().y / 2.f));
	blueLine.setOrigin(blueLine.getSize() / 2.f);
	blueLine.setPosition(window->getSize().x / 2.f, window->getSize().y / 2.f);
	blueLine.setFillColor(sf::Color::Blue);

	//Init Selection Boxes (1 is highlighted by default)
	for (unsigned i = 0; i < 6; ++i)
	{
		selectionBoxes[i].setTexture(&boxesTex);
		selectionBoxes[i].setTextureRect(sf::IntRect(49, 1, 45, 45));
		selectionBoxes[i].setSize(sf::Vector2f(128, 128));
		selectionBoxes[i].setOrigin(sf::Vector2f(selectionBoxes[i].getSize().x / 2, selectionBoxes[i].getSize().y / 2));
	}
	selectionBoxes[0].setTextureRect(sf::IntRect(2, 1, 45, 45));
	selectionBoxes[0].setPosition(2 * window->getSize().x / 5, window->getSize().y / 4);
	selectionBoxes[1].setPosition(3 * window->getSize().x / 5, window->getSize().y / 4);
	selectionBoxes[2].setPosition(4 * window->getSize().x / 6, 2 * window->getSize().y / 4);
	selectionBoxes[3].setPosition(3 * window->getSize().x / 5, 3 * window->getSize().y / 4);
	selectionBoxes[4].setPosition(2 * window->getSize().x / 5, 3 * window->getSize().y / 4);
	selectionBoxes[5].setPosition(2 * window->getSize().x / 6, 2 * window->getSize().y / 4);

	//Init bosses images
	for (unsigned i = 0; i < 6; ++i)
	{
		bossImages[i].setTexture(&boxesTex);
		bossImages[i].setTextureRect(sf::IntRect(1, 51, 32, 32));
		bossImages[i].setSize(sf::Vector2f(95, 95));
		bossImages[i].setOrigin(sf::Vector2f(bossImages[i].getSize().x / 2, bossImages[i].getSize().y / 2));
		bossImages[i].setPosition(selectionBoxes[i].getPosition());
	}
	bossImages[1].setTextureRect(sf::IntRect(137, 17, 32, 32));

	//Init the boss names
	font.loadFromFile("font/PressStart2P-vaV7.ttf");
	for (unsigned i = 0; i < 6; ++i)
	{
		bossNames[i].setString("UNAVAILABLE");
		initText(bossNames[i], font);
		//For the 3 names of the 3 boxes on the right, display the name on the right. For the others on the left
		if (i >= 1 && i <= 3)
			bossNames[i].setPosition(selectionBoxes[i].getPosition().x+ bossNames[i].getGlobalBounds().width, selectionBoxes[i].getPosition().y);
		else
			bossNames[i].setPosition(selectionBoxes[i].getPosition().x - bossNames[i].getGlobalBounds().width, selectionBoxes[i].getPosition().y);
	}
	bossNames[0].setFillColor(sf::Color::Yellow);
	bossNames[1].setString("SCIMAN");

	//Init the selected boss texts
	bossName.setString("");
	clearPointsText.setString("CLEAR POINTS:\n0");
	initText(bossName, font);
	initText(clearPointsText, font);
	bossName.setPosition(window->getSize().x / 6.f, window->getSize().y / 2.f);
	clearPointsText.setPosition(3 * window->getSize().x / 4.f, window->getSize().y / 2.f);
	clearPoints = 0;
	bossLanded = false;

	//Init bosses
	scimanTex.loadFromFile("custom_sprites/Sciman.PNG");
	sciman.setTexture(&scimanTex);
	sciman.setTextureRect(sf::IntRect(0, 32, 32, 32));
	sciman.setVelocity(sf::Vector2f(0, -200));
	sciman.setSize(sf::Vector2f(128, 128));
	sciman.setOrigin(sciman.getSize() / 2.f);
	sciman.setPosition(selectionBoxes[1].getPosition());
	scimanIdle.addFrame(sf::IntRect(0, 0, 32, 32));
	scimanIdle.addFrame(sf::IntRect(32, 0, 32, 32));
	scimanIdle.setLooping(true);
	scimanIdle.setFrameSpeed(1.f / 6.f);

	//Init trackers
	selectionTracker = 0;
	prevSelection = selectionTracker;
	timePassedTracker = 0;
	selected = false;
}

StageMenu::~StageMenu()
{

}

void StageMenu::handleInput(float dt)
{
	//Update time passed
	timePassedTracker += dt;

	//Change selection tracker
	if (!selected)
		changeBoxSelection();

	//Make a selection
	if (timePassedTracker > .2f && !selected && selectionTracker == 1)
		if (input->isKeyDown(sf::Keyboard::Enter) || input->isKeyDown(sf::Keyboard::F) || input->isMouseLDown())
		{
			//Set the selected tracker and play the selection sound
			selected = true;
			audio->stopAllMusic();
			audio->playSoundbyName("select");

			//Selection made, play the selection music
			if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
			{
				audio->playMusicbyName("stageStart");
				audio->getMusic()->setLoop(false);
			}
		}
}

void StageMenu::update(float dt)
{
	//Play the stage selection theme
	if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped && !selected)
	{
		audio->playMusicbyName("stageSelect");
		audio->getMusic()->setLoop(true);
	}

	//If a selection has not been yet made, allow the change of selection
	if (!selected)
		changeBoxHighlight();
	else
	{
		selectStage(dt);
	}

	//Debug infos update
	if (debugUi->isDebugging())
		debugUi->updateDebugUi();
}

void StageMenu::render()
{
	beginDraw();

	//Draw everything
	window->draw(background);
	for (unsigned i = 0; i < 6 && !selected && gameState->getCurrentState() != State::LEVEL; ++i)
	{
		window->draw(bossImages[i]);
		window->draw(selectionBoxes[i]);
		window->draw(bossNames[i]);
	}
	if (selected)
	{
		window->draw(blueLine);
		if (bossLanded)
		{
			window->draw(bossName);
			window->draw(clearPointsText);
		}
		window->draw(sciman);
	}

	//Debug infos update
	if (debugUi->isDebugging())
		window->draw(*debugUi->getUi());

	endDraw();
}

void StageMenu::changeBoxHighlight()
{
	setButtonsToWhite();
	if (timePassedTracker <= .5f)
	{
		selectionBoxes[selectionTracker].setTextureRect(sf::IntRect(2, 1, 45, 45));
		bossNames[selectionTracker].setFillColor(sf::Color::Yellow);
	}
	if (timePassedTracker >= 1.f)
		timePassedTracker = 0;
}

void StageMenu::changeBoxSelection()
{
	if (timePassedTracker > .2f)		//Allow a change of selection every .2 seconds, so that we do not need to be a ninja to select
	{
		//Keyboard selection highlight
		if (input->isKeyDown(sf::Keyboard::Left))
		{
			if (selectionTracker == 0) selectionTracker = 5;
			else --selectionTracker;
		}
		if (input->isKeyDown(sf::Keyboard::Right))
		{
			if (selectionTracker == 5) selectionTracker = 0;
			else ++selectionTracker;
		}

		//Mouse selection highlight
		sf::Vector2i mousePos = sf::Vector2i(input->getMouseX(), input->getMouseY());
		for (unsigned i = 0; i < 6; ++i)
			if (Collision::checkBoundingBox(&selectionBoxes[i].getGlobalBounds(), mousePos))
				selectionTracker = i;
		
		//Check if a selection has been changed, play the change sound and reset the prevSelection tracker
		if (prevSelection != selectionTracker)
		{
			audio->playSoundbyName("changeSelection");
			prevSelection = selectionTracker;
			timePassedTracker = 0;
		}
	}
}

void StageMenu::selectStage(float dt)
{
	switch (selectionTracker)
	{
	case 0:																					break;
	case 1:			selectBoss(dt, &sciman);			bossName.setString("SCIMAN");		break;
	case 2:																					break;
	case 3:																					break;
	case 4:																					break;
	case 5:																					break;
	default:																				break;
	}

	if (bossLanded)
	{
		if (audio->getMusic()->getPlayingOffset() >= sf::milliseconds(4500) || audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
		{
			//Update clear points
			if (clearPoints < 2500 && timePassedTracker >= .05f)
			{
				audio->playSoundbyName("points");
				clearPoints += 50;
				timePassedTracker = 0;
			}
			else if (timePassedTracker >= 2.f)
			{
				//Reset all trackers and change the game state
				selectionTracker = 0;
				prevSelection = selectionTracker;
				timePassedTracker = 0;
				selected = false;
				clearPoints = 0;
				bossLanded = false;
				bossName.setString("");
				clearPointsText.setString("CLEAR POINTS:\n0");
				sciman.setPosition(selectionBoxes[1].getPosition());
				sciman.setVelocity(sf::Vector2f(0, -200));
				gameState->setCurrentState(State::LEVEL);
			}
			clearPointsText.setString("CLEAR POINTS:\n" + std::to_string(clearPoints));
		}

		//Animate the boss
		switch (selectionTracker)
		{
		case 0:																										break;
		case 1:			scimanIdle.animate(dt);		sciman.setTextureRect(scimanIdle.getCurrentFrame());			break;
		case 2:																										break;
		case 3:																										break;
		case 4:																										break;
		case 5:																										break;
		default:																									break;
		}
	}
}

void StageMenu::selectBoss(float dt, GameObject* boss)
{
	//Move the boss to the correct location
	if (boss->getPosition().y < window->getSize().y / 2.f || boss->getVelocity().y < 0)
	{
		boss->move(sf::Vector2f(window->getSize().x / 3 - boss->getPosition().x, sciman.getVelocity().y) * dt);
		boss->setVelocity(sciman.getVelocity().x, sciman.getVelocity().y + 980 * dt);
	}
	else bossLanded = true;
}

void StageMenu::setButtonsToWhite()
{
	for (unsigned i = 0; i < 6; ++i)
	{
		selectionBoxes[i].setTextureRect(sf::IntRect(49, 1, 45, 45));
		bossNames[i].setFillColor(sf::Color::White);
	}
}