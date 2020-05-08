#pragma once
#include "Framework/GameObject.h"
class BlasterBullet : public GameObject
{
private:
	sf::Vector2f direction;

public:
	BlasterBullet();
	~BlasterBullet();

	void update(float dt) override;
	void setDirection(sf::Vector2f d);
};

