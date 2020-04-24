// Game State Class
// Simple game state controller. enum lists possible game states (can be modified), tracks and updates the current state
// @author Paul Robertson

#pragma once

// Possible states. This enum can be modified to add additional states as required.
enum class State { MENU, LEVEL, PAUSE, CREDITS, OPTION, STAGESELECT, INTRO, TUTORIAL };

class GameState
{
public:
	GameState();
	~GameState();

	// Set the current state
	void setCurrentState(State s);
	// Returns the current state.
	State getCurrentState();
	// Returns the previous state.
	State getPreviousState();

	// Update the global score
	void updateGlobalScore(unsigned s);
	// Returns the global score
	unsigned getGlobalScore();
	// Update the number of lives
	void updateGlobalLives(unsigned l);
	// Return the number of lives
	unsigned getGlobalLives();

protected:
	//States
	State currentState;
	State previousState;

	//We will also use this class to store things we want to have shared across all levels, such as the number of lives, score, etc.
	unsigned globalScore;
	unsigned globalLives;
};