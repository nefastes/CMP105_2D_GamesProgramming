#include "IntroCinematic.h"
IntroCinematic::IntroCinematic(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* ui)
{
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;
	debugUi = ui;

	//Init blackscreen
	blackScreen.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	blackScreen.setPosition(0, 0);
	blackScreen.setFillColor(sf::Color(0, 0, 0, 255));

	//Init Logo stuff
	logoTex.loadFromFile("custom_sprites/Infogrames_logo.PNG");
	logoDisplay.setTexture(&logoTex);
	logoDisplay.setSize(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));
	logoDisplay.setPosition(window->getSize().x / 4, window->getSize().y / 4);

	//Init House stuff
	houseBackgroundTex.loadFromFile("custom_sprites/megaman_house_background.PNG");
	houseFrontgroundTex.loadFromFile("custom_sprites/megaman_house_isolated.PNG");
	houseBackground.setTexture(&houseBackgroundTex);
	houseBackground.setSize(sf::Vector2f(1.5f * window->getSize().x, 2 * window->getSize().y / 3));
	houseBackground.setPosition(-.5f * window->getSize().x, 0);
	houseFrontground.setTexture(&houseFrontgroundTex);
	houseFrontground.setSize(sf::Vector2f(1.5f * window->getSize().x, 2 * window->getSize().y / 3));
	houseFrontground.setPosition(0, 0);

	//Init Conflict stuff
	confictBackgroundTex.loadFromFile("custom_sprites/megaman_laboratory.PNG");
	conflictWilyTex.loadFromFile("custom_sprites/drWily.PNG");
	conflictLightTex.loadFromFile("custom_sprites/drLight.PNG");
	conflictBackground.setTexture(&confictBackgroundTex);
	conflictBackground.setSize(sf::Vector2f(1.1f * window->getSize().x, 2 * window->getSize().y / 3));
	conflictBackground.setPosition(-.1f * window->getSize().x, 0);
	conflictWily.setTexture(&conflictWilyTex);
	conflictWily.setSize(sf::Vector2f(128, 128));
	conflictWily.setPosition(3 * window->getSize().x / 4, window->getSize().y / 2 - 45);
	conflictLight.setTexture(&conflictLightTex);
	conflictLight.setSize(sf::Vector2f(86, 128));
	conflictLight.setPosition(window->getSize().x / 4, window->getSize().y / 2 - 55);

	//Init Fortress stuff
	fortressBackgroundTex.loadFromFile("custom_sprites/megaman_fortress.PNG");
	fortressFrontgroundTex.loadFromFile("custom_sprites/megaman_fortress_front.PNG");
	fortressLightningTex.loadFromFile("custom_sprites/Lightning.PNG");
	fortressBackground.setTexture(&fortressBackgroundTex);
	fortressBackground.setSize(sf::Vector2f(1.1f * window->getSize().x, 2 * window->getSize().y / 3));
	fortressBackground.setPosition(-.1f * window->getSize().x, 0);
	fortressFrontground.setTexture(&fortressFrontgroundTex);
	fortressFrontground.setSize(sf::Vector2f(1.1f * window->getSize().x, 2 * window->getSize().y / 3));
	fortressFrontground.setPosition(0, 0);
	fortressLightning.setTexture(&fortressLightningTex);
	fortressLightning.setSize(sf::Vector2f(32, 32));
	fortressLightning.setPosition(100, 100);
	lightningBolt.addFrame(sf::IntRect(0, 0, 16, 16));
	lightningBolt.addFrame(sf::IntRect(0, 16, 16, 16));
	lightningBolt.setFrameSpeed(1.f / 6.f);

	//Init Final scene
	megamanTex.loadFromFile("custom_sprites/NES _Mega_Man.PNG");
	laserTex.loadFromFile("custom_sprites/NES_Mega_Man_Laser.PNG");
	finalMegaman.setTexture(&megamanTex);
	finalMegaman.setTextureRect(sf::IntRect(0, 8, 24, 24));
	finalMegaman.setSize(sf::Vector2f(105, 120));
	//Change the origin of the sprite to it's center (it's the only one with this change !!)
	finalMegaman.setOrigin(finalMegaman.getSize().x / 2, finalMegaman.getSize().y / 2);
	finalMegaman.setPosition(3 * window->getSize().x / 4 + finalMegaman.getSize().x / 2, window->getSize().y / 2 - 45 + finalMegaman.getSize().y / 2);
	finalLaser.setTexture(&laserTex);
	finalLaser.setSize(sf::Vector2f(28, 160));
	finalLaser.setPosition(-100, -100);

	//Init the message
	font.loadFromFile("font/PressStart2P-vaV7.ttf");
	message.setFont(font);
	message.setCharacterSize(24);
	message.setLineSpacing(1.5f);
	message.setString(sentence[0]);
	message.setPosition(window->getSize().x / 20, 3 * window->getSize().y / 4);

	//Init cinematic trackers
	logo = false;
	house = false;
	conflict = false;
	fortress = false;
	megaman = false;
	megaFront = false;
	megaJump = false;

	//Init other trackers
	timePassedTracker = 0;

	//Init the fading vars
	alpha = 255;
	hasFadeIn = false;
	isFadingIn = false;
	fadeTimeTracker = 0;
	lightningTimeTracker = 0;
}

