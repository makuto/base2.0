#ifndef TILE_CAMERA_CPP
#define TILE_CAMERA_CPP
#include "tileCamera.hpp"

tileCamera::tileCamera()
{
	staticMap=NULL;
	dynamicMap=NULL;
	positionX=positionY=0;
}
		
//Set the map the camera will be controlling (it's one or the other)
void tileCamera::setMap(staticTileMap* newMap)
{
	staticMap=newMap;
	dynamicMap=NULL;
}
void tileCamera::setMap(dynamicTileMap* newMap)
{
	staticMap=NULL;
	dynamicMap=newMap;
}

//Takes the width of the desired viewport (in pixels) and configures map properly
void tileCamera::setViewSize(float x, float y)
{
	if (staticMap)
	{
		staticMap->setViewSize((x / staticMap->getTileWidth()) + 1, (y / staticMap->getTileHeight()) + 1);
	}
	else if (dynamicMap)
	{
		dynamicMap->setViewSize((x / dynamicMap->getTileWidth()) + 1, (y / dynamicMap->getTileHeight()) + 1);
	}
}

//Takes a position (in pixels) and positions the tile map properly
void tileCamera::setPosition(float x, float y)
{
	if (staticMap)
	{
		positionX=(int) x / staticMap->getTileWidth();
		positionY=(int) y / staticMap->getTileHeight();
		staticMap->setViewOffset(-(int) x % (int) staticMap->getTileWidth(), -(int) y % (int) staticMap->getTileHeight() );
	}
	else if (dynamicMap)
	{
		positionX=(int) x / dynamicMap->getTileWidth();
		positionY=(int) y / dynamicMap->getTileHeight();
		dynamicMap->setViewOffset(-(int) x % (int) dynamicMap->getTileWidth(), -(int) y % (int) dynamicMap->getTileHeight() );
	}
}

//Pass these into the render function to get a proper position
int tileCamera::getX()
{
	return positionX;
}
int tileCamera::getY()
{
	return positionY;
}
#endif
