#include "Tutorial.h"
Tutorial::Tutorial(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Init stuff from Main()
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;
	debugUi = dui;

	//Pass pointers to the itemManager
	itemManager.sendPointers(gameState, audio, &tileManager);

	//Pass pointers to the tile manager
	tileManager.setWindow(window);
	tileManager.setDebugRef(debugUi);
	tileManager.setAudio(audio);

	//Create the map
	tileManager.createMap(Maps::TUTORIAL, 0);
	tileManager.buildCreatedMap(sf::Vector2f(0, 0));

	//Init checkpoints positions
	checkpoint = sf::Vector2f(100, -100);

	//Init player
	playerTex.loadFromFile("custom_sprites/NES _Mega_Man.PNG");
	player.setTexture(&playerTex);
	player.setTextureRect(sf::IntRect(484, 0, 9, 32));
	player.setSize(sf::Vector2f(75, 75));
	player.setInput(input);
	player.setWindow(window);
	player.setAudio(audio);
	player.setPosition(checkpoint);
	player.setVelocity(sf::Vector2f(200, 0));
	player.setCollisionBox(sf::FloatRect(15, 5, 45, 70));
	player.setCollisionBoxColor(sf::Color::Red);
	player.setAlive(false);

	//Init ready text
	font.loadFromFile("font/PressStart2P-vaV7.ttf");
	readyText.setFont(font);
	readyText.setCharacterSize(24);
	readyText.setFillColor(sf::Color::White);
	readyText.setOutlineColor(sf::Color::Black);
	readyText.setOutlineThickness(1);
	readyText.setString("READY");
	readyText.setOrigin(readyText.getGlobalBounds().width / 2, readyText.getGlobalBounds().height / 2);
	readyText.setPosition(window->getSize().x / 2, window->getSize().y / 2);
	readyBlinkCount = 0;
	isReadyBlinking = false;

	//Init score text
	scoreText.setFont(font);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setCharacterSize(24);
	scoreText.setOutlineColor(sf::Color::Black);
	scoreText.setOutlineThickness(1);

	//Init hints textures
	moveTex.loadFromFile("custom_sprites/Move.PNG");
	jumpTex.loadFromFile("custom_sprites/Jump.PNG");
	climbTex.loadFromFile("custom_sprites/Climb.PNG");
	spikeTex.loadFromFile("custom_sprites/Spikes.PNG");
	hintMove.setTexture(&moveTex);
	hintMove.setSize(sf::Vector2f(200, 100));
	hintMove.setPosition(100, 400);
	hintJump.setTexture(&jumpTex);
	hintJump.setSize(sf::Vector2f(200, 100));
	hintJump.setPosition(825, 525);
	hintClimb.setTexture(&climbTex);
	hintClimb.setSize(sf::Vector2f(200, 100));
	hintClimb.setPosition(700, 100);
	hintSpike.setTexture(&spikeTex);
	hintSpike.setSize(sf::Vector2f(200, 100));
	hintSpike.setPosition(1325, 400);

	//Init camera
	camera = window->getView();

	//Init the pause hud
	pauseUi.sendPointers(window, input, gameState, audio);

	//Init trackers
	timePassedTracker = 0;
	playerSpawned = false;
	currentMap = 0;
}

Tutorial::~Tutorial()
{

}

void Tutorial::handleInput(float dt)
{
	//Update the time tracker ONCE here
	timePassedTracker += dt;

	//Player control
	if (player.isAlive())
		player.handleInput(dt);

	//Pause control
	if ((input->isKeyDown(sf::Keyboard::Tab) || input->isKeyDown(sf::Keyboard::Escape)) && timePassedTracker >= .2f && player.isAlive())
	{
		gameState->setCurrentState(State::PAUSE);
		audio->pauseAllMusic();
		audio->pauseAllSounds();
		audio->playSoundbyName("pause");
		timePassedTracker = 0;
		pauseUi.updatePosition();
	}

	//Debug camera (to check if maps unload correctly
	/*
	if (input->isKeyDown(sf::Keyboard::Right))
		camera.move(sf::Vector2f(100, 0) * dt);
	if (input->isKeyDown(sf::Keyboard::Left))
		camera.move(sf::Vector2f(-100, 0) * dt);
	if (input->isKeyDown(sf::Keyboard::Down))
		camera.move(sf::Vector2f(0, 100) * dt);
	if (input->isKeyDown(sf::Keyboard::Up))
		camera.move(sf::Vector2f(0, -100) * dt);
	*/
}

