
#include "sound.h"
#include <stdexcept>
using namespace std;


Sound::Sound(const char* filename, bool looping)
{
	if(!buffer.loadFromFile(filename)) {
		throw std::logic_error("could not load sound file");
	}
	sound.setBuffer(buffer);
	sound.setLoop(looping);
}

Sound::~Sound(void)
{
}


void Sound::Play()
{
	sound.play();
}

void Sound::Stop()
{
	sound.stop();
}
