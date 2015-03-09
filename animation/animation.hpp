#ifndef ANIMATION_HPP
#define ANIMATION_HPP
#include "../graphics/graphics.hpp"
#include "../ept/eptParser.hpp"
#include "../timer/timer.hpp"
class animation
{
	private:
		sprite* sourceImage;
		
		float frameRate;
		int totalFrames;
		int currentFrame;
		timer currentTime;
		bool isPlaying;
		
		float startX;
		float startY;
		//This will be added for a new frame
		float addX;
		float addY;

		float widthX;
		float widthY;
		//Mostly for rotation
		float originX, originY;
		float rotation;
		
		float posX, posY;
	public:
		animation();
		//All of these must be set (I recommend an EPT file for these!)
		void setImage(sprite*);
		void setFrameRate(float);
		void setTotalFrames(int);
		void setStartPosition(float, float);
		void setAddAmount(float, float);
		void setWidth(float, float);
		void setOrigin(float, float);

		void setRotation(float);
		void setPosition(float, float);
		float getX();
		float getY();
		
		void render(window*);
		
		//Starts the animation at zero
		void start();
		//Stops the animation on the first frame
		void stop();
		//Pauses the animation on its current frame
		void pause();
		//Resume the animation
		void resume();
		
		//Use an EPT file to set up the animation (ultra convenient)
		void configureFromEpt(eptGroup*);
};
#endif
