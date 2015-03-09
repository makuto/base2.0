#ifndef TILE_CAMERA_HPP
#define TILE_CAMERA_HPP
#include "tileMap.hpp"
/* Because I do not want to touch the tile map
 * to keep compatibility, this class will handle simpler
 * view control of a tile map (static or dynamic)
 *
 * USAGE:
 * --------
 * staticTileMap myMap;
 * tileCamera myCam;
 * 
 * myCam.setMap(&myMap)
 * //Don't start setting anything until your tile map is ready
 * myCam.setViewSize(win.getWidth(), win.getHeight())
 * 
 * myCam.setPosition(340, 345)
 * myMap.render(myCam.getX(), myCam.getY(), &win)*/
class tileCamera
{
	private:
		staticTileMap* staticMap;
		dynamicTileMap* dynamicMap;
		
		int positionX;
		int positionY;
	public:
		tileCamera();
		
		//Set the map the camera will be controlling (it's one or the other)
		void setMap(staticTileMap*);
		void setMap(dynamicTileMap*);

		//Takes the width of the desired viewport (in pixels) and configures map properly
		//Note that your tile map should already be set up properly (tile sizes etc.)
		void setViewSize(float, float);

		//Takes a position (in pixels) and will position it properly with getX & getY
		void setPosition(float, float);

		//Pass these into the render function to get a proper position
		int getX();
		int getY();
};
#endif
