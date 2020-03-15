#include "Boss.h"
Boss::Boss()
{
	//-----------------------------------------------------------
	//----------------------------------------------------Phase 1
	//-----------------------------------------------------------

	//Idle anim
	idlePhase1.addFrame(sf::IntRect(0, 0, 64, 64));
	idlePhase1.addFrame(sf::IntRect(64, 0, 64, 64));
	idlePhase1.setLooping(true);
	idlePhase1.setFrameSpeed(1.f / 6.f);

	//Transformation anim
	transform.addFrame(sf::IntRect(64, 128, 64, 64));
	transform.addFrame(sf::IntRect(128, 128, 64, 64));
	transform.setLooping(false);
	transform.setFrameSpeed(1.f / 6.f);

	//Taunt anim
	taunt.addFrame(sf::IntRect(192, 128, 64, 64));
	taunt.addFrame(sf::IntRect(0, 192, 64, 64));
	taunt.setLooping(true);
	taunt.setFrameSpeed(1.f / 6.f);

	//Attack anim
	intoAttackPhase1.addFrame(sf::IntRect(128, 0, 64, 64));
	intoAttackPhase1.addFrame(sf::IntRect(192, 0, 64, 64));
	intoAttackPhase1.setLooping(false);
	intoAttackPhase1.setFrameSpeed(1.f / 6.f);

	//Attack Idle 1
	attackIdlePhase1_1.addFrame(sf::IntRect(0, 64, 64, 64));
	attackIdlePhase1_1.addFrame(sf::IntRect(192, 0, 64, 64));
	attackIdlePhase1_1.setLooping(true);
	attackIdlePhase1_1.setFrameSpeed(1.f / 6.f);

	//Attack Idle 2
	attackIdlePhase1_2.addFrame(sf::IntRect(192, 64, 64, 64));
	attackIdlePhase1_2.addFrame(sf::IntRect(128, 64, 64, 64));
	attackIdlePhase1_2.setLooping(true);
	attackIdlePhase1_2.setFrameSpeed(1.f / 6.f);
	

	//Attack anim
	attackPhase1.addFrame(sf::IntRect(64, 64, 64, 64));
	attackPhase1.addFrame(sf::IntRect(128, 64, 64, 64));
	attackPhase1.setLooping(false);
	attackPhase1.setFrameSpeed(1.f / 6.f);


	//-----------------------------------------------------------
	//----------------------------------------------------Phase 2
	//-----------------------------------------------------------



	//-----------------------------------------------------------
	//----------------------------------------------------Phase 3
	//-----------------------------------------------------------



	//Trackers init
	phase = 1;						//Start on phase 1
	taunted = false;				
	animMode = 5;					//Start in idle
	dialogOver = false;				

	//Other
	oldTime = 0;
	window = nullptr;
}

Boss::~Boss()
{

}

void Boss::update(float dt)
{
	if (phase == 1)
	{
		if (!taunted)
		{
			if (!dialogOver)
			{
				idlePhase1.animate(dt);
				setTextureRect(idlePhase1.getCurrentFrame());
			}
			else
			{
				if (animMode == 5)
				{
					transform.animate(dt);
					setTextureRect(transform.getCurrentFrame());
					if (!transform.getPlaying() && !transform.getReversed())
						animMode = 6;
					else if (!transform.getPlaying() && transform.getReversed())
					{
						taunted = true;
						animMode = 0;
					}
				}
				if (animMode == 6)
				{
					oldTime += dt;
					
					taunt.animate(dt);
					setTextureRect(taunt.getCurrentFrame());
					if (oldTime >= 1.5f)
					{
						taunt.stop();
						animMode = 5;
						transform.setReversed(true);
						transform.setPlaying(true);
						oldTime = 0;
					}
					
				}
			}
		}
		else
		{
			if (animMode == 0)
			{
				idlePhase1.animate(dt);
				setTextureRect(idlePhase1.getCurrentFrame());
			}
			if (animMode == 1)
			{

			}
			if (animMode == 2)
			{

			}
			if (animMode == 3)
			{

			}
			if (animMode == 4)
			{

			}
		}
	}
	else if (phase == 2)
	{

	}
	else if (phase == 3)
	{

	}
}

void Boss::setDialogState(bool state)
{
	dialogOver = state;
}