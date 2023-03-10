// Main
// Entry point of the application.
// Handles window creation and window events.
// Contains the game loop, updates input class and contains the level objects.
// @author Paul Robertson

#include <iostream>
#include "Level.h"
#include "Framework/AudioManager.h"
#include "DebugUi.h"
#include "MainMenu.h"
#include "OptionMenu.h"
#include "StageMenu.h"
#include "IntroCinematic.h"
#include "CreditCinematic.h"
#include "Tutorial.h"
#include "Sciman.h"

int main()
{
	//Create the window
	sf::RenderWindow window(sf::VideoMode(1200, 700), "CMP105");

	//Initialise window proprieties
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	//Initialise input and level objects.
	Input input;

	//Rng init
	srand(time(NULL));

	//Init audio
	AudioManager audioManager;

	//Game state init
	GameState gameState;
	gameState.setCurrentState(State::INTRO);
	
	//Init UI (main menu, option menu, ...)
	DebugUi debugUi;
	MainMenu menu(&window, &input, &audioManager, &gameState, &debugUi);
	OptionMenu option(&window, &input, &audioManager, &gameState, &debugUi);
	StageMenu stageMenu(&window, &input, &audioManager, &gameState, &debugUi);
	IntroCinematic intro(&window, &input, &audioManager, &gameState, &debugUi);
	CreditCinematic credits(&window, &input, &audioManager, &gameState, &debugUi);

	//Levels
	Tutorial tuto(&window, &input, &audioManager, &gameState, &debugUi);
	Sciman sciman(&window, &input, &audioManager, &gameState, &debugUi);

	// Initialise objects for delta time
	sf::Clock clock;
	float deltaTime;

	// Game Loop
	while (window.isOpen())
	{
		//Store the pressed keys of the last frame
		input.updatePreviousFrameKeys();

		// Handle window events.
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
				break;
			case sf::Event::KeyPressed:
				// update input class
				input.setKeyDown(event.key.code);
				break;
			case sf::Event::KeyReleased:
				//update input class
				input.setKeyUp(event.key.code);
				break;
			case sf::Event::MouseMoved:
				//update input class
				input.setMousePosition(event.mouseMove.x, event.mouseMove.y);
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					//update input class
					input.setMouseLDown(true);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					input.setMouseRDown(true);
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					//update input class
					input.setMouseLDown(false);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					input.setMouseRDown(false);
				}
				break;
			default:
				// don't handle other events
				break;
			}
		}

		// Calculate delta time. How much time has passed 
		// since it was last calculated (in seconds) and restart the clock.
		deltaTime = clock.restart().asSeconds();

		//Thanks to delta time, we can calculate a real time frame rate
		if(debugUi.isDebugging())
			debugUi.setFrameRate(std::round(1.f / deltaTime));

		// Call standard game loop functions (input, update and render)
		switch (gameState.getCurrentState())
		{
		case State::MENU:
			menu.handleInput(deltaTime);
			menu.update(deltaTime);
			menu.render();
			break;

		case State::LEVEL:
			sciman.handleInput(deltaTime);
			sciman.update(deltaTime);
			sciman.render();
			break;

		case State::CREDITS:
			credits.handleInput(deltaTime);
			credits.update(deltaTime);
			credits.render();
			break;

		case State::OPTION:
			option.handleInput(deltaTime);
			option.update(deltaTime);
			option.render();
			break;

		case State::STAGESELECT:
			stageMenu.handleInput(deltaTime);
			stageMenu.update(deltaTime);
			stageMenu.render();
			break;

		case State::INTRO:
			intro.handleInput(deltaTime);
			intro.update(deltaTime);
			intro.render();
			break;
			
		case State::TUTORIAL:
			tuto.handleInput(deltaTime);
			tuto.update(deltaTime);
			tuto.render();
			break;
		case State::PAUSE:
			switch (gameState.getPreviousState())
			{
			case State::LEVEL:				sciman.handlePause(deltaTime); sciman.render();		break;
			case State::TUTORIAL:			tuto.handlePause(deltaTime); tuto.render();			break;
			default:						gameState.setCurrentState(State::MENU);				break;	//Illegal pause, go back to menu
			}
			break;
		}
	}
}

