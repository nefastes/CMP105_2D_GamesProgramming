#include "Level.h"
Level::Level()
{
	//Init stuff from Main()
	window = nullptr;
	input = nullptr;
	audio = nullptr;
	gameState = nullptr;
	debugUi = nullptr;

	//By default the current level is the tutorial
	//This needs to be changed in each level accordingly
	currentLevel = Maps::TUTORIAL;

	//Init spawnpoint position
	spawnPoint = sf::Vector2f(2, 12);
	spawnMap = 0;

	//Init player
	playerTex.loadFromFile("custom_sprites/NES _Mega_Man.PNG");
	player.setTexture(&playerTex);
	player.setTextureRect(sf::IntRect(484, 0, 9, 32));
	player.setSize(sf::Vector2f(75, 75));
	player.setPosition(sf::Vector2f(spawnPoint.x * 50, -100));
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
	readyBlinkCount = 0;
	isReadyBlinking = false;

	//Init score text
	scoreText.setFont(font);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setCharacterSize(24);
	scoreText.setOutlineColor(sf::Color::Black);
	scoreText.setOutlineThickness(1);
	counter = 0;

	//Init trackers
	timePassedTracker = 0;
	playerSpawned = false;
	currentMap = 0;
}

Level::~Level()
{

}

void Level::initLevel(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Init things from main()
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;
	debugUi = dui;

	//Init camera
	camera = window->getView();

	//Create the map
	tileManager.createMap(currentLevel, 0);
	tileManager.buildCreatedMap(sf::Vector2f(0, 0));

	//Pass pointers to the itemManager
	itemManager.sendPointers(gameState, audio, &tileManager);

	//Pass pointers to the tile manager
	tileManager.setWindow(window);
	tileManager.setDebugRef(debugUi);
	tileManager.setAudio(audio);

	//Pass pointers to the player
	player.setInput(input);
	player.setWindow(window);
	player.setAudio(audio);

	//Init the pause hud
	pauseUi.sendPointers(window, input, gameState, audio);
}

// handle user input
void Level::handleLevelInput(float dt)
{
	//Update the time tracker ONCE here
	timePassedTracker += dt;

	//Handle inputs of game objects
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
}

// Update game objects
void Level::updateLevel(float dt)
{
	//Change the map section if needed
	if (currentMap > tileManager.getCurrentMap())
	{
		currentMap = tileManager.getCurrentMap();
		tileManager.createMap(currentLevel, currentMap);
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
			tileManager.createMap(currentLevel, currentMap);
			position = sf::Vector2f((int)camera.getCenter().x - (int)camera.getSize().x / 2,
				(int)camera.getCenter().y - 3 * (int)camera.getSize().y / 2);
			tileManager.buildCreatedMap(position);
		}
		else
		{
			tileManager.createMap(currentLevel, currentMap);
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
	//We also do not want the view to change in a transition, so check for that as well
	if ((player.getCollisionBox().left + player.getCollisionBox().width / 2 >=
		tileManager.getMapPosition().x + camera.getSize().x / 2 &&
		player.getCollisionBox().left + player.getCollisionBox().width / 2 <=
		tileManager.getMapPosition().x + tileManager.getMapSize().x * 50 - camera.getSize().x / 2) && !tileManager.isTransitionning())
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
					resetLevel();
				}
				else
				{
					//Reset all lives, score, etc.
					//TODO: GAME OVER, CONTINUE menu ?
					resetLevel();
					gameState->setGlobalLives(2);
					gameState->setGlobalScore(0);
					gameState->setCurrentState(State::MENU);
				}
			}
		}
	}
	//If both checks failed, it means we are currently spawning the player
	else
	{
		readyText.setPosition(window->getSize().x / 2, window->getSize().y / 2);
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
			else if (timePassedTracker >= 1.f && counter == 2500 && gameState->getGlobalScore() != 2500)
			{
				if (gameState->getGlobalScore() == 0)
				{
					scoreText.setString("CLEAR POINTS: 2500\n\nTOTAL SCORE: 2500");
					scoreText.setOrigin(sf::Vector2f(scoreText.getGlobalBounds().width / 2.f, scoreText.getGlobalBounds().height / 2.f));
					scoreText.setPosition(window->getView().getCenter());
					audio->playSoundbyName("points");
				}
				else counter += 100;
				gameState->addGlobalScore(counter);
				timePassedTracker = 0;
			}
			else if (timePassedTracker >= .05f && counter > 2500 && counter < gameState->getGlobalScore())
			{
				counter += 100;
				scoreText.setString("CLEAR POINTS: 2500\n\nTOTAL SCORE: " + std::to_string(counter));
				scoreText.setOrigin(sf::Vector2f(scoreText.getGlobalBounds().width / 2.f, scoreText.getGlobalBounds().height / 2.f));
				scoreText.setPosition(window->getView().getCenter());
				audio->playSoundbyName("points");
				timePassedTracker = 0;
			}
			else if (timePassedTracker >= 3.f && counter == gameState->getGlobalScore())
			{
				spawnPoint = sf::Vector2f(2, 12);
				spawnMap = 0;		//Reset the spawn location or it will remain the same
				resetLevel();
				gameState->setLevelFinished(false);
				gameState->setCurrentState(State::STAGESELECT);
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

		//Update debug hitboxes of bullets
		std::vector<Bullet*> temp = player.getBullets();
		for (unsigned i = 0; i < temp.size(); ++i)
			if(temp[i]->isAlive())
				temp[i]->updateDebugBoxes();
	}
}

