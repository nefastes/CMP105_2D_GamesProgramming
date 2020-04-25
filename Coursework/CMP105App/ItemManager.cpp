#include "ItemManager.h"
ItemManager::ItemManager()
{
	audio = nullptr;
	gameState = nullptr;
	tileManager = nullptr;
	aliveSprites = 0;
	itemTex.loadFromFile("custom_sprites/NES_Mega_Man_Items.PNG");
	bigHealthAnim.addFrame(sf::IntRect(53, 0, 14, 16));
	bigHealthAnim.addFrame(sf::IntRect(71, 0, 14, 16));
	bigHealthAnim.setFrameSpeed(1.f / 10.f);
}

ItemManager::~ItemManager()
{

}

void ItemManager::spawnItem(sf::Vector2f spawnPoint, short unsigned Id)
{
	for (unsigned i = 0; i < items.size(); ++i)
	{
		if (!items[i].isAlive() && items[i].getId() == Id)
		{
			items[i].setPosition(spawnPoint);
			items[i].setAlive(true);
			aliveSprites += 1;
			return;
		}
	}
	items.push_back(Item());
	switch (Id)
	{
	case 0:
		items[items.size() - 1].setSize(sf::Vector2f(25, 25));
		items[items.size() - 1].setTextureRect(sf::IntRect(0, 8, 8, 8));
		items[items.size() - 1].setId(Id);
		break;
	case 1:
		items[items.size() - 1].setSize(sf::Vector2f(30, 36));
		items[items.size() - 1].setTextureRect(sf::IntRect(23, 4, 10, 12));
		items[items.size() - 1].setId(Id);
		break;
	case 2:
		items[items.size() - 1].setSize(sf::Vector2f(56, 64));
		items[items.size() - 1].setTextureRect(sf::IntRect(53, 0, 14, 16));
		items[items.size() - 1].setId(Id);
		break;
	case 3:
		items[items.size() - 1].setSize(sf::Vector2f(50, 50));
		items[items.size() - 1].setTextureRect(sf::IntRect(148, 1, 16, 15));
		items[items.size() - 1].setId(Id);
		break;
	case 4:
		items[items.size() - 1].setSize(sf::Vector2f(64, 64));
		items[items.size() - 1].setTextureRect(sf::IntRect(129, 0, 16, 16));
		items[items.size() - 1].setId(Id);
		break;
	default:
		std::cout << "Error: Tried to spawn an item that doesnt exist !" << std::endl; items.pop_back();
		return;
		break;
	}
	items[items.size() - 1].setAlive(true);
	items[items.size() - 1].setTexture(&itemTex);
	items[items.size() - 1].setPosition(spawnPoint);
	items[items.size() - 1].setCollisionBox(sf::FloatRect(0, 0, items[items.size() - 1].getSize().x, items[items.size() - 1].getSize().y));
	aliveSprites += 1;
}

void ItemManager::updatePosition(sf::Vector2f position)
{
	for (unsigned i = 0; i < items.size(); ++i)
		items[i].setPosition(items[i].getPosition() + position);
}

void ItemManager::update(float dt, Player& p)
{
	//Check for a collision with the player
	for (unsigned i = 0; i < items.size(); ++i)
	{
		if (items[i].isAlive() && Collision::checkBoundingBox(&items[i], &p))
		{
			switch (items[i].getId())
			{
			case 0:		gameState->addGlobalScore(200);		items[i].setAlive(false);		audio->playSoundbyName("points2");		break;
			case 1:		p.addHealth(25);					items[i].setAlive(false);												break;
			case 2:		p.addHealth(50);					items[i].setAlive(false);												break;
			case 3:		gameState->addGlobalLives(1);		items[i].setAlive(false);		audio->playSoundbyName("life");			break;
			case 4:		items[i].setAlive(false); break;//TODO: make win here
			default:	std::cout << "Error: Collided with an item that doesnt exist !" << std::endl;								break;
			}
		}
	}

	//Update all alive items
	bigHealthAnim.animate(dt);
	for (unsigned i = 0; i < items.size(); ++i)
	{
		if (items[i].isAlive())
		{
			items[i].update(dt);
			if(!items[i].isGrounded())
				tileManager->checkItemCollision(items[i]);
			if (items[i].getId() == 2)
				items[i].setTextureRect(bigHealthAnim.getCurrentFrame());
		}
	}
}

void ItemManager::renderItems(sf::RenderWindow* window)
{
	//Draw alive sprites on the screen
	for (unsigned i = 0; i < items.size(); ++i)
	{
		if (items[i].isAlive())
		{
			window->draw(items[i]);
			if (items[i].isDebugging())
				window->draw(*items[i].getDebugCollisionBox());
		}
	}
}

void ItemManager::killAllItems()
{
	for (unsigned i = 0; i < items.size(); ++i)
	{
		items[i].setAlive(false);
		items[i].setGrounded(false);
	}
	aliveSprites = 0;
}

void ItemManager::setDebugging(bool debug)
{
	for (unsigned i = 0; i < items.size(); ++i)
		if (items[i].isAlive())
			items[i].setDebugging(debug);
}