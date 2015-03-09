#ifndef MATH_HPP
#define MATH_HPP
/*
 * "Math" is meant to provide simple and commonly used mathematical operations
 * in games. Collision also uses these math functions, and making the math functions
 * seperate shouldn't break backwards compatibility*/
struct coordinate
{
	float x;
	float y;
};
//True distance
float distanceTo (float,float, float, float);
//Non sqrted distance
//Is it actually manhattan?
float manhattanTo (float, float, float, float);
//The angle to point towards something
float pointTowards(float, float, float, float);
//Returns magnitude of vector (good for normalization)
float magnitude(float, float);

//Normalizes coordinates
void normalize(coordinate*);
//Grabs the normal of a vector (perpendicular vector)
coordinate getVectorNormal(float, float, bool);
coordinate getVectorNormal(coordinate*, bool);
//Radian/Degree functions
float radToDeg(float);
float degToRad(float);
//Returns vector from degree
coordinate degToVec(float);

//If an image is off the screen, this function will return false
//Takes X, Y, image W, image H, Window W, and Window H
bool isVisible(int, int, int, int, int, int);
#endif
