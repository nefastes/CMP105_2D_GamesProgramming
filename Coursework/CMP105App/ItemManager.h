#pragma once
#include "Item.h"
#include "Framework/AudioManager.h"
#include "Framework/GameState.h"
#include <iostream>
#include "Framework/Collision.h"
#include "Player.h"
#include "TileManager.h"
#include "Framework/Animation.h"
class ItemManager
{
private:
	//Audio pointer for sound effects
	AudioManager* audio;
	GameState* gameState;
	TileManager* tileManager;

	sf::Texture itemTex;
	Item item;

	std::vector<Item> items;

	Animation bigHealthAnim;

	//Alive sprites tracker
	unsigned aliveSprites;

public:
	ItemManager();
	~ItemManager();

	void spawnItem(sf::Vector2f spawnpoint, short unsigned Id);
	void updatePosition(sf::Vector2f position);
	void update(float dt, Player& p);
	void renderItems(sf::RenderWindow* window);
	void killAllItems();

	void setDebugging(bool debug);

	void sendPointers(GameState* gs, AudioManager* aud, TileManager* tm) { gameState = gs; audio = aud; tileManager = tm; };
};

