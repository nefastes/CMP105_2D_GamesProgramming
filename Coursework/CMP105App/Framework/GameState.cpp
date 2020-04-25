#include "GameState.h"
GameState::GameState()
{
	globalLives = 2;		//Start with 3 lives
	globalScore = 0;		//Start with a score of 0
	levelFinished = false;
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