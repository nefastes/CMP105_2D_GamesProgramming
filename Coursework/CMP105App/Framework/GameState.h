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

	// Set a score
	void setGlobalScore(unsigned s);
	// Add a number to the global score
	void addGlobalScore(unsigned s);
	// Substract a number to the global score
	void subGlobalScore(unsigned s);
	// Returns the global score
	unsigned getGlobalScore();
	// Set a number of lives
	void setGlobalLives(unsigned l);
	// Add a number of lives
	void addGlobalLives(unsigned l);
	// Substract a number of lives
	void subGlobalLives(unsigned l);
	// Return the number of lives
	unsigned getGlobalLives();
	// Set the status of the current level
	void setLevelFinished(bool finish);
	// Get the status of the current level
	bool isLevelFinished();
	// Set a level as cleared
	void setLevelClear(unsigned position);
	// Check if a particular level has been cleared
	bool isLevelCleared(unsigned position);
	// Check if all levels have been cleared
	bool allLevelCleared();

protected:
	//States
	State currentState;
	State previousState;

	//We will also use this class to store things we want to have shared across all levels, such as the number of lives, score, etc.
	unsigned globalScore;
	unsigned globalLives;

	//Tracker to see if we finished a level, will be set to true by the win item
	bool levelFinished;

	//Store which level has been completed
	bool clearedLevels[6];
};