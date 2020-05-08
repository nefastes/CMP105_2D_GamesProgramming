#pragma once
#include "Framework/Collision.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "TileManager.h"
#include "ItemManager.h"
class EnemyManager
{
protected:
	//Pointers
	TileManager* tileManager;
	ItemManager* itemManager;
	AudioManager* audio;
	GameState* gameState;

public:
	EnemyManager();
	~EnemyManager();

	//Base classe update
	void updateEnemy(Enemy& enemy, Player& player, short int bodyDamage, unsigned pointsReward);

	//Get pointers
	void sendPointers(TileManager* tm, ItemManager* im, AudioManager* aud, GameState* gs);
};

