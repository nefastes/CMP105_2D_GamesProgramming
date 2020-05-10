#include "SuperCutter.h"
SuperCutter::SuperCutter()
{
	maxHealth = 5;
	resetHealth();

	cutterAnim.addFrame(sf::IntRect(0, 0, 20, 20));
	cutterAnim.addFrame(sf::IntRect(20, 0, 20, 20));
	cutterAnim.setFrameSpeed(1.f / 10.f);
}

SuperCutter::~SuperCutter()
{

}

void SuperCutter::update(float dt)
{
	if (isAlive())
	{
		//Animate the sprite
		if (velocity.x > 0)
			cutterAnim.setFlipped(true);
		else
			cutterAnim.setFlipped(false);
		cutterAnim.animate(dt);
		setTextureRect(cutterAnim.getCurrentFrame());

		//Move the cutter
		float pos = velocity.y * dt + 0.5f * 980.f * dt;
		velocity.y += 980.f * dt;
		setPosition(getPosition().x + velocity.x * dt, getPosition().y + pos);
	}
	else
	{
		timePassedTracker += dt;
		if (timePassedTracker >= .2f)
		{
			isDying = false;
			timePassedTracker = 0;
		}
	}
}