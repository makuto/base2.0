#ifndef TILEMAP_HPP
#define TILEMAP_HPP
#include <vector>
#include <fstream>
#include "../graphics/graphics.hpp"

//Which map classes to compile
#define COMPILE_STATIC_MAP
#define COMPILE_DYNAMIC_MAP

extern const unsigned int SEED;
//Map formats
extern const unsigned char MAP_DEFAULT;
extern const unsigned char NO_DISPLAY;

struct tile
{
    unsigned char x;
    unsigned char y;
};
#ifdef COMPILE_STATIC_MAP
class staticTileMap
{
    private:
	    sprite* imageMap;
	    //The width of individual tiles
	    float tileWidth;
	    float tileHeight;
	    //How many rows/columns required of the image map (how many tiles are possible)
	    unsigned char imageRows;
	    unsigned char imageCols;
	    //The number of columns and rows to display
	    float viewRows;
	    float viewColumns;
	    //The entire map offset
	    float viewOffsetX;
	    float viewOffsetY;
		//The map itself
	    std::vector<tile*> *map;
	    unsigned int mapRows;
	    unsigned int mapCols;
	    unsigned char mapFormat;
	    unsigned char width1;
	    unsigned char width2;
	    unsigned char height1;
	    unsigned char height2;
    public:
		staticTileMap();
		~staticTileMap();
		
		unsigned char  getImageRows();
		unsigned char  getImageCols();
		//NOTE: It is row, column, which means Y, X !
		tile*  getTileAt(unsigned int row, unsigned int column);
		float  getViewOffsetX();
		float  getViewOffsetY();
		float  getTileWidth();
		float  getTileHeight();
		float  getMapRows();
		float  getMapCols();
		sprite*  getImage();

        //Oddly enough, this is shoud actually be image size divided by the tile size
		void  setImageSize(float width, float height);
		void  setFormat(unsigned char newFormat);
		void  setMapSize(unsigned int newWidth1, unsigned int newWidth2, unsigned int newHeight1, unsigned int newHeight2);
		//Remember that these are in row column, not pixels (2 rows at x px per row)
		void  setViewSize(float newWidth, float newHeight);
		
		void  setTileSize(float newX, float newY);
		void  setViewOffset(float newX, float newY);
		//Tells the map displayer which image to use
		void  setImage(sprite* newImageMap);
		//Set a new array of tiles for the map
		//VERY IMPORTANT: The previous array will be lost, which will leak!
		//You should save the previous array (using getMap()), then reset it
		//before destruction!
		//ALSO VERY IMPORTANT: This array will be deleted during destruction!
		//Because of these gotchas, it is recommended that you avoid using this function
		void setMap(std::vector<tile*>*);
		/*Returns a tile at the given mouse position. NULL is returned if 
		 * there isn't a tile at that mouse position.*/
		tile*  pickTile(int viewX, int viewY, float x, float y);
		//Returns the position in tile map coordinates
		int  pointToMapX(int viewX, float x);
		//Returns the position in tile map coordinates
		int  pointToMapY(int viewY, float y);
		
		bool  loadMapFromFile(const char* filename);
		bool  dumpMapToFile(const char* filename);
		
		std::vector<tile*>*  getMap();
		//Randomizes the tile map (great for newborns)
		void  randomize();
		//X and Y are the view position from the top left position; these
		//are in tile coordinates, not pixel coordinates (ie 1 could be 32)
		//HINT: Use setViewOffset to smoothly scroll the map (negative)
		void  drawMap(int viewX, int viewY, window* win);
};
#endif
#ifdef COMPILE_DYNAMIC_MAP
//Note that you will get segfaults if a map is never assigned
class dynamicTileMap
{
	private:
	    sprite* imageMap;
	    //The width of individual tiles
	    float tileWidth;
	    float tileHeight;
	    //The number of columns and rows to display
	    float viewRows;
	    float viewColumns;
	    //The entire map offset
	    float viewOffsetX;
	    float viewOffsetY;
		//The map itself
	    std::vector<tile> *map;
	    unsigned int mapRows;
	    unsigned int mapCols;
    public:
		dynamicTileMap();
		~dynamicTileMap();
    
	    //Tells the map displayer which image to use
	    void setImage(sprite* newImageMap);
	    //Tells what array to display with the current image map (currently 
	    //a single dimension STL vector)
	    void setMap(std::vector<tile> *newMap);
		void setViewOffset(float newX, float newY);
		//Remember that these are in row column, not pixels (ex 2 rows at 32 px per row)
		void setViewSize(float newWidth, float newHeight);
		void setTileSize(float newX, float newY);
		void setMapSize(unsigned int numCols, unsigned int numRows);
		
		float getTileWidth();
		float getTileHeight();
		float getMapRows();
		float getMapCols();
	    tile* getTileAt(unsigned int row, unsigned int column);
	    std::vector<tile>* getMap();
	    
		//Randomizes the tile map (great for newborns)
		void  randomize(unsigned int maxWidth, unsigned int maxHeight);
		//Also good for new maps. Sets all tiles to the user's requested value
		void blank(unsigned char x, unsigned char y);
		
		/*Returns a tile at the given position. NULL is returned if 
		 * there isn't a tile at that position.*/
		tile* pickTile(int viewX, int viewY, float x, float y);
		//Returns the position in tile map coordinates
		int  pointToMapX(int viewX, float x);
		//Returns the position in tile map coordinates
		int  pointToMapY(int viewY, float y);
		//X and Y are the view position from the top left position; these
		//are in tile coordinates, not pixel coordinates (ie 1 could be 32)
		//HINT: Use setViewOffset to smoothly scroll the map (negative)
		void drawMap(int viewX, int viewY, window* win);
};
#endif

#endif
