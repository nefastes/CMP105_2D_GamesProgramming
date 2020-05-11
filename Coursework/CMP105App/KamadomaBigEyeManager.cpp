#include "KamadomaBigEyeManager.h"
KamadomaBigEyeManager::KamadomaBigEyeManager()
{
	kamadomaTex.loadFromFile("custom_sprites/Kamadoma.PNG");
	bigEyeTex.loadFromFile("custom_sprites/BigEye.PNG");
}

KamadomaBigEyeManager::~KamadomaBigEyeManager()
{

}

void KamadomaBigEyeManager::initKamaOrBigEye(sf::Vector2f spawnPoint, unsigned place, bool kamadoma)
{
	if (kamadoma)
	{
		kamadomas[place].setTexture(&kamadomaTex);
		kamadomas[place].setTextureRect(sf::IntRect(0, 0, 14, 19));
		kamadomas[place].setSize(sf::Vector2f(50, 50));
		kamadomas[place].setCollisionBox(sf::FloatRect(0, 25, 50, 25));
		kamadomas[place].setPosition(spawnPoint);
		kamadomas[place].setMaxHealth(1);
	}
	else
	{
		kamadomas[place].setTexture(&bigEyeTex);
		kamadomas[place].setTextureRect(sf::IntRect(0, 0, 33, 48));
		kamadomas[place].setSize(sf::Vector2f(150, 250));
		kamadomas[place].setCollisionBox(sf::FloatRect(0, 40, 150, 210));
		kamadomas[place].setPosition(spawnPoint.x, spawnPoint.y - 200);
		kamadomas[place].setMaxHealth(25);
	}
}

void KamadomaBigEyeManager::spawnKamadoma(sf::Vector2f spawnPoint, bool kamadoma)
{
	for (unsigned i = 0; i < kamadomas.size(); ++i)
	{
		if (!kamadomas[i].isAlive())
		{
			kamadomas[i].setAlive(true);
			kamadomas[i].resetHealth();
			kamadomas[i].setGroundState(true);	//They might have been killed in air
			initKamaOrBigEye(spawnPoint, i, kamadoma);
			return;
		}
	}
	kamadomas.push_back(KamadomaBigEye());
	kamadomas[kamadomas.size() - 1].setAlive(true);
	kamadomas[kamadomas.size() - 1].resetHealth();
	initKamaOrBigEye(spawnPoint, kamadomas.size() - 1, kamadoma);
}

void KamadomaBigEyeManager::update(float dt, Player& p, sf::View view)
{
	for (unsigned i = 0; i < kamadomas.size(); ++i)
	{
		//Only update if it is in the view
		if (kamadomas[i].getPosition().x + kamadomas[i].getSize().x > view.getCenter().x - view.getSize().x / 2.f &&
			kamadomas[i].getPosition().x < view.getCenter().x + view.getSize().x / 2.f &&
			kamadomas[i].getPosition().y < view.getCenter().y + view.getSize().y / 2.f)
		{
			//Always call the update, an alive check is made inside (so we can display the death frame)
			kamadomas[i].update(dt, p);
		}

		//Only if alive updates
		if (kamadomas[i].isAlive())
		{
			//Check if he is in air and adjust sprite texture rect and hitbox
			if (!kamadomas[i].getGroundState())
			{
				//Kamadoma
				if (kamadomas[i].getSize().x == 50)
				{
					kamadomas[i].setCollisionBox(sf::FloatRect(0, 0, 50, 50));
					kamadomas[i].setTextureRect(sf::IntRect(15, 0, 14, 19));
				}
				//Big Eye
				else
				{
					kamadomas[i].setCollisionBox(sf::FloatRect(30, 0, 90, 250));
					if(kamadomas[i].getVelocity().x < 0)
						kamadomas[i].setTextureRect(sf::IntRect(0, 49, 33, 48));
					else
						kamadomas[i].setTextureRect(sf::IntRect(33, 49, -33, 48));
				}

				//Check for world collisions if he is in the air
				tileManager->checkKamadomaCollision(kamadomas[i]);
			}
			else
			{
				//Kamadoma
				if (kamadomas[i].getSize().x == 50)
				{
					kamadomas[i].setCollisionBox(sf::FloatRect(0, 25, 50, 25));
					kamadomas[i].setTextureRect(sf::IntRect(0, 0, 14, 19));
				}
				//Big Eye
				else
				{
					kamadomas[i].setCollisionBox(sf::FloatRect(0, 40, 150, 210));
					if (kamadomas[i].getVelocity().x < 0)
						kamadomas[i].setTextureRect(sf::IntRect(0, 0, 33, 48));
					else
						kamadomas[i].setTextureRect(sf::IntRect(33, 0, -33, 48));
				}
			}

			//Update collisions with player
			//Kamadoma
			if (kamadomas[i].getSize().x == 50)
				updateEnemy(kamadomas[i], p, 20, 200);
			//Big Eye
			else
				updateEnemy(kamadomas[i], p, 50, 20000);

			//Update debug hitboxes
			if (kamadomas[i].isDebugging())
				kamadomas[i].updateDebugBoxes();
		}
	}
}

void KamadomaBigEyeManager::render(sf::RenderWindow* window)
{
	for (unsigned i = 0; i < kamadomas.size(); ++i)
		if (kamadomas[i].isAlive() || kamadomas[i].getDying())
		{
			window->draw(kamadomas[i]);

			if (kamadomas[i].isDebugging())
			{
				window->draw(*kamadomas[i].getDebugObjectSize());
				window->draw(*kamadomas[i].getDebugCollisionBox());
			}
		}
}

void KamadomaBigEyeManager::killAllKamadomas()
{
	for (unsigned i = 0; i < kamadomas.size(); ++i)
		if (kamadomas[i].isAlive())
			kamadomas[i].setAlive(false);
}

void KamadomaBigEyeManager::setDebugging(bool debug)
{
	for (unsigned i = 0; i < kamadomas.size(); ++i)
		if (kamadomas[i].isAlive())
			kamadomas[i].setDebugging(debug);
}