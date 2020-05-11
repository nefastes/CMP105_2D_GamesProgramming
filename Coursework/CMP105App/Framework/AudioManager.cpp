#include "AudioManager.h"

// Some initial setup of music.
// All music will loop and have half the normal volume (so background music is quieter).
AudioManager::AudioManager()
{
	music.setLoop(true);
	music.setVolume(50);

	//Init ALL audio musics and ALL sound effects
	addMusic("sfx/Infogrames_Catchphrase.ogg", "infogrames");
	addMusic("sfx/Mega_Man_2_Intro.ogg", "intro");
	addMusic("sfx/JDG_Chanson_Infogrames.ogg", "chansonInfogrames");
	addMusic("sfx/Mega_Man_2_Menu.ogg", "menu");
	addMusic("sfx/Megaman_1_Stage_Select.ogg", "stageSelect");
	addMusic("sfx/Megaman_1_Stage_Start.ogg", "stageStart");
	addMusic("sfx/Megaman_1_Stage_Clear.ogg", "stageClear");
	addMusic("sfx/Megaman_1_Cut_Man_Stage.ogg", "cutMan");
	addMusic("sfx/Megaman_1_Elec_Man_Stage.ogg", "elecMan");
	addSound("sfx/Megaman_1_Select.ogg", "select");
	addSound("sfx/Megaman_1_Press.ogg", "press");
	addSound("sfx/Megaman_1_Change_Selection.ogg", "changeSelection");
	addSound("sfx/Megaman_1_Land.ogg", "land");
	addSound("sfx/Megaman_1_Teleport_Land.ogg", "tpLand");
	addSound("sfx/Megaman_1_Death.ogg", "death");
	addSound("sfx/Megaman_1_Pause.ogg", "pause");
	addSound("sfx/Megaman_1_Player_Damage.ogg", "playerDamage");
	addSound("sfx/Megaman_1_Damage.ogg", "enemyDamage");
	addSound("sfx/Megaman_1_No_Damage.ogg", "enemyInvincible");
	addSound("sfx/Megaman_1_Blaster_Shoot.ogg", "blasterShoot");
	addSound("sfx/Megaman_1_Shoot.ogg", "shoot");
	addSound("sfx/Megaman_1_Door.ogg", "door");
	addSound("sfx/Megaman_1_Points.ogg", "points");
	addSound("sfx/Megaman_1_Points2.ogg", "points2");
	addSound("sfx/Megaman_1_Life.ogg", "life");
	addSound("sfx/Megaman_1_Health.ogg", "health");
	addSound("sfx/Megaman_1_BigEye_Land.ogg", "bigEyeLand");
}

AudioManager::~AudioManager()
{
	stopAllMusic();
	stopAllSounds();
	sounds.clear();
	musics.clear();
}

// Given a filename and key a new SOUND object is added to the list. Loading the object into memory for playback
void AudioManager::addSound(std::string filename, std::string lname)
{
	
	sounds.push_back(s);
	sounds.back().sound.setVolume(50);			//Set a default sound volume of 50 (same as music)
	sounds.back().loadSound(filename, lname);
}

// Play back sound based on provided key.
void AudioManager::playSoundbyName(std::string lname)
{
	for (int i = 0; i < sounds.size(); i++)
	{
		if (sounds[i].name == lname)
		{
			sounds[i].sound.play();
			return;
		}
	}
}

// Stop all sounds. Possible that multiple sounds (not music) can be played simultaneously. This stops all sounds.
void AudioManager::stopAllSounds()
{
	for (int i = 0; i < sounds.size(); i++)
	{
		sounds[i].sound.stop();
	}
}

void AudioManager::pauseAllSounds()
{
	for (unsigned i = 0; i < sounds.size(); i++)
	{
		//pause() pause all PLAYING sounds
		sounds[i].sound.pause();
	}
}

void AudioManager::resumeAllSounds()
{
	for (unsigned i = 0; i < sounds.size(); i++)
	{
		if(sounds[i].sound.getStatus() == sf::SoundSource::Paused) sounds[i].sound.play();
	}
}

sf::Sound* AudioManager::getSound(std::string lname)
{
	for (int i = 0; i < sounds.size(); i++)
	{
		if (sounds[i].name == lname)
		{
			return &sounds[i].sound;
		}
	}
	return nullptr;
}

void AudioManager::setSoundsVolume(unsigned vol)
{
	for (unsigned i = 0; i < sounds.size(); i++)
		sounds[i].sound.setVolume(vol);
}

// Stores filename and key of music file. These are not loaded into memory but streamed when required.
void AudioManager::addMusic(std::string filename, std::string lname)
{
	MusicObject mu;
	musics.push_back(mu);
	musics.back().setMusic(filename, lname);
}

// Starts streaming music file based on key provided.
void AudioManager::playMusicbyName(std::string lname)
{
	for (int i = 0; i < musics.size(); i++)
	{
		if (musics[i].name == lname)
		{
			music.openFromFile(musics[i].filename);
			music.play();
			return;
		}
	}
}

// Stops music.
void AudioManager::stopAllMusic()
{
	music.stop();
}

void AudioManager::pauseAllMusic()
{
	music.pause();
}

void AudioManager::resumeAllMusic()
{
	music.play();
}

// Returns pointer to music object.
sf::Music* AudioManager::getMusic()
{
	return &music;
}