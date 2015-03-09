#ifndef TILEMAP_CPP
#define TILEMAP_CPP
#include <iostream>
#include "tileMap.hpp"

const unsigned int SEED=58008;
//Map formats
const unsigned char MAP_DEFAULT=0;
const unsigned char NO_DISPLAY=255;

#ifdef COMPILE_STATIC_MAP
staticTileMap::staticTileMap()
{
	imageMap=NULL;
	tileWidth=32;
	tileHeight=32;
	viewRows=10;
	viewColumns=10;
	viewOffsetX=0;
	viewOffsetY=0;
	mapRows=0;
	mapCols=0;
	mapFormat=MAP_DEFAULT;
	imageRows=0;
	imageCols=0;
	width1=0;
	width2=0;
	height1=0;
	height2=0;

	//Important to new this
	map=new std::vector<tile*>;
}
staticTileMap::~staticTileMap()
{
	//Wipe the map vector
	if (!map->empty())
	{
		while (!map->empty())
		{
			delete map->back();
			map->pop_back();
		}
	}
	delete map;	
}
//Tells the map displayer which image to use
void staticTileMap::setImage(sprite* newImageMap)
{
	imageMap=newImageMap;
}
//Tells what array to display with the current image map (currently 
//a single dimension stl vector); 
void staticTileMap::setMap(std::vector<tile*> *newMap)
{
	map=newMap;
}
unsigned char staticTileMap::getImageRows()
{
	return imageRows;
}
unsigned char staticTileMap::getImageCols()
{
	return imageCols;
}
//NOTE: It is row, column, which means Y, X !
tile* staticTileMap::getTileAt(unsigned int row, unsigned int column)
{
	if (row>=mapRows || column>=mapCols) return NULL;
	else return (*map)[row * mapCols + column]; 
}
void staticTileMap::setViewOffset(float newX, float newY)
{
	viewOffsetX=newX;
	viewOffsetY=newY;
}
float staticTileMap::getViewOffsetX()
{
	return viewOffsetX;
}
float staticTileMap::getViewOffsetY()
{
	return viewOffsetY;
}
//Remember that these are in row column, not pixels (2 rows at 32 px per row)
void staticTileMap::setViewSize(float newWidth, float newHeight)
{
	viewColumns=newWidth;
	viewRows=newHeight;
}
void staticTileMap::setTileSize(float newX, float newY)
{
	tileWidth=newX;
	tileHeight=newY;
}

float staticTileMap::getTileWidth()
{
	return tileWidth;
}
float staticTileMap::getTileHeight()
{
	return tileHeight;
}
float staticTileMap::getMapRows()
{
	return mapRows;
}
float staticTileMap::getMapCols()
{
	return mapCols;
}
sprite* staticTileMap::getImage()
{
	return imageMap;
}
void staticTileMap::setImageSize(float width, float height)
{
	imageCols=(unsigned char) width;
	imageRows=(unsigned char) height;
}
void staticTileMap::setFormat(unsigned char newFormat)
{
	mapFormat=newFormat;
}
void staticTileMap::setMapSize(unsigned int newWidth1, unsigned int newWidth2, unsigned int newHeight1, unsigned int newHeight2)
{
	width1=newWidth1;
	width2=newWidth2;
	height1=newHeight1;
	height2=newHeight2;
	mapRows=height1 * height2;
	mapCols=width1 * width2;
}

/*Returns a tile at the given mouse position. NULL is returned if 
 * there isn't a tile at that mouse position.*/
