#include "EnemyManager.h"
EnemyManager::EnemyManager()
{
	tileManager = nullptr;
	itemManager = nullptr;
	audio = nullptr;
	gameState = nullptr;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::updateEnemy(Enemy& enemy, Player& player, short int bodyDamage, unsigned pointsReward)
{
	//Retrieve a vector of all the alive bullets shot by the player
	std::vector<Bullet*> bullets = player.getBullets();

	//Check for a player collision
	if (enemy.isAlive() && Collision::checkBoundingBox(&enemy, &player))
	{
		//First check if the collision does NOT happen on the very edge of both collision boxes,
		//we do not want a collision in this case
		if (player.getCollisionBox().left + player.getCollisionBox().width != enemy.getCollisionBox().left &&
			player.getCollisionBox().left != enemy.getCollisionBox().left + enemy.getCollisionBox().width &&
			player.getCollisionBox().top + player.getCollisionBox().height != enemy.getCollisionBox().top &&
			player.getCollisionBox().top != enemy.getCollisionBox().top + enemy.getCollisionBox().height)
			player.damage(bodyDamage);
	}

	//Check for a bullet collision
	for (unsigned j = 0; j < bullets.size(); ++j)
	{
		if (enemy.isAlive() && bullets[j]->isAlive() && Collision::checkBoundingBox(&enemy, bullets[j]))
		{
			enemy.damage(1);
			bullets[j]->setAlive(false);
			if(!enemy.isInvincible())
				audio->playSoundbyName("enemyDamage");
			else
				audio->playSoundbyName("enemyInvincible");

			//Check if we killed the suzy
			if (!enemy.isAlive())
			{
				//Add score
				gameState->addGlobalScore(pointsReward);

				//Drop a random item
				unsigned random = rand() % 100 + 1;
				if (random <= 2)
					itemManager->spawnItem(enemy.getPosition(), 3);	//Tile sized items
				else if (random <= 10)
					itemManager->spawnItem(enemy.getPosition(), 2);
				else if (random <= 20)
					itemManager->spawnItem(enemy.getPosition() + enemy.getSize() / 2.f - sf::Vector2f(25, 25), 1);	//Smaller items
				else if (random <= 40)
					itemManager->spawnItem(enemy.getPosition() + enemy.getSize() / 2.f - sf::Vector2f(25, 25), 0);
			}
		}
	}
}

void EnemyManager::sendPointers(TileManager* tm, ItemManager* im, AudioManager* aud, GameState* gs)
{
	tileManager = tm;
	itemManager = im;
	audio = aud;
	gameState = gs;
}