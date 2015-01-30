#include "AudioManager.h"

#define MAXCHANNELS 100
#define CHECKFMODRESULT(r) if(r != FMOD_OK) { FMOD_ErrorString(r); return false; } 

#pragma region cons/des
AudioManager::AudioManager()
{
	soundVolume = 10.0f;
	musicVolume = 1.0f;

	isMute = false;

	soundMap = std::map<std::string, FMOD::Sound*>();
	channelMap = std::map<std::string, FMOD::Channel*>();
	channelGroupMap = std::map<std::string, FMOD::ChannelGroup*>();
}

AudioManager::~AudioManager()
{

}

#pragma endregion

#pragma region init/run/shutdown
bool AudioManager::Init()
{
	//creating the fmod system for the audio system. 
	fm_result = FMOD::System_Create(&fm_system);
	CHECKFMODRESULT(fm_result);

	fm_result = fm_system->getNumDrivers(&numDrivers);
	CHECKFMODRESULT(fm_result);

	//no sound cards -> disabled sounds.
	if(numDrivers == 0)
	{
		fm_result = fm_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		CHECKFMODRESULT(fm_result);
	}

	fm_result = fm_system->getDriverInfo(0, name, 256, 0, 0, 0, 0);
	CHECKFMODRESULT(fm_result);

	//actually initialize fmod.
	fm_result = fm_system->init(MAXCHANNELS, FMOD_INIT_NORMAL, 0);
	if(fm_result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		fm_result = fm_system->setSoftwareFormat(48000, FMOD_SPEAKERMODE_STEREO, 0);
	}
	CHECKFMODRESULT(fm_result);

	return true;
}

void AudioManager::Update(float _dt)
{

}

void AudioManager::Shutdown()
{
	//release all memory allocated to short sounds.
	for(soundMapIT it = soundMap.begin(); it != soundMap.end(); ++it)
	{
		(*it).second->release();
	}

	//release the fmod system.
	fm_system->release();
}
#pragma endregion

#pragma region creation

//requires full name "gamma.ogg" for instance.
void AudioManager::CreateSound(std::string _name)
{
	if(soundMap.find(_name) != soundMap.end())
	{
		return;
	}

	// "Assets/Audio Assets/my noise.wewe "
	std::string path = "../Assets/Audio Assets/" + _name;
	FMOD::Sound* audio;
	fm_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &audio);
	audio->setLoopCount(1);

	FMOD::Channel* channel;
	fm_result = fm_system->playSound(audio, NULL, true, &channel);

	soundMap[_name] = audio;
	channelMap[_name] = channel;
}

void AudioManager::CreateAudioStream(std::string _name)
{
	if (soundMap.find(_name) != soundMap.end())
	{
		return;
	}

	std::string path = "../Assets/Audio Assets/" + _name;
	FMOD::Sound* audio;
	fm_system->createStream(path.c_str(), FMOD_LOOP_NORMAL, 0, &audio);
	
	FMOD::Channel* channel;
	fm_result = fm_system->playSound(audio, NULL, false, &channel);
	
	soundMap[_name] = audio;
	channelMap[_name] = channel;
}

#pragma region

#pragma region adjust playback
void AudioManager::PlaySFX(std::string _key)
{
	if(soundMap.find(_key) == soundMap.end())
	{
		CreateSound(_key);
	}

	if (isMute == false)
	{
		fm_system->playSound(soundMap[_key], NULL, false, NULL);
	}
	
}

void AudioManager::PauseSound(std::string _key)
{
	channelMap[_key]->setPaused(true);
	fm_system->update();
}
void AudioManager::PauseAllSounds()
{
	for (channelMapIT it = channelMap.begin(); it != channelMap.end(); ++it)
	{
		it->second->setPaused(true);
	}

	fm_system->update();
}
void AudioManager::ResumeSound(std::string _key)
{
	channelMap[_key]->setPaused(false);
	fm_system->update();
}
void AudioManager::ResumeAllSounds()
{
	for(channelMapIT it = channelMap.begin(); it != channelMap.end(); ++it)
	{
		for (channelMapIT it = channelMap.begin(); it != channelMap.end(); ++it)
		{
			it->second->setPaused(false);
		}
	}
}
void AudioManager::StopSound(std::string _key)
{
	bool isPaused;
	channelMap[_key]->getPaused(&isPaused);
	channelMap[_key]->setPaused(!isPaused);
}
void AudioManager::StopAllSounds()
{
	for (channelMapIT it = channelMap.begin(); it != channelMap.end(); ++it)
	{
		bool isPaused;
		it->second->getPaused(&isPaused);
		it->second->setPaused(!isPaused);
	}

	isMute = !isMute;
	fm_system->update();
	
}

#pragma endregion

#pragma region adjust volume 
void AudioManager::SetSoundVolume(float _new)
{
	soundVolume = _new;
}
void AudioManager::SetMusicVolume(float _new)
{
	musicVolume = _new;
}
void AudioManager::UnmuteAllSounds()
{
	for(channelMapIT it = channelMap.begin(); it != channelMap.end(); ++it)
	{
		
	}
}

void AudioManager::MuteAllSounds()
{
	for(channelMapIT it = channelMap.begin(); it != channelMap.end(); ++it)
	{
		bool isMuted;
		fm_result = it->second->isPlaying(&isMuted);

		if(isMuted == false)
		{
			it->second->setPaused(true);
		}
	}
}

#pragma endregion

//KW