IntroCinematic::~IntroCinematic()
{

}

void IntroCinematic::handleInput(float dt)
{
	if (input->isKeyDownOnce(sf::Keyboard::Enter) || input->isMouseLDown())
		changeGameStateToMenu();
}

void IntroCinematic::update(float dt)
{
	//Update time tracker
	timePassedTracker += dt;

	//Update screens and make transitions when necessary
	if (!logo)
	{
		//Wait 2 seconds to make it more confortable
		if (timePassedTracker >= 2.f)
		{
			//Play the Infogrames sound
			if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped && audio->getMusic()->getDuration() != sf::seconds(10.29f))
			{
				audio->playMusicbyName("infogrames");
				audio->getMusic()->setLoop(false);
			}

			//Only fade out if it's not fading in
			if (!isFadingIn)
				fadeToBlackTransition(255 / 60 / 2, dt, false);		//fade out for 2 seconds
			//Since the fade out took 2 seconds, wait 8 seconds so the logo is fully shown for 6 seconds
			if (audio->getMusic()->getPlayingOffset() >= sf::seconds(8.f) || audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
			{
				fadeToBlackTransition(255 / 60 / 2, dt, true);		//fade in for 2 seconds
				//When the fading in has been done, update these trackers to start displaying the next cinematic screen
				if (hasFadeIn)
				{
					audio->stopAllMusic();	//Add this stop function here just to be sure if smth went wrong
					logo = true;
					timePassedTracker = 0;
				}
			}
		}
	}
	else if (!house)
	{
		//Play the intro music
		if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped)
		{
			audio->playMusicbyName("intro");
			audio->getMusic()->setLoop(false);
		}

		//Only fade out if it's not fading in
		if (!isFadingIn)
			fadeToBlackTransition(255 / 60 / 2, dt, false);		//fade out for 2 seconds

		//Update the house screen
		updateHouse(dt);

		//Since the fade out took 2 seconds, wait 3 seconds so that the screen is fully shown for 1 second
		if (timePassedTracker >= 6.f)
		{
			fadeToBlackTransition(255 / 60 / 2, dt, true);		//fade in for 2 seconds
			//When the fading in has been done, update these trackers to start displaying the next cinematic screen
			if (hasFadeIn)
			{
				house = true;
				timePassedTracker = 0;
			}
		}
	}
	else if (!conflict)
	{
		//Only fade out if it's not fading in
		if (!isFadingIn)
			fadeToBlackTransition(255 / 60 / 2, dt, false);		//fade out for 2 seconds

		//Update the conflict screen
		updateConflict(dt);

		//Update the message to be shown
		message.setString(sentence[1]);

		//Since the fade out took 2 seconds, wait 3 seconds so that the screen is fully shown for 1 second
		if (timePassedTracker >= 6.f)
		{
			fadeToBlackTransition(255 / 60 / 2, dt, true);		//fade in for 2 seconds
			//When the fading in has been done, update these trackers to start displaying the next cinematic screen
			if (hasFadeIn)
			{
				conflict = true;
				timePassedTracker = 0;
			}
		}
	}
	else if(!fortress)
	{
		//Only fade out if it's not fading in
		if (!isFadingIn)
			fadeToBlackTransition(255 / 60 / 2, dt, false);		//fade out for 2 seconds

		//Update the fortress screen
		lightningBolt.animate(dt);
		fortressLightning.setTextureRect(lightningBolt.getCurrentFrame());
		updateFortress(dt);

		//Update the message to be shown
		message.setString(sentence[2]);

		//Since the fade out took 2 seconds, wait 3 seconds so that the screen is fully shown for 1 second
		if (timePassedTracker >= 8.f)
		{
			fadeToBlackTransition(255 / 60 / 2, dt, true);		//fade in for 2 seconds
			//When the fading in has been done, update these trackers to start displaying the next cinematic screen
			if (hasFadeIn)
			{
				fortress = true;
				timePassedTracker = 0;
			}
		}
	}
	else if (!megaman)
	{
		//Only fade out if it's not fading in
		if (!isFadingIn)
			fadeToBlackTransition(255 / 60 / 2, dt, false);		//fade out for 2 seconds

		//Update the Final screen
		updateFinalScreen(dt);

		//Update the message to be shown
		message.setString(sentence[3]);

		//Since the fade out took 2 seconds, wait 3 seconds so that the screen is fully shown for 1 second
		if (timePassedTracker >= 7.f)
		{
			megaFront = true;							//Make megaman appearing in the front layer
			fadeToBlackTransition(255 / 60 / 2, dt, true);		//fade in for 2 seconds
			//When the fading in has been done, change game state to menu
			if (hasFadeIn)
			{
				//Make megaman jump on the last note of the song
				if (audio->getMusic()->getPlayingOffset() >= sf::seconds(37.75f))
					megaJump = true;
				if (megaJump && audio->getMusic()->getStatus() == sf::SoundSource::Stopped)		//Change the game state at the end of the soundtrack
				{
					megaman = true;			//Mean the intro cinematic is now finished
					changeGameStateToMenu();
				}
			}
		}
	}

	//Debug infos update
	if (debugUi->isDebugging())
		debugUi->updateDebugUi();
}

