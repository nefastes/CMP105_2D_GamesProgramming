#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
class DialogBox : public GameObject
{
private:
	//Trackers
	int sentenceTracker;
	float oldTime;
	float skipTime;
	int currentLetter;
	bool isWriting;

	//Sentences
	std::string sentence1;
	std::string sentence2;
	std::string sentence3;
	std::string sentence4;

	//What will be drawn to the screen
	sf::Text message;
	sf::Font font;
	float textSpeed;

	//Box anim
	Animation boxAnim;

	//General activation bool, set it to false not to draw the dialog box
	bool activated;
	bool onlyOnce;
	bool hasFinished;

public:
	DialogBox();
	~DialogBox();

	void update(float dt) override;
	void handleInput(float dt) override;
	void drawSentence(std::string& s);
	sf::Text getDialog();
	void activateOnce();
	bool getActivated();
	bool isFinished();
};

