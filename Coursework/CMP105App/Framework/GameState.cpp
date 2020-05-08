#include "GameState.h"
GameState::GameState()
{
	globalLives = 2;		//Start with 3 lives
	globalScore = 0;		//Start with a score of 0
	levelFinished = false;
	for (unsigned i = 0; i < 6; ++i)
		clearedLevels[i] = false;
}

GameState::~GameState()
{

}

// Set the current state
void GameState::setCurrentState(State s)
{
	previousState = currentState;
	currentState = s;
}

// Returns the current state.
State GameState::getCurrentState()
{
	return currentState;
}

// Returns the previous state.
State GameState::getPreviousState()
{
	return previousState;
}

void GameState::setGlobalScore(unsigned s)
{
	globalScore = s;
}

void GameState::addGlobalScore(unsigned s)
{
	globalScore += s;
}

void GameState::subGlobalScore(unsigned s)
{
	globalScore -= s;
}

unsigned GameState::getGlobalScore()
{
	return globalScore;
}

void GameState::setGlobalLives(unsigned l)
{
	globalLives = l;
}

void GameState::addGlobalLives(unsigned l)
{
	globalLives += l;
}

void GameState::subGlobalLives(unsigned l)
{
	globalLives -= l;
}

unsigned GameState::getGlobalLives()
{
	return globalLives;
}

void GameState::setLevelFinished(bool finished)
{
	levelFinished = finished;
}

bool GameState::isLevelFinished()
{
	return levelFinished;
}

void GameState::setLevelClear(unsigned position)
{
	//Set a level to be cleared
	clearedLevels[position] = true;
}

bool GameState::isLevelCleared(unsigned position)
{
	return clearedLevels[position];
}

bool GameState::allLevelCleared()
{
	//Returns true if all levels are cleared
	for (unsigned i = 0; i < 6; ++i)
		if (clearedLevels[i] == false)
			return false;
	return true;
}