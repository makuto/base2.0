#ifndef SOUND_HPP
#define SOUND_HPP
#include <SFML/Audio.hpp>
class music
{
	private:
		sf::Music song;
	public:
		music();
		bool load(const char*);
		void play();
		void pause();
		void stop();
		void loop(bool);
		void setVolume(float);
		void setPosition(float, float, float );
		sf::Music* getBase();
};
class sound
{
	private:
		sf::SoundBuffer buffer;
		sf::Sound clip;
	public:
		bool load(const char*);
		void play();
		void pause();
		void stop();
		void loop(bool);
		void setVolume(float);
		void setPosition(float, float, float );
		sf::Sound* getBase();
};
class listener
{
	public:
		void setVolume(float);
		void setPosition(float, float, float);
};
#endif
