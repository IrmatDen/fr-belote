#include "SoundManager.h"

SoundManager* Singleton<SoundManager>::ms_Singleton = 0;

SoundManager::SoundManager()
{
	m_FXBuffers[FX_CLICK].LoadFromFile("Sounds/click.wav");
	m_FXSounds[FX_CLICK].SetBuffer(m_FXBuffers[FX_CLICK]);
}

void SoundManager::Init()
{
	new SoundManager();
}

void SoundManager::PlayFX(SoundFX fx)
{
	m_FXSounds[fx].Play();
}

bool SoundManager::IsFXPlaying(SoundFX fx) const
{
	return m_FXSounds[fx].GetStatus() == sf::SoundSource::Playing;
}