void Tutorial::update(float dt)
{	
	//Change the map section if needed
	if (currentMap > tileManager.getCurrentMap())
	{
		currentMap = tileManager.getCurrentMap();
		tileManager.createMap(Maps::TUTORIAL, currentMap);
		sf::Vector2f position = sf::Vector2f((int)camera.getCenter().x + (int)camera.getSize().x / 2 - (int)tileManager.getMapSize().x * 50,
			(int)camera.getCenter().y + (int)camera.getSize().y / 2);
		tileManager.buildCreatedMap(position);
		spawnItemsInRoom(position);
	}
	else if (currentMap < tileManager.getCurrentMap())
	{
		currentMap = tileManager.getCurrentMap();
		sf::Vector2f position;
		if (tileManager.getTransitionType() != 3)
		{
			tileManager.createMap(Maps::TUTORIAL, currentMap);
			position = sf::Vector2f((int)camera.getCenter().x - (int)camera.getSize().x / 2,
				(int)camera.getCenter().y - 3 * (int)camera.getSize().y / 2);
			tileManager.buildCreatedMap(position);
		}
		else
		{
			tileManager.createMap(Maps::TUTORIAL, currentMap);
			position = sf::Vector2f(tileManager.getMapPosition().x + tileManager.getMapSize().x * 50,
				tileManager.getMapPosition().y);
			tileManager.buildCreatedMap(position);
		}
		spawnItemsInRoom(position);
	}
	if (tileManager.isTransitionning())
	{
		switch (tileManager.getTransitionType())
		{
		case 1:
			if ((int)(camera.getCenter().y - camera.getSize().y / 2) > tileManager.getMapPosition().y)
			{
				camera.move(sf::Vector2f(0, (int)(-600 * dt)));
				player.freezeControls(true);
			}
			else
			{
				camera.setCenter(sf::Vector2f(tileManager.getMapPosition().x + (int)camera.getSize().x / 2,
					tileManager.getMapPosition().y + (int)camera.getSize().y / 2));
				tileManager.setTransitionning(false);
				player.freezeControls(false);
			}
			break;
		case 2:
			if ((int)(camera.getCenter().y - camera.getSize().y / 2) < tileManager.getMapPosition().y)
			{
				camera.move(sf::Vector2f(0, (int)(600 * dt)));
				player.freezeControls(true);
			}
			else
			{
				camera.setCenter(sf::Vector2f(tileManager.getMapPosition().x + tileManager.getMapSize().x * 50 -
					(int)camera.getSize().x / 2, tileManager.getMapPosition().y + (int)camera.getSize().y / 2));
				tileManager.setTransitionning(false);
				player.freezeControls(false);
			}
			break;
		case 3:
			if ((int)(camera.getCenter().x - camera.getSize().x / 2) < tileManager.getMapPosition().x)
			{
				camera.move(sf::Vector2f((int)(600 * dt), 0));
				player.move(sf::Vector2f(75 * dt, 0));
			}
			else
			{
				camera.setCenter(sf::Vector2f(tileManager.getMapPosition().x + (int)camera.getSize().x / 2,
					tileManager.getMapPosition().y + (int)camera.getSize().y / 2));
				tileManager.setCloseDoor(true);
			}
			break;
		}
	}

	//Set the camera relatively to the player's horizontal position (megaman games do not follow the player vertically)
	//as an INTEGER (otherwise we will have dead pixels, lines). The 50 is because of the tile size which is 50
	if (player.getCollisionBox().left + player.getCollisionBox().width / 2 >=
			tileManager.getMapPosition().x + camera.getSize().x / 2 &&
		player.getCollisionBox().left + player.getCollisionBox().width / 2 <=
			tileManager.getMapPosition().x + tileManager.getMapSize().x * 50 - camera.getSize().x / 2)
		camera.setCenter(sf::Vector2f((int)(player.getCollisionBox().left + player.getCollisionBox().width / 2), camera.getCenter().y));
	//Set the window view
	window->setView(camera);

	//Only update the objects when necessary
	//AND most importanly, update them AFTER the view has been updated since they might base calculation on viewPos
	if (player.isAlive() && playerSpawned)
	{
		//Update objects
		player.update(dt);
		tileManager.update(dt, player);

		//Reset the time tracker
		timePassedTracker = 0;
	}
	//If the player dies, do the following
	else if (playerSpawned)
	{
		//Player dead, stop all musics
		audio->stopAllMusic();

		if (!deathParticleManager.haveParticlesSpawned())
		{
			if (timePassedTracker > .5f)
			{
				deathParticleManager.spawnParticles(player.getPosition() + player.getSize() / 2.f);
				audio->playSoundbyName("death");
				timePassedTracker = 0;
			}
		}
		else
		{
			deathParticleManager.update(dt);
			if (timePassedTracker > 4.f)
			{
				if (gameState->getGlobalLives() > 0)
				{
					gameState->subGlobalLives(1);
					restartLevel();
				}
				else
				{
					//TODO: reset all lives, score, etc. GAME OVER, CONTINUE menu ?
					restartLevel();
					gameState->setCurrentState(State::MENU);
				}
			}
		}
	}
	//If both checks failed, it means we are currently spawning the player
	else
	{
		startLevel(dt);
	}

	//Update the score
	scoreText.setString(std::to_string(gameState->getGlobalScore()));
	scoreText.setOrigin(scoreText.getGlobalBounds().width / 2.f, scoreText.getGlobalBounds().height / 2.f);
	scoreText.setPosition(window->getView().getCenter().x, window->getView().getCenter().y - 9 * window->getView().getSize().y / 20.f);

	//Update items
	itemManager.update(dt, player);

	//Update debug infos
	if (debugUi->isDebugging())
	{
		debugUi->updateDebugUi();
		debugUi->updateUiPos(sf::Vector2f(camera.getCenter() - camera.getSize() / 2.f));
		player.setDebugging(true);
		itemManager.setDebugging(true);
	}
}

