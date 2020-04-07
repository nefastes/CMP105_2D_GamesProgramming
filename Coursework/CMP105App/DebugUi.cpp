#include "DebugUi.h"
DebugUi::DebugUi()
{
	//Assign the default values
	verticalSyncEnabled = false;
	masterDebug = false;
	masterVolume = 100;
	frameRate = 60;

	//Ui init
	debugFont.loadFromFile("font/arial.ttf");
	ui.setFont(debugFont);
	ui.setCharacterSize(24);
	ui.setFillColor(sf::Color::Red);
	ui.setString("FPS: " + std::to_string(frameRate) + "\nVSync: Disabled\nVolume: " +std::to_string(masterVolume));
	ui.setPosition(sf::Vector2f(0, 0));
}

DebugUi::~DebugUi()
{

}

void DebugUi::setVerticalSync(bool enable)
{
	verticalSyncEnabled = enable;
}

void DebugUi::setDebugging(bool mode)
{
	masterDebug = mode;
}

void DebugUi::setMasterVolume(unsigned volume)
{
	masterVolume = volume;
}

void DebugUi::setFrameRate(unsigned rate)
{
	frameRate = rate;
}

bool DebugUi::getVerticalSync()
{
	return verticalSyncEnabled;
}

bool DebugUi::isDebugging()
{
	return masterDebug;
}

unsigned DebugUi::getMasterVolume()
{
	return masterVolume;
}

unsigned DebugUi::getFrameRate()
{
	return frameRate;
}

void DebugUi::updateDebugUi()
{
	std::string vsync;
	if (verticalSyncEnabled)
		vsync = "Enabled";
	else
		vsync = "Disabled";
	ui.setString("FPS: " + std::to_string(frameRate) + "\nVSync: " + vsync + "\nVolume: " +std::to_string(masterVolume));
}

sf::Text* DebugUi::getUi()
{
	return &ui;
}

void DebugUi::updateUiPos(sf::Vector2f pos)
{
	ui.setPosition(pos);
}