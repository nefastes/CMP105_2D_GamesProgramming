#pragma once
#include "DeathParticle.h"
#include "Framework/Vector.h"
#include "math.h"

class DeathParticleManager
{
private:
	//Declare the death particle object and texture
	sf::Texture deathParticleTex;
	sf::Vector2f direction;
	//No need to make a vector of particles since we will spawn 8 only, use array instead
	DeathParticle deathParticles[8];
	//Tracker set to true when particles have been spawned
	bool particlesSpawned;

public:
	DeathParticleManager();
	~DeathParticleManager();

	void spawnParticles(sf::Vector2f spawnPoint);
	void update(float dt);
	void render(sf::RenderWindow* window);
	void killAllParticles();
	void setParticlesSpawned(bool spawned);
	bool haveParticlesSpawned();
};

