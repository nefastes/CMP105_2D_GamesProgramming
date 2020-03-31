// Collision Class
// Provides default functions for testing collision detection. Functions for standard axis aligned bounding box, and bounding circle.
// @author Paul Robertson

#pragma once
#include "GameObject.h"

// Static class provide collision detection functions.
class Collision
{
public:
	// Check AABB for collision. Returns true if collision occurs.
	static bool checkBoundingBox(GameObject* sp1, GameObject* sp2);
	// Check collision with box and point. Returns true if collision occurs.
	static bool checkBoundingBox(GameObject* s1, sf::Vector2i s2);
	// Check collision with a generic float rect object and a point. Returns true if collision occurs.
	// Has been added to check collisions with the mouse position and menu texts
	static bool checkBoundingBox(sf::FloatRect* s1, sf::Vector2i s2);
	// Check bounding circle collision. Returns true if collision occurs.
	static bool checkBoundingSphere(GameObject* sp1, GameObject* sp2);

};