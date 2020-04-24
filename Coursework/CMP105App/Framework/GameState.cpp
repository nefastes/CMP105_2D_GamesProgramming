#include "GameState.h"
GameState::GameState()
{
	globalLives = 3;		//Start with 3 lives
	globalScore = 0;		//Start with a score of 0
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

void GameState::updateGlobalScore(unsigned s)
{
	globalScore = s;
}

unsigned GameState::getGlobalScore()
{
	return globalScore;
}

void GameState::updateGlobalLives(unsigned l)
{
	globalLives = l;
}

unsigned GameState::getGlobalLives()
{
	return globalLives;
}