#ifndef BELOTE_SOUNDMANAGER_H
#define BELOTE_SOUNDMANAGER_H

#include <SFML/Audio.hpp>

#include "Tools/Singleton.h"

class SoundManager : public Singleton<SoundManager>
{
public:
	enum SoundFX
	{
		FX_CLICK,

		_SoundFXCount
	};

public:
	static void		Init();

	void			PlayFX(SoundFX fx);
	bool			IsFXPlaying(SoundFX fx) const;

private:
	SoundManager();

private:
	sf::SoundBuffer		m_FXBuffers[_SoundFXCount];
	sf::Sound			m_FXSounds[_SoundFXCount];
};

#endif
