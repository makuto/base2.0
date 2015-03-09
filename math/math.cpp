#ifndef MATH_CPP
#define MATH_CPP
#include <cmath>

#include "math.hpp"

float distanceTo(float aX, float aY, float bX, float bY)
{
    //ax-bx sq + ay-by sq
    float x=aX-bX;
    float y=aY-bY;
    return sqrtf(x*x + y*y);
}
float manhattanTo(float aX, float aY, float bX, float bY)
{
    //ax-bx sq + ay-by sq
    float x=aX-bX;
    float y=aY-bY;
    return x*x + y*y;
}
float pointTowards(float aX, float aY, float bX, float bY)
{
    return radToDeg(atan2f((bY-aY) , (bX-aX)));
}
float magnitude(float x, float y)
{
    return distanceTo(0, 0, x, y);
}
void normalize(coordinate* toNormalize)
{
	float length=magnitude(toNormalize->x, toNormalize->y);
    if (length > 0)
    {
        toNormalize->x/=length;
        toNormalize->y/=length;
    }
}
coordinate getVectorNormal(float x, float y, bool right)
{
	coordinate returnCoord;
	if (!right)
	{
		returnCoord.x=-y;
		returnCoord.y=x;
	}
	else
	{
		returnCoord.x=y;
		returnCoord.y=-x;
	}
	return returnCoord;
}
coordinate getVectorNormal(coordinate* vectorIn, bool right)
{
	coordinate returnCoord;
	if (!right)
	{
		returnCoord.x=-vectorIn->y;
		returnCoord.y=vectorIn->x;
	}
	else
	{
		returnCoord.x=vectorIn->y;
		returnCoord.y=-vectorIn->x;
	}
	return returnCoord;
}
float radToDeg(float radian)
{
	//180/pi
	return radian*57.295779513;
}
float degToRad(float degree)
{
	//pi/180
	return degree*0.017453278;
}
coordinate degToVec(float degree)
{
	coordinate result;
	float radian=degToRad(degree);
	result.x=cosf(radian);
	result.y=sinf(radian);
	return result;
}
bool isVisible(int x, int y, int wX, int wY, int windowW, int windowH)
{
	if (x + wX < 0 || y + wY < 0) return false;
	if (x > windowW || y > windowH) return false;
	return true;
}
#endif
