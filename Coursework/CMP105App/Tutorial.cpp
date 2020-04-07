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
	checkpoint = sf::Vector2f(100, 0);

	//Init player
	playerTex.loadFromFile("custom_sprites/NES _Mega_Man.PNG");
	teleportationTex.loadFromFile("custom_sprites/Megaman_Teleportation.PNG");
	player.setTexture(&teleportationTex);
	player.setTextureRect(sf::IntRect(0, 0, 9, 32));
	player.setSize(sf::Vector2f(75, 75));
	player.setInput(input);
	player.setWindow(window);
	player.setPosition(checkpoint);
	player.setVelocity(sf::Vector2f(200, 0));
	player.setCollisionBox(sf::FloatRect(10, 5, 55, 70));
	player.setCollisionBoxColor(sf::Color::Red);
	player.setAlive(false);

	//Init camera
	camera = window->getDefaultView();

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
		//Update time tracker
		timePassedTracker += dt;

		if (!deathParticleManager.haveParticlesSpawned())
		{
			if (timePassedTracker > .5f)
			{
				deathParticleManager.spawnParticles(player.getPosition() + player.getSize() / 2.f);
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
	//the 50 is because of the tile size
	if (player.getPosition().x >= 0 + camera.getSize().x / 2 && player.getPosition().x <= tileManager.getMapSize().x * 50 - camera.getSize().x / 2)
		camera.move(sf::Vector2f(player.getPosition().x - camera.getCenter().x, 0));

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

	//Set the window view
	window->setView(camera);

	//Draw everything to the screen
	tileManager.render(window);
	
	if(!deathParticleManager.haveParticlesSpawned())
		window->draw(player);
	else
		deathParticleManager.render(window);

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
	//Move the player down until it reaches the ground (ground is at map line 13, so 12 lines of tiles of height 50)
	if(player.getPosition().y <= 12.f * 50.f - player.getCollisionBox().height)
		player.move(sf::Vector2f(0, 1000.f) * dt);
	else
	{
		if (player.isTeleportAnimFinished(dt))
		{
			player.setTexture(&playerTex);
			player.setTextureRect(sf::IntRect(0, 8, 24, 24));
			player.setHealth(100);
			player.setAlive(true);
			player.setStates(false, false, true);
			playerSpawned = true;
		}
	}
}

void Tutorial::restartLevel()
{
	//Reset all trackers
	timePassedTracker = 0;
	playerSpawned = false;

	//Reset the player position
	player.setPosition(checkpoint);
	player.setTexture(&teleportationTex);
	player.setTextureRect(sf::IntRect(0, 0, 9, 32));

	//Kill death particles
	deathParticleManager.killAllParticles();

	//Reset the camera
	camera.setCenter(window->getSize().x / 2, window->getSize().y / 2);
}