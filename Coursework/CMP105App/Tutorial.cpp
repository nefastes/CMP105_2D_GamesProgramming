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
	spawnPoint = sf::Vector2f(100, -100);

	//Init player
	playerTex.loadFromFile("custom_sprites/NES _Mega_Man.PNG");
	player.setTexture(&playerTex);
	player.setTextureRect(sf::IntRect(484, 0, 9, 32));
	player.setSize(sf::Vector2f(75, 75));
	player.setInput(input);
	player.setWindow(window);
	player.setAudio(audio);
	player.setPosition(spawnPoint);
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
	counter = 0;

	//Init hints textures
	hintTex[0].loadFromFile("custom_sprites/Move.PNG");
	hintTex[1].loadFromFile("custom_sprites/Jump.PNG");
	hintTex[2].loadFromFile("custom_sprites/Climb.PNG");
	hintTex[3].loadFromFile("custom_sprites/Spikes.PNG");
	hintTex[4].loadFromFile("custom_sprites/Up.PNG");
	hintTex[5].loadFromFile("custom_sprites/Points.PNG");
	hintTex[6].loadFromFile("custom_sprites/25Health.PNG");
	hintTex[7].loadFromFile("custom_sprites/50Health.PNG");
	hintTex[8].loadFromFile("custom_sprites/Health.PNG");
	hintTex[9].loadFromFile("custom_sprites/Door.PNG");
	hintTex[10].loadFromFile("custom_sprites/Score.PNG");
	hintTex[11].loadFromFile("custom_sprites/Shoot.PNG");
	hintTex[12].loadFromFile("custom_sprites/Win.PNG");
	hintTex[13].loadFromFile("custom_sprites/Pause.PNG");

	//Init hints
	hints[0].setTexture(&hintTex[0]);
	hints[0].setSize(sf::Vector2f(200, 100));
	hints[0].setPosition(100, 400);
	hints[1].setTexture(&hintTex[1]);
	hints[1].setSize(sf::Vector2f(200, 100));
	hints[1].setPosition(825, 525);
	hints[2].setTexture(&hintTex[2]);
	hints[2].setSize(sf::Vector2f(200, 100));
	hints[2].setPosition(700, 100);
	hints[3].setTexture(&hintTex[3]);
	hints[3].setSize(sf::Vector2f(200, 100));
	hints[3].setPosition(1325, 400);
	hints[4].setTexture(&hintTex[4]);
	hints[4].setSize(sf::Vector2f(200, 100));
	hints[5].setTexture(&hintTex[5]);
	hints[5].setSize(sf::Vector2f(200, 100));
	hints[6].setTexture(&hintTex[6]);
	hints[6].setSize(sf::Vector2f(200, 100));
	hints[7].setTexture(&hintTex[7]);
	hints[7].setSize(sf::Vector2f(200, 100));
	hints[8].setTexture(&hintTex[8]);
	hints[8].setSize(sf::Vector2f(200, 100));
	hints[9].setTexture(&hintTex[9]);
	hints[9].setSize(sf::Vector2f(200, 100));
	hints[10].setTexture(&hintTex[10]);
	hints[10].setSize(sf::Vector2f(200, 100));
	hints[11].setTexture(&hintTex[11]);
	hints[11].setSize(sf::Vector2f(200, 100));
	hints[12].setTexture(&hintTex[12]);
	hints[12].setSize(sf::Vector2f(200, 100));
	hints[13].setTexture(&hintTex[13]);
	hints[13].setSize(sf::Vector2f(200, 100));

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
	if ((input->isKeyDown(sf::Keyboard::Tab) || input->isKeyDown(sf::Keyboard::Escape)) && player.isAlive() && !gameState->isLevelFinished())
	{
		gameState->setCurrentState(State::PAUSE);
		audio->pauseAllMusic();
		audio->pauseAllSounds();
		audio->playSoundbyName("pause");
		timePassedTracker = 0;
		pauseUi.updatePosition();
	}

	//Debug camera (to check if maps unload correctly
	if (input->isKeyDown(sf::Keyboard::Right))
		camera.move(sf::Vector2f(100, 0) * dt);
	if (input->isKeyDown(sf::Keyboard::Left))
		camera.move(sf::Vector2f(-100, 0) * dt);
	if (input->isKeyDown(sf::Keyboard::Down))
		camera.move(sf::Vector2f(0, 100) * dt);
	if (input->isKeyDown(sf::Keyboard::Up))
		camera.move(sf::Vector2f(0, -100) * dt);
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
			//Transition upwards
			camera.move(sf::Vector2f(0, (int)(-600 * dt)));
			player.freezeControls(true);
			if ((int)(camera.getCenter().y - camera.getSize().y / 2) < tileManager.getMapPosition().y)
			{
				camera.setCenter(sf::Vector2f(tileManager.getMapPosition().x + (int)camera.getSize().x / 2,
					tileManager.getMapPosition().y + (int)camera.getSize().y / 2));
				tileManager.setTransitionning(false);
				player.freezeControls(false);
			}
			break;
		case 2:
			//Transition downwards
			camera.move(sf::Vector2f(0, (int)(600 * dt)));
			player.freezeControls(true);
			if ((int)(camera.getCenter().y - camera.getSize().y / 2) > tileManager.getMapPosition().y)
			{
				camera.setCenter(sf::Vector2f(tileManager.getMapPosition().x + tileManager.getMapSize().x * 50 -
					(int)camera.getSize().x / 2, tileManager.getMapPosition().y + (int)camera.getSize().y / 2));
				tileManager.setTransitionning(false);
				player.freezeControls(false);
			}
			break;
		case 3:
			//Transition left to right
			if ((int)(camera.getCenter().x - camera.getSize().x / 2) >= tileManager.getMapPosition().x)
			{
				camera.setCenter(sf::Vector2f(tileManager.getMapPosition().x + (int)camera.getSize().x / 2,
					tileManager.getMapPosition().y + (int)camera.getSize().y / 2));
				tileManager.setCloseDoor(true);
				player.setMoving(false);
			}
			else if ((int)(camera.getCenter().x - camera.getSize().x / 2) < tileManager.getMapPosition().x)
			{
				camera.move(sf::Vector2f((int)(600 * dt), 0));
				player.move(sf::Vector2f(75 * dt, 0));
				player.setMoving(true);
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

		//Reset the time tracker to 0 if the level is not finished, as we do not use it otherwise
		//If we dont do this the death animation will trigger to soon, as the tracker will be in the range of seconds
		if (!gameState->isLevelFinished())
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

	//Check if the level has been finished (if the win item has been collected)
	if (gameState->isLevelFinished())
	{
		player.freezeControls(true);
		player.setMoving(false);
		if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
		{
			//Count clear points
			if (timePassedTracker >= .05f && counter < 2500)
			{
				counter += 50;
				scoreText.setString("CLEAR POINTS: " + std::to_string(counter));
				scoreText.setOrigin(sf::Vector2f(scoreText.getGlobalBounds().width / 2.f, scoreText.getGlobalBounds().height / 2.f));
				scoreText.setPosition(window->getView().getCenter());
				audio->playSoundbyName("points");
				timePassedTracker = 0;
			}
			else if (timePassedTracker >= 1.f && counter == 2500)
			{
				gameState->addGlobalScore(counter);
				counter += 50;
				timePassedTracker = 0;
			}
			else if (timePassedTracker >= .05f && counter > 2500 && counter < gameState->getGlobalScore())
			{
				counter += 50;
				scoreText.setString("CLEAR POINTS: 2500\n\nTOTAL SCORE: " + std::to_string(counter));
				scoreText.setOrigin(sf::Vector2f(scoreText.getGlobalBounds().width / 2.f, scoreText.getGlobalBounds().height / 2.f));
				scoreText.setPosition(window->getView().getCenter());
				audio->playSoundbyName("points");
				timePassedTracker = 0;
			}
			else if (timePassedTracker >= 3.f && counter == gameState->getGlobalScore())
			{
				restartLevel();
				gameState->setLevelFinished(false);
				gameState->setCurrentState(State::MENU);
			}
		}
	}
	else
	{
		//Update the score
		scoreText.setString(std::to_string(gameState->getGlobalScore()));
		scoreText.setOrigin(scoreText.getGlobalBounds().width / 2.f, scoreText.getGlobalBounds().height / 2.f);
		scoreText.setPosition(window->getView().getCenter().x, window->getView().getCenter().y - 9 * window->getView().getSize().y / 20.f);
	}

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
	
	//Draw hints
	switch (currentMap)
	{
	case 0:
		window->draw(hints[0]);
		window->draw(hints[1]);
		window->draw(hints[2]);
		window->draw(hints[3]);
		break;
	case 1:
		window->draw(hints[4]);
		window->draw(hints[5]);
		window->draw(hints[6]);
		window->draw(hints[7]);
		break;
	case 2:
		window->draw(hints[8]);
		window->draw(hints[9]);
		break;
	case 3:
		window->draw(hints[10]);
		break;
	case 4:
		window->draw(hints[11]);
		window->draw(hints[12]);
		window->draw(hints[13]);
		break;
	default:
		break;
	}

	//Draw any item
	itemManager.renderItems(window);

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
				player.freezeControls(false);
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
	player.setPosition(spawnPoint);
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
	//Spawn items & hints corresponding to the current map section
	itemManager.killAllItems();
	if (currentMap == 0)
	{
		hints[0].setPosition(position + sf::Vector2f(2 * 50, 8 * 50));
		hints[1].setPosition(position + sf::Vector2f(16.5f * 50, 10.5f * 50));
		hints[2].setPosition(position + sf::Vector2f(14 * 50, 2 * 50));
		hints[3].setPosition(position + sf::Vector2f(26.5f * 50, 8 * 50));

	}
	else if (currentMap == 1)
	{
		//This section also has items
		itemManager.spawnItem(position + sf::Vector2f(6 * 50, 7 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(9 * 50, 3 * 50), 1);
		itemManager.spawnItem(position + sf::Vector2f(15 * 50, 3 * 50), 2);
		itemManager.spawnItem(position + sf::Vector2f(18 * 50, 7 * 50), 3);

		hints[4].setPosition(position + sf::Vector2f(16 * 50, 5.5f * 50));
		hints[5].setPosition(position + sf::Vector2f(5 * 50, 5.5f * 50));
		hints[6].setPosition(position + sf::Vector2f(8 * 50, 1.5f * 50));
		hints[7].setPosition(position + sf::Vector2f(13 * 50, 1.5f * 50));

	}
	else if (currentMap == 2)
	{
		hints[8].setPosition(position + sf::Vector2f(3 * 50, 1.5f * 50));
		hints[9].setPosition(position + sf::Vector2f(18* 50, 8 * 50));
	}
	else if (currentMap == 3)
	{
		itemManager.spawnItem(position + sf::Vector2f(4 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(8 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(12 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(16 * 50, 10 * 50), 0);
		itemManager.spawnItem(position + sf::Vector2f(20 * 50, 10 * 50), 0);

		hints[10].setPosition(position + sf::Vector2f(10 * 50, 1 * 50));
	}
	else
	{
		itemManager.spawnItem(position + sf::Vector2f(11 * 50, 2 * 50), 4);
		hints[11].setPosition(position + sf::Vector2f(3 * 50, 8 * 50));
		hints[12].setPosition(position + sf::Vector2f(17 * 50, 8 * 50));
		hints[13].setPosition(position + sf::Vector2f(10 * 50, 4 * 50));
	}
}