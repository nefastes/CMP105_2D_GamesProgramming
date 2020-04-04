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

	//Init objects
	background.setTexture(&backgroundTex);
	background.setTextureRect(sf::IntRect(0, 0, window->getSize().x /2 - 25, window->getSize().y / 2));
	background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	background.setPosition(0, 0);

	//Init trackers
	selectionTracker = 0;
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

	//Make a selection
	if (timePassedTracker > .2f && !selected)
		if (input->isKeyDown(sf::Keyboard::Enter) || input->isKeyDown(sf::Keyboard::F) || input->isMouseLDown())
		{
			selected = true;
			audio->stopAllMusic();

			//Selection made, play the selection sound
			if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
			{
				audio->playMusicbyName("stageStart");
				audio->getMusic()->setLoop(false);
			}
		}
}

void StageMenu::update(float dt)
{
	//Play the main menu theme
	if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped && !selected)
	{
		audio->playMusicbyName("stageSelect");
		audio->getMusic()->setLoop(true);
		//we change it to false when a selection is made, we have to set the loop to true here or it will not loop correctly
	}
}

void StageMenu::render()
{
	beginDraw();

	//Draw everything
	window->draw(background);

	//Debug infos update
	if (debugUi->isDebugging())
		window->draw(*debugUi->getUi());

	endDraw();
}