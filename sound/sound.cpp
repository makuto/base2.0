#ifndef SOUND_CPP
#define SOUND_CPP
#include "sound.hpp"
/////////////////////
//Music//////////////
/////////////////////
music::music()
{
}
bool music::load(const char* filename)
{
	return song.openFromFile(filename);
}
void music::play()
{
	song.play();
}
void music::pause()
{
	song.pause();
}
void music::stop()
{
	song.stop();
}
void music::loop(bool shouldLoop)
{
	song.setLoop(shouldLoop);
}
void music::setVolume(float newVolume)
{
	song.setVolume(newVolume);
}
void music::setPosition(float x, float y, float z)
{
	song.setPosition(x,y,z);
}
sf::Music* music::getBase()
{
	return &song;
}
/////////////////////
//Sound//////////////
/////////////////////
bool sound::load(const char* filename)
{
	if (!buffer.loadFromFile(filename)) return false;
	clip.setBuffer(buffer);
	return true;
}
void sound::play()
{
	clip.play();
}
void sound::pause()
{
	clip.pause();
}
void sound::stop()
{
	clip.stop();
}
void sound::loop(bool shouldLoop)
{
	clip.setLoop(shouldLoop);
}
void sound::setVolume(float newVolume)
{
	clip.setVolume(newVolume);
}
void sound::setPosition(float x, float y, float z)
{
	clip.setPosition(x, y, z);
}
sf::Sound* sound::getBase()
{
	return &clip;
}
/////////////////////
//Listener///////////
/////////////////////
void listener::setVolume(float newVolume)
{
	sf::Listener::setGlobalVolume(newVolume);
}
void listener::setPosition(float x, float y, float z)
{
	sf::Listener::setPosition(x, y, z);
}
#endif
