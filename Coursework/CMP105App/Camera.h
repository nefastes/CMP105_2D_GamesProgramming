#pragma once
#include "Framework/GameObject.h"
class Camera : public GameObject
{
public:
	Camera();
	~Camera();

	void setView(sf::View v);
	sf::View getView();
	void moveViewPos(sf::Vector2f pos);
	void sendWindowInfos(sf::RenderWindow* hwnd) { window = hwnd; };

private:
	sf::View view;
	sf::RenderWindow* window;
};

