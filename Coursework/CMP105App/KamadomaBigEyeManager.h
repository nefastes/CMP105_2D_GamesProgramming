#pragma once
#include "KamadomaBigEye.h"
#include "EnemyManager.h"
class KamadomaBigEyeManager : public EnemyManager
{
private:
	sf::Texture kamadomaTex;
	sf::Texture bigEyeTex;

	std::vector<KamadomaBigEye> kamadomas;

	void initKamaOrBigEye(sf::Vector2f spawnPoint, unsigned place, bool kamadoma);

public:
	KamadomaBigEyeManager();
	~KamadomaBigEyeManager();

	//if true spawn a kamadoma, else a Big Eye
	void spawnKamadoma(sf::Vector2f spawnPoint, bool kamadoma);

	void update(float dt, Player& p, sf::View view);
	void render(sf::RenderWindow* window);

	void killAllKamadomas();

	void setDebugging(bool debug);
};

