#pragma once
#include "Framework/GameObject.h"
#include "Framework/TileMap.h"
#include "Framework/Collision.h"
#include "Framework/AudioManager.h"
#include "Player.h"
#include "DebugUi.h"

//Enum to know with map (stage) we want to create
enum class Maps { TUTORIAL, SCIMAN };

class TileManager
{
private:
	//Main tile map object
	TileMap tileMap;

	//Tile objects
	GameObject tile;						//Will contain a unique tile
	std::vector<GameObject> tiles;			//Will store all the different tiles

	//Map creation variables
	sf::Vector2u mapSize;				//Map dimensions
	std::vector<int> map;				//the tile map

	//Window ref
	sf::RenderWindow* window;

	//Debug tracker acces
	DebugUi* debugUi;

	//Map trackers, tack which map is supposed to be drawn
	unsigned mapTracker;

	//Transition tracker, to enable a transition in the level and what kind of transition it is (vertical upwards, etc.)
	bool transitionning;
	unsigned transitionType;
	//Door tracker, to determine which door tile is to be opened
	unsigned doorTracker;
	//Door tracker, to be set to true when the screen transition has been made and the door must be closed
	bool doorClosing;

	//Time tracker used to open doors only
	float timeTracker;

	//Audio pointer
	AudioManager* audio;

public:
	TileManager();
	~TileManager();
	void update(float dt, Player& p);
	void render();
	void createMap(Maps level, unsigned section);
	void buildCreatedMap(sf::Vector2f position);
	void setWindow(sf::RenderWindow* hwnd) { window = hwnd; };
	void setDebugRef(DebugUi* dui) { debugUi = dui; };
	sf::Vector2u getMapSize() { return mapSize; };
	unsigned getCurrentMap() { return mapTracker; };
	void setCurrentMap(unsigned m) { mapTracker = m; };
	sf::Vector2f getMapPosition() { return tileMap.getPosition(); };
	bool isTransitionning() { return transitionning; };
	void setTransitionning(bool t) { transitionning = t; };
	unsigned getTransitionType() { return transitionType; };
	void setAudio(AudioManager* aud) { audio = aud; };
	void setCloseDoor(bool c) { doorClosing = c; };

private:
	//Transition
	void openDoor(unsigned topTile, bool inside);
	bool closeDoor(unsigned topTile);
};

