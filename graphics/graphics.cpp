#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP
#include "graphics.hpp"
#include <iostream>
///////////////////////////////////////////////
// Window //////////////////////////////////
///////////////////////////////////////////////
window::window(int width,int height, std::string title)
{
    isClearing=true;
    win=new sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Close);
    //win->setVerticalSyncEnabled(false);
    //win->setFramerateLimit(45);
    focused=true;
	time.start();
}
//Close & delete the window
window::~window()
{
    win->close();
    delete win;
}
void window::draw(sprite * spr)
{
	if (spr) win->draw(spr->spr);
}
void window::draw(text *txt)
{
    if (txt) win->draw(txt->str);
}
void window::update()
{
	time.start();
    win->display();
    if (isClearing) win->clear(background);
}
///Doesn't work
float window::getFrameTime()
{
    //return win->GetFrameTime(); //Looks like it's deprecated; let's just use a timer then
	return time.getTime();
}
//This is a sloppy function because it does a lot more then tell whether or
//not the window should close, but it works well in this condition
bool window::shouldClose()
{
    sf::Event event;
    while (win->pollEvent(event))
    {
        if (event.type==sf::Event::Closed) return true;
        if (event.type==sf::Event::Resized)
        {
			//std::cout << "Window resized to " << event.size.width << " , " << event.size.height << "\n";
			//win->setSize(sf::Vector2u(event.size.width, event.size.height));
		}
		if (event.type==sf::Event::GainedFocus)
		{
			focused=true;
		}
		if (event.type==sf::Event::LostFocus)
		{
			focused=false;
		}
    }
    return false;
}
bool window::isFocused()
{
    return focused;
}
void window::setBackgroundColor(char r, char g, char b, char a)
{
    background.r=r;
    background.g=g;
    background.b=b;
    background.a=a;
}
//If you don't want to clear the window after updating (at all)
void window::shouldClear(bool newState)
{
    isClearing=newState;
}
//Dimensions
unsigned int window::getWidth()
{
	return win->getSize().x;
}
unsigned int window::getHeight()
{
	return win->getSize().y;
}

//If you need the SFML window itself
sf::RenderWindow* window::getBase()
{
    return win;
}
///////////////////////////////////////////////
// Text ///////////////////////////////////////
///////////////////////////////////////////////
bool text::loadFont(const char* fileName)
{
	if (!font.loadFromFile(fileName)) return false;
	else
	{
		str.setFont(font);
		return true;
	}
}
void text::setText(std::string newText)
{
    str.setString(newText);
}
void text::setPosition(float newX, float newY)
{
    str.setPosition(newX, newY);
}
float text::getX()
{
    return str.getPosition().x;
}
float text::getY()
{
    return str.getPosition().y;
}
void text::setSize(unsigned int size)
{
    str.setCharacterSize(size);
}
void text::setColor(char r, char g, char b, char a)
{
    str.setColor(sf::Color(r,g,b,a));
}
sf::Text* text::getBase()
{
    return &str;
}
///////////////////////////////////////////////
// Sprite  /////////////////////////////////////
///////////////////////////////////////////////
bool sprite::load(const char* filename)
{
    if (!srcImage.loadFromFile(filename)) return false;
    spr.setTexture(srcImage);
    return true;
}
void sprite::setPosition(float newX, float newY)
{
    spr.setPosition(newX, newY);
}
void sprite::addVector(float newX, float newY)
{
    spr.setPosition(newX+getX(), newY+getY());
}
float sprite::getX()
{
    return spr.getPosition().x;
}
float sprite::getY()
{
    return spr.getPosition().y;
}
unsigned int sprite::getWidth()
{
	return srcImage.getSize().x;
}
unsigned int sprite::getHeight()
{
	return srcImage.getSize().y;
}
void sprite::setRotation(float rotation)
{
    spr.setRotation(rotation);
}
void sprite::setOrigin(float x, float y)
{
    spr.setOrigin(x, y);
}
void sprite::setSmooth(bool useSmooth)
{
    srcImage.setSmooth(useSmooth);
}
void sprite::setSubRect(int x, int y, int wx, int hy)
{
    spr.setTextureRect(sf::Rect<int>(x,y,wx,hy));
}
void sprite::clearSubRect()
{
	setSubRect(0, 0, getWidth(), getHeight());
}
sf::Sprite* sprite::getBase()
{
    return &spr;
}
#endif
