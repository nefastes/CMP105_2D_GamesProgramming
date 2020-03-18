#include "DialogBox.h"
DialogBox::DialogBox()
{
	//init trackers
	sentenceTracker = 0;
	oldTime = 0;
	isWriting = false;
	currentLetter = 1;

	//init sentences
	sentence1 = "Well you found me.\nWelcome to my\nlaboratory !";
	sentence2 = "Unfortunately for you,\nyour visit will not be\nlong here..";
	sentence3 = "But don't worry,\nI intend to use your\nremains for my\nexperiments HAHAHA !!";
	sentence4 = "Let the show begin !";

	//Message init
	font.loadFromFile("font/PressStart2P-vaV7.ttf");
	message.setFont(font);
	message.setCharacterSize(48);
	message.setLineSpacing(2);
	message.setFillColor(sf::Color::Black);
	message.setPosition(50, 50);
	textSpeed = .1f;

	//Anim init
	boxAnim.addFrame(sf::IntRect(0, 16, 64, 16));
	boxAnim.addFrame(sf::IntRect(0, 32, 64, 16));
	boxAnim.setFrameSpeed(1.f / 10.f);

	//Global init
	activated = false;
	onlyOnce = true;
	hasFinished = false;
}

DialogBox::~DialogBox()
{

}

void DialogBox::update(float dt)
{
	if (isWriting && activated)
	{
		setTextureRect(sf::IntRect(0, 0, 64, 16));
		oldTime += dt;
		if (oldTime >= textSpeed)
		{
			oldTime = 0;
			switch (sentenceTracker)
			{
			case 1:			drawSentence(sentence1);						break;
			case 2:			drawSentence(sentence2);						break;
			case 3:			drawSentence(sentence3);						break;
			case 4:			drawSentence(sentence4);						break;
			case 5:			activated = false; hasFinished = true;			break;
			default:														break;
			}
		}
	}
}

void DialogBox::handleInput(float dt)
{
	if (activated)
	{
		if (!isWriting)
		{
			if (sentenceTracker != 0)
			{
				boxAnim.animate(dt);
				setTextureRect(boxAnim.getCurrentFrame());
				textSpeed = .1f;
				if (input->isKeyDown(sf::Keyboard::F) || input->isKeyDown(sf::Keyboard::Enter))
				{
					++sentenceTracker;
					isWriting = true;
				}
			}
			else
			{
				++sentenceTracker;
				isWriting = true;
			}
		}
		if (isWriting)
		{
			//TODO: ask the teachers about a way to check if the key was up before this check and get rid of dt below

			skipTime += dt;
			if(skipTime >= .75f)
				if (input->isKeyDown(sf::Keyboard::F) || input->isKeyDown(sf::Keyboard::Enter))
				{
					textSpeed = .01f;
					skipTime = 0;
				}
		}
	}
}

void DialogBox::drawSentence(std::string& s)
{
	std::string temp;
	if (currentLetter <= s.size())
	{
		if (currentLetter >= 53 && sentenceTracker == 3)	message.setPosition(50, -50);
		else												message.setPosition(50, 50);

		for (unsigned i = 0; i < currentLetter; ++i)
			temp += s[i];
		message.setString(temp);
		++currentLetter;
	}
	else
	{
		currentLetter = 1;
		isWriting = false;
	}
}

sf::Text DialogBox::getDialog()
{
	return message;
}

void DialogBox::activateOnce()
{
	if (onlyOnce)
	{
		onlyOnce = false;
		activated = true;
	}
}

bool DialogBox::getActivated()
{
	return activated;
}

bool DialogBox::isFinished()
{
	return hasFinished;
}