// Render level
void IntroCinematic::render()
{
	beginDraw();

	//Draw everything to the screen
	if (!logo)
	{
		window->draw(logoDisplay);
	}
	else if (!house)
	{
		window->draw(houseBackground);
		window->draw(houseFrontground);
	}
	else if (!conflict)
	{
		window->draw(conflictBackground);
		window->draw(conflictLight);
		window->draw(conflictWily);
	}
	else if (!fortress)
	{
		window->draw(fortressBackground);
		window->draw(fortressFrontground);
		window->draw(fortressLightning);
	}
	else if (!megaman)
	{
		window->draw(conflictBackground);
		window->draw(conflictLight);
		if(!megaFront)
			window->draw(finalMegaman);
	}

	//Draw the message after the logo has been shown
	if(logo)
		window->draw(message);

	//Draw the fade screen at it's current alpha
	window->draw(blackScreen);

	//When megaman jumps we want to keep seeing it
	if (megaFront)
	{
		if(megaJump)
			window->draw(finalLaser);
		window->draw(finalMegaman);
	}

	//Draw debug infos if enabled
	if (debugUi->isDebugging())
		window->draw(*debugUi->getUi());

	endDraw();
}

// Begins rendering to the back buffer. Background colour set to black.
void IntroCinematic::beginDraw()
{
	window->clear(sf::Color(1, 1, 1));
}

// Ends rendering to the back buffer, and swaps buffer to the screen.
void IntroCinematic::endDraw()
{
	window->display();
}

void IntroCinematic::updateHouse(float dt)
{
	houseFrontground.move(-30.f * dt, 0);
	houseBackground.move(20.f * dt, 0);
}

void IntroCinematic::updateConflict(float dt)
{
	conflictBackground.move(10.f * dt, 0);
	conflictWily.move(5.f * dt, 0);
	conflictLight.move(5.f * dt, 0);
}

void IntroCinematic::updateFortress(float dt)
{
	fortressBackground.move(10.f * dt, 0);
	fortressFrontground.move(-10.f * dt, 0);

	lightningTimeTracker += dt;
	if (lightningTimeTracker > 1)
	{
		fortressLightning.setPosition(std::rand() % window->getSize().x - fortressLightning.getSize().x, std::rand() % window->getSize().y / 3);
		lightningTimeTracker = 0;
	}
}

