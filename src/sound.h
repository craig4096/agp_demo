
#ifndef __SOUND__
#define __SOUND__

#include <SFML/Audio.hpp>

class Sound {
	sf::SoundBuffer buffer;
	sf::Sound sound;
public:

	Sound(const char* wavFile, bool looping);
	~Sound();

	void Play();
	void Stop();
};



#endif