tile* staticTileMap::pickTile(int viewX, int viewY, float x, float y)
{
	//Mouse is out of window, so don't process it
	if (x < 0 || y < 0) return NULL;
	//Offset x and y by the map offset
	x-=viewOffsetX;
	y-=viewOffsetY;
	//Convert the mouse position to tile coordinate (window relative)
	x=x/tileWidth;
	y=y/tileHeight;
	//Convert the mouse position from window to map position
	x+=viewX;
	y+=viewY;
	return getTileAt((int)y ,(int)x);
}
//Returns the position in tile map coordinates
int staticTileMap::pointToMapX(int viewX, float x)
{
	//Out of window, so don't process it
	if (x < 0) return -1;
	//Offset x and y by the map offset
	x-=viewOffsetX;
	//Convert the position to tile coordinate (window relative)
	x=x/tileWidth;
	//Convert the position from window to map position
	x+=viewX;
	return x;
}
//Returns the position in tile map coordinates
int staticTileMap::pointToMapY(int viewY, float y)
{
	//Out of window, so don't process it
	if (y < 0) return -1;
	//Offset x and y by the map offset
	y-=viewOffsetY;
	//Convert the position to tile coordinate (window relative)
	y=y/tileHeight;
	//Convert the position from window to map position
	y+=viewY;
	return y;
}
bool staticTileMap::loadMapFromFile(const char* filename)
{
	unsigned int total=0;
	unsigned int temp=0;
	std::ifstream in;
	in.open(filename, std::ios::binary);
	if (in.is_open())
	{
		char current;
		//Parse the header data
		//First two chars are width of map

		if (in.good())
		{
			in.get(current);
			width1=current;
			temp+=(int) (unsigned char)current;
			if (in.good())
			{
				in.get(current);
				width2=current;
				temp*=(int) (unsigned char)current;
				mapCols=(float) temp;
			}
			else return false;
		}
		else return false;
		//Next two chars are height of map
		temp=0;
		if (in.good())
		{
			in.get(current);
			height1=current;
			temp+=(int) (unsigned char)current;
			if (in.good())
			{
				in.get(current);
				height2=current;
				temp*=(int) (unsigned char)current;
				mapRows=temp;
			}
			else return false;
		}
		else return false;
		//Width of the tile
		if (in.good())
		{
			in.get(current);
			tileWidth=(float) (int) current;
		}
		else return false;
		//Height of the tile
		if (in.good())
		{
			in.get(current);
			tileHeight=(float) (int) current;
		}
		else return false;
		//Number of columns required in the image map
		if (in.good())
		{
			in.get(current);
			imageCols=current;
		}
		else return false;
		//Number of rows required in the image map
		if (in.good())
		{
			in.get(current);
			imageRows=current;
		}
		else return false;
		
		//Tile format (currently unsupported)
		if (in.good())
		{
			in.get(current);
			mapFormat=current;
		}
		//Parse the tile data
		tile* newTile;
		while (in.good() && !in.eof())
		{
			newTile=new tile;
			in.get(current);
			if (in.eof())
			{
				delete newTile;
				break;
			}
			newTile->x=(unsigned char) current;
			in.get(current);
			newTile->y=(unsigned char) current;
			map->push_back(newTile);
			++total;
		}
	}
	else return false;
	in.close();
	return true;
}
bool staticTileMap::dumpMapToFile(const char* filename)
{
	unsigned int count=0;
	std::ofstream out;
	out.open(filename, std::ios::binary | std::ios::trunc);
	if (out.is_open())
	{
		//Dump the needed header information
		if (!out.good()) return false;
		//Dump the map width and height
		out << width1 << width2 << height1 << height2;
		//Dump the tile width and height
		out << (unsigned char) tileWidth << (unsigned char) tileHeight;
		//Dump the required rows and columns
		out << imageCols << imageRows;
		//The map format
		out << mapFormat;
		//Dump the map data
		for (std::vector<tile*>::iterator i=map->begin(); i!=map->end(); ++i)
		{
			if (!out.good()) return false;
			out << (*i)->x;
			out << (*i)->y;
			++count;
		}
	}
	else
	{
		return false;
	}
	out.close();
	return true;
}
std::vector<tile*>* staticTileMap::getMap()
{
	return map;
}
//Randomizes the tile map (great for newborns)
void staticTileMap::randomize()
{
	if (map->size()<1) map->resize(mapRows*mapCols);
	srand(SEED);
	for (std::vector<tile*>::iterator i=map->begin(); i!=map->end(); ++i)
	{
		(*i)=new tile;
		(*i)->x=rand() % imageCols;
		(*i)->y=rand() % imageRows;
	}
}
//X and Y are the view position from the top left position; these
//are in tile coordinates, not pixel coordinates (ie 1 could be 32)
//HINT: Use setViewOffset to smoothly scroll the map (negative)
void staticTileMap::drawMap(int viewX, int viewY, window* win)
{
	//Make sure the tile map is being viewed at all
	if (viewX<0) //Is the view position negative?
	{
		if (viewX + viewColumns<0) return; //Will the viewport still show something?
	}
	else 
	{
		if ((unsigned int) viewX > mapCols) return; //Is the view too far to the right?
	}
	if (abs(viewY)!=viewY) //Is the view position negative?
	{
		if (viewY + viewColumns<0) return; //Will the viewport still show something?
	}
	else 
	{
		if ((unsigned int) viewY > mapCols) return; //Is the view too far to the right?
	}
	float x=viewOffsetX;
	float y=viewOffsetY;
	tile* currentTile=NULL;
	for (int i=viewY; i < viewRows + viewY; ++i)
	{
		//We don't need to waste any time with a negative (noexistant) row
		if (i<0)
		{
			y+=tileHeight;
			continue;
		}
		for (int n=viewX; n < viewColumns + viewX; ++n)
		{
			if (n<0)
			{
				x+=tileWidth;
				continue;
			}
			currentTile=getTileAt(i, n);
			if (currentTile !=NULL)
			{
				if (currentTile->x !=NO_DISPLAY && currentTile->y !=NO_DISPLAY)
				{
					imageMap->setPosition(x,y);
					imageMap->setSubRect(currentTile->x * tileWidth, currentTile->y * tileHeight, tileWidth, tileHeight);
					win->draw(imageMap);
				}
			}
			x+=tileWidth;
		} 
		y+=tileHeight;
		//We need to reset the x position for the next row
		x=viewOffsetX;
	}
}
#endif
#ifdef COMPILE_DYNAMIC_MAP
dynamicTileMap::dynamicTileMap()
{
	imageMap=NULL;
	tileWidth=32;
	tileHeight=32;
	viewRows=10;
	viewColumns=10;
	viewOffsetX=0;
	viewOffsetY=0;
	map=NULL;//new std::vector<tile>;
	mapRows=0;
	mapCols=0;
}
dynamicTileMap::~dynamicTileMap()
{
    //std::cout << "clearing\n";
	//map->clear();
	//delete map;
    //std::cout << "done\n";
}
//Tells the map displayer which image to use
void dynamicTileMap::setImage(sprite* newImageMap)
{
	imageMap=newImageMap;
}
//Tells what array to display with the current image map (currently 
//a single dimension stl vector)
void dynamicTileMap::setMap(std::vector<tile> *newMap)
{
	map=newMap;
}
tile* dynamicTileMap::getTileAt(unsigned int row, unsigned int column)
{
	if (!map) return NULL;
	if (row>=mapRows || column>=mapCols) return NULL;
	else return &(*map)[row * mapCols + column]; 
}
void dynamicTileMap::setViewOffset(float newX, float newY)
{
	viewOffsetX=newX;
	viewOffsetY=newY;
}
//Remember that these are in row column, not pixels (2 rows at 32 px per row)
void dynamicTileMap::setViewSize(float newWidth, float newHeight)
{
	viewColumns=newWidth;
	viewRows=newHeight;
}
void dynamicTileMap::setTileSize(float newX, float newY)
{
	tileWidth=newX;
	tileHeight=newY;
}
void dynamicTileMap::setMapSize(unsigned int numCols, unsigned int numRows)
{
	mapRows=numRows;
	mapCols=numCols;
}
float dynamicTileMap::getTileWidth()
{
	return tileWidth;
}
float dynamicTileMap::getTileHeight()
{
	return tileHeight;
}
float dynamicTileMap::getMapRows()
{
	return mapRows;
}
float dynamicTileMap::getMapCols()
{
	return mapCols;
}
std::vector<tile>* dynamicTileMap::getMap()
{
	return map;
}
//Randomizes the tile map (great for newborns)
void dynamicTileMap::randomize(unsigned int maxWidth, unsigned int maxHeight)
{
	if (!map) return;
	map->resize(mapRows*mapCols);
	srand(SEED);
	for (std::vector<tile>::iterator i=map->begin(); i!=map->end(); ++i)
	{
		(*i).x=rand() % maxWidth;
		(*i).y=rand() % maxHeight;
	}
}
void dynamicTileMap::blank(unsigned char x, unsigned char y)
{
	if (!map) return;
	//Note that we don't resize for blanks
	if (map->size()<mapCols * mapRows) map->resize(mapCols * mapRows);
	for (std::vector<tile>::iterator i=map->begin(); i!=map->end(); ++i)
	{
		(*i).x=x;
		(*i).y=y;
	}
}
/*Returns a tile at the given mouse position. NULL is returned if 
 * there isn't a tile at that mouse position.*/