// Render level
void Tutorial::render()
{
	beginDraw();

	//Draw everything to the screen
	tileManager.render();
	itemManager.renderItems(window);
	
	//Draw hints
	switch (currentMap)
	{
	case 0:
		window->draw(hintMove);
		window->draw(hintJump);
		window->draw(hintClimb);
		window->draw(hintSpike);
		break;
	case 1:
		break;
	case 2:
		break;
	case 4:
		break;
	default:
		break;
	}

	//Draw any potential bullet
	player.renderBullets(window);

	//Draw the player if he is not dead yet (the death particle have not spawned yet)
	if(!deathParticleManager.haveParticlesSpawned())
		window->draw(player);
	else
		deathParticleManager.render(window);

	//Draw level ui stuff
	if(playerSpawned)
		player.drawHealth(window);

	if (!isReadyBlinking)
		window->draw(readyText);

	window->draw(scoreText);

	//Render the pause hud
	if(gameState->getCurrentState() == State::PAUSE) pauseUi.render();

	//Draw debug infos
	if (debugUi->isDebugging())
	{
		window->draw(*debugUi->getUi());
		window->draw(*player.getDebugObjectSize());
		window->draw(*player.getDebugCollisionBox());
	}

	endDraw();
}

// Begins rendering to the back buffer. Background colour set to black.
void Tutorial::beginDraw()
{
	window->clear(sf::Color(1, 1, 1));
}

