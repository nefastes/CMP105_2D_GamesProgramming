// Game Object Class
// A class representing a single renderable game object.
// Game Object extend sf::RectangleShape, and added functionailty for collision, sprite states (alive/dead), velocity for movement, input component
// Most elements of your game will extend game object to create larger more complex elements such as player, enemies, bullets etc.
// @author paul Robertson

#pragma once
#include "SFML\Graphics.hpp"
#include "Input.h"

class GameObject : public sf::RectangleShape
{
public:
	GameObject();
	~GameObject();

	// Virtual update function. Base function does nothing (for static objects). Inheritted version will most likely override this function.
	virtual void handleInput(float dt);
	virtual void update(float dt);

	// Control sprite speed and direction
	void setVelocity(sf::Vector2f vel);
	void setVelocity(float vx, float vy);
	sf::Vector2f getVelocity();

	// Sprite state
	bool isAlive() { return alive; };
	void setAlive(bool b) { alive = b; };

	// For sprite collision, set collider box, get collider box, and dedicated virtual function for collision response
	bool isCollider() { return collider; };
	void setCollider(bool b) { collider = b; };
	sf::FloatRect getCollisionBox();
	void setCollisionBox(float x, float y, float width, float height) { collisionBox = sf::FloatRect(x, y, width, height); };
	void setCollisionBox(sf::FloatRect fr) { collisionBox = fr; };
	virtual void collisionResponse(GameObject* collider);
	//Set and get the object ID so we can have different collisions responses based on that as well
	void setTargetName(std::string tname) { targetname = tname; };		//ID exmaples: "world", "ladder", "door", etc.
	std::string getTargetname() { return targetname; };

	// Set the input component
	void setInput(Input* in) { input = in; };

	//Debugging
	void setDebugging(bool debug) { debugging = debug; };
	bool isDebugging() { return debugging; };
	void setCollisionBoxColor(sf::Color color);
	void updateDebugBoxes();
	sf::RectangleShape* getDebugObjectSize() { return &debugSize; };
	sf::RectangleShape* getDebugCollisionBox() { return &debugCollisionBox; };

protected:
	// Sprite properties
	sf::Vector2f velocity;
	bool alive;

	// Collision vars
	sf::FloatRect collisionBox;
	bool collider;

	//Additionnal informations (ID used for tiles)
	std::string targetname;

	// input component
	Input* input;

	//Debugging
	bool debugging;
	sf::RectangleShape debugSize;
	sf::RectangleShape debugCollisionBox;
};