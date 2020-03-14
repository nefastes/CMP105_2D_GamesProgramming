#pragma once
#include "Framework/GameObject.h"
#include "Framework/Animation.h"
class Player : public GameObject
{
private:
	sf::RenderWindow* window;
	Animation walk;
	Animation idle;
	bool isMoving;
	bool isOnGround;
	float sScale;
	sf::Vector2f gravity;
	sf::Vector2f stepVelocity;

public:
	Player();
	~Player();
	void update(float dt) override;
	void handleInput(float dt) override;
	void setWindow(sf::RenderWindow* hwnd) { window = hwnd; };
};

