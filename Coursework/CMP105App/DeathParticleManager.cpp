#include "DeathParticleManager.h"
DeathParticleManager::DeathParticleManager()
{
	//Init texture
	deathParticleTex.loadFromFile("custom_sprites/Megaman_Death_Particle.PNG");

	//Init all the death particles
	for (unsigned i = 0; i < 8; ++i)
	{
		deathParticles[i] = DeathParticle();
		deathParticles[i].setTexture(&deathParticleTex);
		deathParticles[i].setSize(sf::Vector2f(64, 64));
	}

	//Set a diffrent direction for each particle
	deathParticles[0].setDirection(sf::Vector2f(0, 1));				//Up
	deathParticles[1].setDirection(sf::Vector2f(1, 1));				//top right
	deathParticles[2].setDirection(sf::Vector2f(1, 0));				//right
	deathParticles[3].setDirection(sf::Vector2f(1, -1));			//bottom right
	deathParticles[4].setDirection(sf::Vector2f(0, -1));			//bottom
	deathParticles[5].setDirection(sf::Vector2f(-1, -1));			//bottom left
	deathParticles[6].setDirection(sf::Vector2f(-1, 0));			//left
	deathParticles[7].setDirection(sf::Vector2f(-1, 1));			//top left

	//They are not spawned yet
	particlesSpawned = false;
}

DeathParticleManager::~DeathParticleManager()
{

}

void DeathParticleManager::spawnParticles(sf::Vector2f spawnPoint)
{
	for (unsigned i = 0; i < 8; ++i)
	{
		deathParticles[i].setPosition(spawnPoint);
		deathParticles[i].setAlive(true);
	}
	particlesSpawned = true;
}

void DeathParticleManager::update(float dt)
{
	for (unsigned i = 0; i < 8; ++i)
		deathParticles[i].update(dt);
}

void DeathParticleManager::render(sf::RenderWindow* window)
{
	for (unsigned i = 0; i < 8; ++i)
		if(deathParticles[i].isAlive())
			window->draw(deathParticles[i]);
}

void DeathParticleManager::killAllParticles()
{
	for (unsigned i = 0; i < 8; ++i)
		deathParticles[i].setAlive(false);
	particlesSpawned = false;
}

void DeathParticleManager::setParticlesSpawned(bool spawned)
{
	particlesSpawned = spawned;
}

bool DeathParticleManager::haveParticlesSpawned()
{
	return particlesSpawned;
}