tile* dynamicTileMap::pickTile(int viewX, int viewY, float x, float y)
{
	//Mouse is out of window, so don't process it
	if (x < 0 || y < 0) return NULL;
	//Offset x and y by the map offset
	x-=viewOffsetX;
	y-=viewOffsetY;
	//Convert the mouse position to tile coordinate (window relative)
	x=x/tileWidth;
	y=y/tileHeight;
	//Convert the mouse position from window to map position
	x+=viewX;
	y+=viewY;
	return getTileAt((int)y ,(int)x);
}
int dynamicTileMap::pointToMapX(int viewX, float x)
{
	//Out of window, so don't process it
	if (x < 0) return -1;
	//Offset x and y by the map offset
	x-=viewOffsetX;
	//Convert the position to tile coordinate (window relative)
	x=x/tileWidth;
	//Convert the position from window to map position
	x+=viewX;
	return x;
}
//Returns the position in tile map coordinates
int dynamicTileMap::pointToMapY(int viewY, float y)
{
	//Out of window, so don't process it
	if (y < 0) return -1;
	//Offset x and y by the map offset
	y-=viewOffsetY;
	//Convert the position to tile coordinate (window relative)
	y=y/tileHeight;
	//Convert the position from window to map position
	y+=viewY;
	return y;
}
//X and Y are the view position from the top left position; these
//are in tile coordinates, not pixel coordinates (ie 1 could be 32)
//HINT: Use setViewOffset to smoothly scroll the map (negative)
void dynamicTileMap::drawMap(int viewX, int viewY, window* win)
{
	if (!map) return;
	//Make sure the tile map is being viewed at all
	if (viewX<0) //Is the view position negative?
	{
		if (viewX + viewColumns<0) return; //Will the viewport still show something?
	}
	else 
	{
		if ((unsigned int) viewX > mapCols) return; //Is the view too far to the right?
	}
	if (viewY<0) //Is the view position negative?
	{
		if (viewY + viewRows<0) return; //Will the viewport still show something?
	}
	else 
	{
		if ((unsigned int) viewY > mapRows) return; //Is the view too far to the right?
	}
	
	
	float x=viewOffsetX;
	float y=viewOffsetY;
	tile* currentTile=NULL;
	for (int i=viewY; i < viewRows + viewY; ++i)
	{
		//We don't need to waste any time with a negative (noexistant) row
		if (i<0)
		{
			y+=tileHeight;
			continue;
		}
		for (int n=viewX; n < viewColumns + viewX; ++n)
		{
			if (n<0)
			{
				x+=tileWidth;
				continue;
			}
			currentTile=getTileAt(i, n);
			if (currentTile !=NULL)
			{
				if (currentTile->x !=NO_DISPLAY && currentTile->y !=NO_DISPLAY)
				{
					imageMap->setPosition(x,y);
					imageMap->setSubRect(currentTile->x * tileWidth, currentTile->y * tileHeight, tileWidth, tileHeight);
					win->draw(imageMap);
				}
			}
			x+=tileWidth;
		} 
		y+=tileHeight;
		//We need to reset the x position for the next row
		x=viewOffsetX;
	}
}
#endif
#endif
