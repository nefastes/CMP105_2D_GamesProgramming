#pragma once
#include "Framework/GameObject.h"
class DeathParticle : public GameObject
{
private:
	sf::Vector2f direction;

public:
	DeathParticle();
	~DeathParticle();
	void update(float dt) override;
	void setDirection(sf::Vector2f d);
	sf::Vector2f getDirection();
};