// Render level
void Level::renderLevel()
{
	beginDraw();

	//Draw tiles to the screen
	tileManager.render();

	//Draw any item
	itemManager.renderItems(window);

	//Render any alive enemy
	renderEnemies(window);

	//Draw any potential bullet
	player.renderBullets(window);

	//Draw the player if he is not dead yet (the death particle have not spawned yet)
	if (!deathParticleManager.haveParticlesSpawned())
		window->draw(player);
	else
		deathParticleManager.render(window);

	//Draw level ui stuff
	if (playerSpawned)
		player.drawHealth(window);

	if (!isReadyBlinking)
		window->draw(readyText);

	window->draw(scoreText);

	//Render the pause hud
	if (gameState->getCurrentState() == State::PAUSE) pauseUi.render();

	//Draw debug infos
	if (debugUi->isDebugging())
	{
		window->draw(*debugUi->getUi());
		window->draw(*player.getDebugObjectSize());
		window->draw(*player.getDebugCollisionBox());

		//Render debug hitboxes of bullets
		std::vector<Bullet*> temp = player.getBullets();
		for (unsigned i = 0; i < temp.size(); ++i)
			if (temp[i]->isAlive())
				window->draw(*temp[i]->getDebugCollisionBox());
	}

	endDraw();
}

// Begins rendering to the back buffer. Background colour set to black.
void Level::beginDraw()
{
	window->clear(sf::Color(1, 1, 1));
}

// Ends rendering to the back buffer, and swaps buffer to the screen.
void Level::endDraw()
{
	window->display();
}

void Level::startLevel(float dt)
{
	//Play the music if it was stopped
	if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
	{
		switch (currentLevel)
		{
		case Maps::TUTORIAL:
			audio->playMusicbyName("cutMan");
			//The music has a small intro at the beginning, so we have to set loop points
			audio->getMusic()->setLoopPoints(sf::Music::TimeSpan(sf::seconds(3.36f), sf::seconds(41.698f)));
			audio->getMusic()->setLoop(true);
			break;
		case Maps::SCIMAN:
			audio->playMusicbyName("elecMan");
			//The music has a small intro at the beginning, so we have to set loop points
			audio->getMusic()->setLoopPoints(sf::Music::TimeSpan(sf::seconds(3.76f), sf::seconds(42.097f)));
			audio->getMusic()->setLoop(true);
			break;
		}
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

		//Move the player down until it reaches the spawnPoint
		if (player.getPosition().y <= spawnPoint.y * 50.f - player.getCollisionBox().height)
			player.move(sf::Vector2f(0, 1000.f) * dt);
		else
		{
			if (player.isTeleportAnimFinished(dt))
			{
				audio->playSoundbyName("tpLand");
				player.setTextureRect(sf::IntRect(0, 8, 24, 24));
				player.setAlive(true);
				player.freezeControls(false);
				playerSpawned = true;
				timePassedTracker = 0;
			}
		}
	}
}

void Level::resetLevel()
{
	//Reset all trackers
	timePassedTracker = 0;
	playerSpawned = false;
	isReadyBlinking = false;
	readyBlinkCount = 0;

	//Reset the map
	currentMap = spawnMap;
	tileManager.setCurrentMap(spawnMap);
	tileManager.createMap(currentLevel, currentMap);
	tileManager.buildCreatedMap(sf::Vector2f(0, 0));

	//Reset origin to default and the player position
	player.resetSettings();
	player.setPosition(sf::Vector2f(spawnPoint.x * 50, -100));
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

	//Respawn items
	spawnItemsInRoom(sf::Vector2f(0, 0));
}

void Level::handleLevelPause(float dt)
{
	//Handle selection inputs
	pauseUi.handleInput(dt);

	//If the game state change to MENU, we need to reset the level
	if (gameState->getCurrentState() == State::MENU) resetLevel();
}

void Level::spawnItemsInRoom(sf::Vector2f position)
{

}

void Level::renderEnemies(sf::RenderWindow* window)
{

}