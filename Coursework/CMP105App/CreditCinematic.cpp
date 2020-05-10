#include "CreditCinematic.h"
CreditCinematic::CreditCinematic(sf::RenderWindow* hwnd, Input* in, AudioManager* aud, GameState* gs, DebugUi* ui)
{
	//Pointers
	window = hwnd;
	input = in;
	audio = aud;
	gameState = gs;
	debugUi = ui;

	//Tracker
	timePassedTracker = 0;

	//Music
	audio->addMusic("sfx/JDG_Chanson_Infogrames.ogg", "chansonInfogrames");

	//Font & textures
	font.loadFromFile("font/PressStart2P-vaV7.ttf");
	cakeTex.loadFromFile("custom_sprites/Cake.PNG");
	infogramesLogoTex.loadFromFile("custom_sprites/Infogrames_logo.PNG");

	//Text & images
	initTitle(fakeTitle, "AMAZING FAKE CREDITS");
	initSub(fakeSubTitle, "They are just amazing ! For real !");

	initTitle(leadDesignerTitle, "LEAD DESIGNER");
	initSub(leadDesignerName, "Batman");

	initTitle(artistTitle, "LEAD ARTIST");
	initSub(artistName, "Obviously not me because I can't draw");

	initTitle(leadProgramerTitle, "LEAD PROGRAMMER");
	initSub(leadProgramerName, "David Goodenough (third time employee of the year !)");

	initTitle(cakeTitle, "NEED A PIECE OF CAKE ?");
	cake.setTexture(&cakeTex);
	cake.setSize(sf::Vector2f(640, 480));
	cake.setOrigin(cake.getSize() / 2.f);

	initTitle(leadTesterTitle, "LEAD TESTING");
	initSub(leadTesterName, "My grandma ?");

	initTitle(publisherTitle, "NOT PUBLISHED BY");
	infogramesLogo.setTexture(&infogramesLogoTex);
	infogramesLogo.setSize(sf::Vector2f(640, 480));
	infogramesLogo.setOrigin(infogramesLogo.getSize() / 2.f);

	resetPositions();
}

CreditCinematic::~CreditCinematic()
{

}

void CreditCinematic::handleInput(float dt)
{
	if (input->isKeyDownOnce(sf::Keyboard::Escape) || input->isKeyDownOnce(sf::Keyboard::Enter) || input->isMouseLDown() ||
		(audio->getMusic()->getStatus() == sf::SoundSource::Stopped && audio->getMusic()->getLoop() == false))
	{
		gameState->setCurrentState(State::MENU);
		audio->stopAllMusic();
		resetPositions();
	}
}

void CreditCinematic::update(float dt)
{
	if (audio->getMusic()->getStatus() == sf::SoundSource::Stopped && audio->getMusic()->getLoop() != false)
	{
		audio->playMusicbyName("chansonInfogrames");
		audio->getMusic()->setLoop(false);
	}

	if (timePassedTracker < 5)
		timePassedTracker += dt;
	else
	{
		if (infogramesLogo.getPosition().y > window->getView().getCenter().y)
		{
			fakeTitle.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			fakeSubTitle.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			leadDesignerTitle.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			leadDesignerName.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			artistTitle.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			artistName.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			leadProgramerTitle.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			leadProgramerName.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			cakeTitle.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			cake.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			leadTesterTitle.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			leadTesterName.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			publisherTitle.move(sf::Vector2f(0, SCROLL_SPEED * dt));
			infogramesLogo.move(sf::Vector2f(0, SCROLL_SPEED * dt));
		}
	}
}

void CreditCinematic::render()
{
	beginDraw();

	window->draw(fakeTitle);
	window->draw(fakeSubTitle);

	window->draw(leadDesignerTitle);
	window->draw(leadDesignerName);

	window->draw(artistTitle);
	window->draw(artistName);

	window->draw(leadProgramerTitle);
	window->draw(leadProgramerName);

	window->draw(cakeTitle);
	window->draw(cake);

	window->draw(leadTesterTitle);
	window->draw(leadTesterName);

	window->draw(publisherTitle);
	window->draw(infogramesLogo);

	endDraw();
}

void CreditCinematic::beginDraw()
{
	window->clear(sf::Color(1, 1, 1));
}

void CreditCinematic::endDraw()
{
	window->display();
}

void CreditCinematic::initTitle(sf::Text& t, std::string m)
{
	t.setFont(font);
	t.setFillColor(sf::Color::White);
	t.setCharacterSize(42);
	t.setString(m);
	t.setOrigin(t.getGlobalBounds().width / 2.f, t.getGlobalBounds().height / 2.f);
}

void CreditCinematic::initSub(sf::Text& t, std::string m)
{
	t.setFont(font);
	t.setFillColor(sf::Color::Yellow);
	t.setCharacterSize(21);
	t.setString(m);
	t.setOrigin(t.getGlobalBounds().width / 2.f, t.getGlobalBounds().height / 2.f);
}

void CreditCinematic::resetPositions()
{
	fakeTitle.setPosition(window->getView().getCenter());
	fakeSubTitle.setPosition(window->getView().getCenter() + sf::Vector2f(0, 2 * fakeTitle.getGlobalBounds().height));

	leadDesignerTitle.setPosition(fakeTitle.getPosition() + sf::Vector2f(0, 400));
	leadDesignerName.setPosition(leadDesignerTitle.getPosition() + sf::Vector2f(0, 2 * leadDesignerTitle.getGlobalBounds().height));

	artistTitle.setPosition(leadDesignerTitle.getPosition() + sf::Vector2f(0, 400));
	artistName.setPosition(artistTitle.getPosition() + sf::Vector2f(0, 2 * artistTitle.getGlobalBounds().height));

	leadProgramerTitle.setPosition(artistTitle.getPosition() + sf::Vector2f(0, 400));
	leadProgramerName.setPosition(leadProgramerTitle.getPosition() + sf::Vector2f(0, 2 * leadProgramerTitle.getGlobalBounds().height));
	
	cakeTitle.setPosition(leadProgramerTitle.getPosition() + sf::Vector2f(0, 400));
	cake.setPosition(cakeTitle.getPosition() + sf::Vector2f(0, cakeTitle.getGlobalBounds().height + cake.getSize().y / 2.f));

	leadTesterTitle.setPosition(cakeTitle.getPosition() + sf::Vector2f(0, 880));
	leadTesterName.setPosition(leadTesterTitle.getPosition() + sf::Vector2f(0, 2 * leadTesterTitle.getGlobalBounds().height));

	publisherTitle.setPosition(leadTesterTitle.getPosition() + sf::Vector2f(0, 400));
	infogramesLogo.setPosition(publisherTitle.getPosition() + sf::Vector2f(0, publisherTitle.getGlobalBounds().height + cake.getSize().y / 2.f));
}