// Ends rendering to the back buffer, and swaps buffer to the screen.
void Tutorial::endDraw()
{
	window->display();
}

void Tutorial::startLevel(float dt)
{
	//Play the music if it was stopped
	if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
	{
		audio->playMusicbyName("cutMan");
		//The music has a small intro at the beginning, so we have to set loop points
		audio->getMusic()->setLoopPoints(sf::Music::TimeSpan(sf::seconds(3.36f), sf::seconds(41.698f)));
	}

	if (readyBlinkCount < 4)
	{
		if (timePassedTracker >= .4f)
		{
			if (!isReadyBlinking)
			{
				readyText.setFillColor(sf::Color::Transparent);
				isReadyBlinking = true;
				timePassedTracker = 0;
				++readyBlinkCount;
			}
			else
			{
				readyText.setFillColor(sf::Color::White);
				isReadyBlinking = false;
				timePassedTracker = 0;
			}
		}
	}
	else
	{
		//Make ready text invisible
		isReadyBlinking = true;

		//Move the player down until it reaches the ground (ground is at map line 13, so 12 lines of tiles of height 50)
		if (player.getPosition().y <= 12.f * 50.f - player.getCollisionBox().height)
			player.move(sf::Vector2f(0, 1000.f) * dt);
		else
		{
			if (player.isTeleportAnimFinished(dt))
			{
				audio->playSoundbyName("tpLand");
				player.setTextureRect(sf::IntRect(0, 8, 24, 24));
				player.setHealth(25);
				player.setAlive(true);
				playerSpawned = true;
				timePassedTracker = 0;
			}
		}
	}
}

void Tutorial::restartLevel()
{
	//Reset all trackers
	timePassedTracker = 0;
	playerSpawned = false;
	isReadyBlinking = false;
	readyBlinkCount = 0;

	//Reset the map
	currentMap = 0;
	tileManager.setCurrentMap(0);
	tileManager.createMap(Maps::TUTORIAL, currentMap);
	tileManager.buildCreatedMap(sf::Vector2f(0, 0));

	//Reset origin to default and the player position
	player.resetSettings();
	player.setPosition(checkpoint);
	player.setTextureRect(sf::IntRect(484, 0, 9, 32));
	player.resetTeleportAnim();
	player.setAlive(false);

	//Kill death particles
	deathParticleManager.killAllParticles();

	//Reset the camera as an INTEGER (otherwise we will have dead pixels, lines)
	camera.setCenter((int)window->getSize().x / 2, (int)window->getSize().y / 2);
	//Set the window view
	window->setView(camera);

	//Reset the health bar off screen
	player.resetHealthPos(sf::Vector2f(-100, -100));

	//reset the ready text color
	readyText.setFillColor(sf::Color::White);

	//Kill all remaining alive bullets
	player.killAllBullets();
}

void Tutorial::handlePause(float dt)
{
	//Handle selection inputs
	pauseUi.handleInput(dt);

	//If the game state change to MENU, we need to reset the level
	if(gameState->getCurrentState() == State::MENU) restartLevel();
}

void Tutorial::spawnItemsInRoom(sf::Vector2f position)
{
	//Spawn items corresponding to the current map section
	itemManager.killAllItems();
	if (currentMap == 1)
	{
		//This section also has items
		itemManager.spawnItem(position + sf::Vector2f(6 * 50, 7 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(9 * 50, 3 * 50), 1);
		itemManager.spawnItem(position + sf::Vector2f(15 * 50, 3 * 50), 2);
		itemManager.spawnItem(position + sf::Vector2f(18 * 50, 7 * 50), 3);
	}
	else if (currentMap == 3)
	{
		itemManager.spawnItem(position + sf::Vector2f(4 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(6 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(8 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(10 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(12 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(14 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(16 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(18 * 50, 10 * 50), 0);
	}
	else if (currentMap == 4)
	{
		itemManager.spawnItem(position + sf::Vector2f(11 * 50, 2 * 50), 4);
	}
}