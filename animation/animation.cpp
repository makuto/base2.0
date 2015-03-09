#ifndef ANIMATION_CPP
#define ANIMATION_CPP
#include "animation.hpp"
////////////////////////////////////////////////////
// Animation ///////////////////////////////////////
////////////////////////////////////////////////////
animation::animation()
{
	sourceImage=NULL;
	
	frameRate=0;
	totalFrames=0; //starts at 1; if you have 3 different frames, put in 3
	currentFrame=0;
	currentTime.start();
	isPlaying=true;
	
	startX=0;
	startY=0;
	addX=0;
	addY=0;
	widthX=0;
	widthY=0;
	originX=originY=0;
	rotation=0;
	posX=posY=0;
}
void animation::setImage(sprite* newImage)
{
	sourceImage=newImage;
}
void animation::setFrameRate(float newFrameRate)
{
	frameRate=newFrameRate;
}
void animation::setTotalFrames(int newTotal)
{
	totalFrames=newTotal;
}
void animation::setStartPosition(float x, float y)
{
	startX=x;
	startY=y;
}
void animation::setAddAmount(float x, float y)
{
	addX=x;
	addY=y;
}
void animation::setWidth(float x, float y)
{
	widthX=x;
	widthY=y;
}
void animation::setPosition(float newX, float newY)
{
	posX=newX;
	posY=newY;
}
void animation::setOrigin(float x, float y)
{
	originX=x;
	originY=y;
}
void animation::setRotation(float newRot)
{
	rotation=newRot;
}
float animation::getX()
{
	return posX;
}		
float animation::getY()
{
	return posY;
}		
void animation::render(window* win)
{
	float newX=0;
	float newY=0;
	//If the animation is playing, update the frames
	if (isPlaying)
	{
		//If it's time to change frames
		if (currentTime.getTime()>=frameRate)
		{
			//Change frames
			if (currentFrame+1==totalFrames || currentFrame>totalFrames) currentFrame=0;
			else ++currentFrame;
			currentTime.start();
		}
	}
	//The magic happens here; find the frame's top left corner
	newX=startX + (addX * currentFrame);
	newY=startY + (addY * currentFrame);
	//Prepare the image
	sourceImage->setSubRect(newX, newY, widthX, widthY);
	sourceImage->setOrigin(originX, originY);
	sourceImage->setRotation(rotation);
	sourceImage->setPosition(posX, posY);
	//Draw the image
	win->draw(sourceImage);
	//Just in case a subrect messes someone up down the line
	sourceImage->clearSubRect();
}
//Starts the animation at zero
void animation::start()
{
	currentFrame=0;
	currentTime.start();
}
//Stops the animation on the first frame
void animation::stop()
{
	isPlaying=false;
	currentFrame=0;
}
//Pauses the animation on its current frame
void animation::pause()
{
	isPlaying=false;
}
void animation::resume()
{
	isPlaying=true;
}
void animation::configureFromEpt(eptGroup* currentAnimation)
{
	frameRate=atof(currentAnimation->getAttribute("frameRate").c_str());
	totalFrames=atoi(currentAnimation->getAttribute("totalFrames").c_str());
	startX=atof(currentAnimation->getAttribute("startX").c_str());
	startY=atof(currentAnimation->getAttribute("startY").c_str());
	addX=atof(currentAnimation->getAttribute("addX").c_str());
	addY=atof(currentAnimation->getAttribute("addY").c_str());
	widthX=atof(currentAnimation->getAttribute("widthX").c_str());
	widthY=atof(currentAnimation->getAttribute("widthY").c_str());
	originX=atof(currentAnimation->getAttribute("originX").c_str());
	originY=atof(currentAnimation->getAttribute("originY").c_str());
}
#endif
