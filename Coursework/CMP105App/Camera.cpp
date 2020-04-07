#include "Camera.h"
Camera::Camera()
{
	
}

Camera::~Camera()
{

}

void Camera::setView(sf::View v)
{
	view = v;
}

sf::View Camera::getView()
{
	return view;
}

void Camera::moveViewPos(sf::Vector2f pos)
{
	if (pos.x >= 0 + view.getSize().x / 2 && pos.x <= 11038 - view.getSize().x/2)
	{
		view.move(pos.x - view.getCenter().x, 0);
	}
}