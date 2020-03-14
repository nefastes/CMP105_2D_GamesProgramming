#include "Boss.h"
Boss::Boss()
{
	//-----------------------------------------------------------
	//----------------------------------------------------Phase 1
	//-----------------------------------------------------------

	//Idle anim
	idlePhase1.addFrame(sf::IntRect(0, 0, 64, 64));
	idlePhase1.addFrame(sf::IntRect(0, 64, 64, 64));
	idlePhase1.setFrameSpeed(1.f / 10.f);

	//Transformation anim
	transform.addFrame(sf::IntRect(64, 128, 64, 64));
	transform.addFrame(sf::IntRect(128, 128, 64, 64));
	transform.setFrameSpeed(1.f / 10.f);

	//Taunt anim
	taunt.addFrame(sf::IntRect(192, 128, 64, 64));
	taunt.addFrame(sf::IntRect(0, 192, 64, 64));
	taunt.setFrameSpeed(1.f / 6.f);

	//Attack anim
	intoAttackPhase1.addFrame(sf::IntRect(128, 0, 64, 64));
	intoAttackPhase1.addFrame(sf::IntRect(192, 0, 64, 64));
	intoAttackPhase1.setFrameSpeed(1.f / 10.f);

	//Attack Idle 1
	attackIdlePhase1_1.addFrame(sf::IntRect(0, 64, 64, 64));
	attackIdlePhase1_1.addFrame(sf::IntRect(192, 0, 64, 64));
	attackIdlePhase1_1.setFrameSpeed(1.f / 10.f);

	//Attack Idle 2
	attackIdlePhase1_2.addFrame(sf::IntRect(192, 64, 64, 64));
	attackIdlePhase1_2.addFrame(sf::IntRect(128, 64, 64, 64));
	attackIdlePhase1_2.setFrameSpeed(1.f / 10.f);
	

	//Attack anim
	attackPhase1.addFrame(sf::IntRect(64, 64, 64, 64));
	attackPhase1.addFrame(sf::IntRect(128, 64, 64, 64));
	attackPhase1.setFrameSpeed(1.f / 10.f);


	//-----------------------------------------------------------
	//----------------------------------------------------Phase 2
	//-----------------------------------------------------------



	//-----------------------------------------------------------
	//----------------------------------------------------Phase 3
	//-----------------------------------------------------------
}

Boss::~Boss()
{

}

void Boss::update(float dt)
{

}