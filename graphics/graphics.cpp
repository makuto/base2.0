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
	win = new sf::RenderWindow(sf::VideoMode(width, height), title,
	                           sf::Style::Close | sf::Style::Resize);
	//win->setVerticalSyncEnabled(false);
    //win->setFramerateLimit(45);
    focused=true;
	time.start();
}

window::window(int width, int height, std::string title, WindowResizeCB onResizeCallback)
    : onResize(onResizeCallback)
{
    isClearing=true;
    win = new sf::RenderWindow(sf::VideoMode(width, height), title,
                               sf::Style::Close | sf::Style::Resize);
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
void window::draw(pixels *pix)
{
    if (pix) win->draw(pix->spr.spr);
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
			win->setView(
			    sf::View(sf::Vector2<float>(event.size.width / 2.f, event.size.height / 2.f),
			             sf::Vector2<float>(event.size.width, event.size.height)));
            if (onResize)
                onResize(event.size.width, event.size.height);
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
void text::setText(std::wstring newText)
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
    color = sf::Color(r,g,b,a);
    str.setColor(color);
}
void text::setAlpha(char a)
{
    color.a = a;
    str.setColor(color);
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

pixels::pixels(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
    rawPixels = new sf::Uint8[width * height * 4];
    //Blank the pixels
    for (int i = 0; i < height; ++i)
    {
        for (int n = 0; n < width; ++n)
        {
            int startIndex = (i * width * 4) + (n * 4);
            rawPixels[startIndex] = 0;
            rawPixels[startIndex + 1] = 0;
            rawPixels[startIndex + 2] = 0;
            rawPixels[startIndex + 3] = 0;
        }
    }
    texture.create(width, height);
    texture.update(rawPixels);
    spr.getBase()->setTexture(texture);
}
pixels::pixels(int newWidth, int newHeight, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    width = newWidth;
    height = newHeight;
    rawPixels = new sf::Uint8[width * height * 4];
    //Blank the pixels
    for (int i = 0; i < height; ++i)
    {
        for (int n = 0; n < width; ++n)
        {
            int startIndex = (i * width * 4) + (n * 4);
            rawPixels[startIndex] = r;
            rawPixels[startIndex + 1] = g;
            rawPixels[startIndex + 2] = b;
            rawPixels[startIndex + 3] = a;
        }
    }
    texture.create(width, height);
    texture.update(rawPixels);
    spr.getBase()->setTexture(texture);
    spr.setSmooth(false);
}
sprite* pixels::getSprite()
{
    return &spr;
}
void pixels::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        int startIndex = (y * width * 4) + (x * 4);
        rawPixels[startIndex] = r;
        rawPixels[startIndex + 1] = g;
        rawPixels[startIndex + 2] = b;
        rawPixels[startIndex + 3] = a;
    }
}
void pixels::update()
{
    texture.update(rawPixels);
}
bool pixels::getPixel(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        int startIndex = (y * width * 4) + (x * 4);
        r = rawPixels[startIndex];
        g = rawPixels[startIndex + 1];
        b = rawPixels[startIndex + 2];
        a = rawPixels[startIndex + 3];
        return true;
    }
    else return false;
}
int pixels::getWidth()
{
    return width;
}
int pixels::getHeight()
{
    return height;
}
#endif
