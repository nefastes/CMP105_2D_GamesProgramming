#include "Tutorial.h"
Tutorial::Tutorial(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* dui)
{
	//Init stuff from Main()
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;
	debugUi = dui;

	//Send debug infos to tile manager
	tileManager.setDebugRef(debugUi);

	//Create the map
	tileManager.createMap(Maps::TUTORIAL);

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

	//Init trackers
	timePassedTracker = 0;
	playerSpawned = false;
}

Tutorial::~Tutorial()
{

}

void Tutorial::handleInput(float dt)
{
	if (player.isAlive())
	{
		player.handleInput(dt);
	}
}

void Tutorial::update(float dt)
{
	//Only update the objects when necessary
	if (player.isAlive() && playerSpawned)
	{
		//Update objects
		player.update(dt);
		tileManager.update(dt, player);

		//Reset the time tracker (will not be 0 after spawn)
		if (timePassedTracker != 0)
			timePassedTracker = 0;
	}
	//If the player dies, do the following
	else if (playerSpawned)
	{
		//Player dead, stop all musics
		audio->stopAllMusic();

		//Update time tracker
		timePassedTracker += dt;

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
				restartLevel();
		}
	}
	//If both checks failed, it means we are currently spawning the player
	else
	{
		startLevel(dt);
	}

	//Set the camera relatively to the player's horizontal position (megaman games do not follow the player vertically)
	//as an INTEGER (otherwise we will have dead pixels, lines). The 50 is because of the tile size which is 50
	if (player.getCollisionBox().left + player.getCollisionBox().width / 2 >= 0 + camera.getSize().x / 2 &&
		player.getCollisionBox().left + player.getCollisionBox().width / 2 <= tileManager.getMapSize().x * 50 - camera.getSize().x / 2)
		camera.setCenter(sf::Vector2f((int)player.getCollisionBox().left + (int)player.getCollisionBox().width / 2, camera.getCenter().y));
	//Set the window view
	window->setView(camera);

	//Update debug infos
	if (debugUi->isDebugging())
	{
		debugUi->updateDebugUi();
		debugUi->updateUiPos(sf::Vector2f(camera.getCenter() - camera.getSize() / 2.f));
		player.setDebugging(true);
	}
}

// Render level
void Tutorial::render()
{
	beginDraw();

	//Draw everything to the screen
	tileManager.render(window);
	
	//Draw hints
	window->draw(hintMove);
	window->draw(hintJump);
	window->draw(hintClimb);
	window->draw(hintSpike);

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
		//update time tracker
		timePassedTracker += dt;

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
				audio->playSoundbyName("land");
				player.setTextureRect(sf::IntRect(0, 8, 24, 24));
				player.setHealth(100);
				player.setAlive(true);
				playerSpawned = true;
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

	//Reset the player position
	player.setPosition(checkpoint);
	player.setTextureRect(sf::IntRect(484, 0, 9, 32));
	player.resetTeleportAnim();

	//Kill death particles
	deathParticleManager.killAllParticles();

	//Reset the camera as an INTEGER (otherwise we will have dead pixels, lines)
	camera.setCenter((int)window->getSize().x / 2, camera.getCenter().y);
	//Set the window view
	window->setView(camera);

	//Reset the health bar off screen
	player.resetHealthPos(sf::Vector2f(-100, -100));

	//reset the ready text color
	readyText.setFillColor(sf::Color::White);

	//Kill all remaining alive bullets
	player.killAllBullets();
}