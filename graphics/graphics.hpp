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
//An image with individually changeable pixels
class pixels
{
    friend class window;
    private:
        int width;
        int height;
        sf::Uint8* rawPixels;
        sf::Texture texture;
        sprite spr;
    public:
        //Creates the pixel array and sets the pixels to (0, 0, 0, 0);
        //sets up the primary sprite as well. Note that the primary sprite
        //smoothing is set to off by default
        pixels(int newWidth, int newHeight);
        //Creates the pixel array and sets the pixels to the provided color
        //sets up the primary sprite as well. Note that the primary sprite
        //smoothing is set to off by default
        pixels(int newWidth, int newHeight, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

        //Use this function to manipulate the position etc. of the pixels
        //through the master sprite
        sprite* getSprite();

        //Manipulate the color of a single pixel (RGBA); does bounds checking.
        //Remember to call update() in order to see changes
        void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

        //Call after changing any pixels in order to see changes
        void update();

        //Sets the provided values to the color of the specified pixel; returns
        //false if out of bounds
        bool getPixel(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);
        //Returns the width and height of the image
        int getWidth();
        int getHeight();
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
        void draw(pixels *pix);
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
