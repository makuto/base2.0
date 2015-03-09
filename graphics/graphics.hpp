/*This is the graphics layer for the game. It is meant to make it easy to switch out the graphics
 * library at a later time, if necessary. It also makes it a little easier to handle scripting language
 * bindings, if they are eventually added. Note that for now I won't be adding primitives like rectangles
 * or lines. They will be added if needed.
 * */
#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../timer/timer.hpp" //Useful for getframetime
 //To link:
//g++ -o "%e" "%f" -lsfml-graphics -lsfml-window -lsfml-system 

class window;
//Renderable text
class text
{
friend class window;
private:
	sf::Text str;
	sf::Font font;
public:
	void setText(std::string);
	bool loadFont(const char*);
	void setPosition(float newX, float newY);
	float getX();
	float getY();
	void setSize(unsigned int);
	void setColor(char r, char g, char b, char a);
	//Returns the actual element used for rendering
	sf::Text* getBase();
};
//A single renderable image
class sprite
{
 friend class window;
 private:
	sf::Texture srcImage;
	sf::Sprite spr;
public:
	//Load and configure the sprite (false=failed)
	bool load (const char* filename);
	//Set the sprite's new position
	void setPosition(float, float);
	//Add a vector-esque value to the position
	void addVector(float, float);
	//Get the sprite's current position
	float getX();
	float getY();
	//Rotate the sprite
	void setRotation(float rotation);
	//Change the origin
	void setOrigin(float,float);
	//Good for tilemap tiles
	void setSmooth(bool);
	//Returns the width and height of the image
	unsigned int getWidth();
	unsigned int getHeight();
	//Splits a part of the sprite out
	//Takes the top left corner coordinates and a width and height (both bases)
	void setSubRect(int,int,int,int);
	//Back to whole sprite
	void clearSubRect();
	//Return a direct pointer to the base sprite (lib dependant)
	sf::Sprite* getBase();
};
//The core of the graphics, the window.
class window
{
    private:
        sf::RenderWindow *win;
        sf::Color background;
        bool isClearing;
        bool focused;
		timer time;
    public:
        window(int width,int height, std::string title);
        //Close & delete the window
        ~window();
        //Various drawing functions
        void draw(sprite * spr);
        void draw(text *txt);
        //Update the window
        void update();
        //Test if the window should close
        bool shouldClose();
        //If the window is focused (true) or buried (false)
        bool isFocused();
        //Time since last frame
        float getFrameTime();
        //Window configuration & management
        void setBackgroundColor(char, char, char, char);
        //If you don't want to clear the window after updating (at all)
        void shouldClear(bool newState);
        //Get the dimensions of the window
        unsigned int getWidth();
        unsigned int getHeight();
        //Return a direct pointer to the base (library dependant)
        sf::RenderWindow* getBase();
};
#endif