void IntroCinematic::updateFinalScreen(float dt)
{
	conflictBackground.move(-10.f * dt, 0);
	conflictLight.move(-5.f * dt, 0);
	if(!megaFront && !megaJump)
		finalMegaman.move(-5.f * dt, 0);
	else if(megaFront && !megaJump)
	{
		//Move megaman towards the center of the screen
		sf::Vector2f distance = sf::Vector2f(window->getSize().x / 2 - finalMegaman.getPosition().x, window->getSize().y / 2 - finalMegaman.getPosition().y);
		finalMegaman.move(distance * dt);
	}
	else
	{
		finalMegaman.setTextureRect(sf::IntRect(192, 0, 26, 30));
		if (finalLaser.getPosition().y < 0)
		{
			finalLaser.setPosition(std::rand() % window->getSize().x - finalLaser.getSize().x, window->getSize().y);
		}
		else
			finalLaser.move(0, -2000.f * dt);
	}
}


void IntroCinematic::fadeToBlackTransition(float transiTime, float dt, bool fadeIn)
{
	//Update the isFadingIn tracker so we can avoid having a fadeIn and fadeOut simultaneously
	isFadingIn = fadeIn;

	//Update the fadeTimeTracker
	fadeTimeTracker += dt;

	//Fade in
	if (alpha < 255 && fadeIn)
	{
		if (fadeTimeTracker >= transiTime / 255)
		{
			if (alpha + std::round((1 * dt) / (transiTime / 255)) < 255)
				alpha += std::round((1 * dt) / (transiTime / 255));
			else
				alpha = 255;
			fadeTimeTracker = 0;
		}

		//Reset trackers for the next step
		if (alpha == 255)
		{
			hasFadeIn = true;
			isFadingIn = false;
		}
	}
	//Fade out
	else if (alpha > 0 && !fadeIn)
	{
		if (fadeTimeTracker > transiTime / 255)
		{
			if (alpha - std::round((1 * dt) / (transiTime / 255)) > 0)
				alpha -= std::round((1 * dt) / (transiTime / 255));
			else
				alpha = 0;
			fadeTimeTracker = 0;
		}

		//Reset trackers for the next step
		if (alpha == 0)
			hasFadeIn = false;
	}

	//Finally, create and return the object to be drawn
	sf::Color color = sf::Color(0, 0, 0, alpha);		//R,G,B,Aplha
	blackScreen.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	blackScreen.setPosition(0, 0);
	blackScreen.setFillColor(color);
}

void IntroCinematic::changeGameStateToMenu()
{
	//Reset all trackers
	logo = false;
	house = false;
	conflict = false;
	fortress = false;
	megaman = false;
	megaFront = false;
	megaJump = false;
	timePassedTracker = 0;
	alpha = 255;
	hasFadeIn = false;
	isFadingIn = false;
	fadeTimeTracker = 0;
	lightningTimeTracker = 0;
	//Reset the message to the first sentence
	message.setString(sentence[0]);
	//Reset blackscreen
	blackScreen.setFillColor(sf::Color(0, 0, 0, 255));
	//Reset moving images and moving sprites positions
	houseBackground.setPosition(-.5f * window->getSize().x, 0);
	houseFrontground.setPosition(0, 0);
	conflictBackground.setPosition(-.1f * window->getSize().x, 0);
	conflictWily.setPosition(3 * window->getSize().x / 4, window->getSize().y / 2 - 45);
	conflictLight.setPosition(window->getSize().x / 4, window->getSize().y / 2 - 55);
	fortressBackground.setPosition(-.1f * window->getSize().x, 0);
	fortressFrontground.setPosition(0, 0);
	finalMegaman.setPosition(3 * window->getSize().x / 4 + finalMegaman.getSize().x / 2, window->getSize().y / 2 - 45 + finalMegaman.getSize().y / 2);
	//Reset the megaman sprite texture rect
	finalMegaman.setTextureRect(sf::IntRect(0, 8, 21, 24));
	//Stop audio cause change state
	audio->stopAllMusic();
	//Change game state to menu
	gameState->setCurrentState(State::MENU);
}