#pragma once
#include <SFML/Graphics.hpp>
class DebugUi
{
private:
	//Debug infos to be set from the option menu
	bool verticalSyncEnabled;
	bool masterDebug;
	unsigned masterVolume;
	unsigned frameRate;

	//Ui text
	sf::Font debugFont;
	sf::Text ui;

public:
	DebugUi();
	~DebugUi();
	//Setters
	void setVerticalSync(bool enable);
	void setDebugging(bool mode);
	void setMasterVolume(unsigned volume);
	void setFrameRate(unsigned rate);
	//Getters
	bool getVerticalSync();
	bool isDebugging();
	unsigned getMasterVolume();
	unsigned getFrameRate();
	//Others
	void updateDebugUi();
	sf::Text* getUi();
};