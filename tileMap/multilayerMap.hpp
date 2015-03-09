#ifndef MULTILAYERMAP_HPP
#define MULTILAYERMAP_HPP
#include <vector>
#include <string>
#include "tileMap.hpp"
#include "../graphics/graphics.hpp"

class multilayerMap
{
	private:
		staticTileMap masterMap;
		std::vector<std::vector<tile*>* > layers;
		unsigned int totalLayers;
		unsigned int currentMasterMapLayer;

		//Simple bounds check for indices
		bool isValidIndex(unsigned int);
		//Loads a single raw .maplayerN file
		bool loadLayer(const std::string&, std::vector<tile*>*);
		//Saves a single layer to file
		bool exportAsLayer(const std::string& filename, std::vector<tile*>* map, unsigned int layerIndex);
	public:
		multilayerMap();
		//Important deallocation
		~multilayerMap();
		
		/*This loading function first loads the master map file
		 * (indicated by the filename you provide), which is a normal
		 * single-layer map. It then loads N layers, from seperate .maplayerN
		 * files. These layer files only contain raw tile data, which is why the
		 * master map is used to configure them.
		 * For example:
		 * load("myMap.map", 4) will...
		 * myMap.map----Loaded first, provides dimension details and becomes [0] in layers array
		 * myMap.maplayer1----Loaded next, becomes [1] in layers array
		 * myMap.maplayer2----Loaded next, becomes [2] in layers array
		 * myMap.maplayer3----Loaded next, becomes [3] in layers array
		 * */
		bool load(std::string, unsigned int);
		bool save(std::string);
		//Returns a pointer to the masterMap
		staticTileMap* getMasterMap();
		//Returns a vector array pointer to the requested layer
		std::vector<tile*>* getLayer(unsigned int);
        //Sets the layer at the provided index to the requested layer
        //Note that the layers will be deleted when multilayerMap is
        //destroyed, so make sure to avoid double deletes, and make sure
        //to delete the layers that are already created
        //Index must be within bounds; layer will be ignored otherwise
        //You probably should avoid using this function
        void setLayer(unsigned int, std::vector<tile*>*);

		//Sets the master map to a different layer (default layer 0)
		//You would only do this if you are using the masterMap directly
		//(it's not used for rendering)
		void setMasterMapLayer(unsigned int);
		//Set the image to use
		void setImage(sprite*);

		//Returns total layers
		unsigned int getTotalLayers();

		/* render(lowRange, highRange, viewX, viewY, window)
		 * Renders the range of layers provided
		 * If you want to render one layer, just use 0 for the high range
		 * Examples:
		 * render(0, 5)----Will render six total layers from layer 0 to layer 5
		 * render(3, 5)----Will render three total layers (3, 4, 5)
		 * render(4, 0)----Only renders layer 4
		 * render(0, 0)----Only renders layer 0
		 *
		 * Note that render will set the masterMap layer back to what it was
		 * previously set to after rendering
		*/
		void render(unsigned int, unsigned int, int, int, window*);
		
};
class dynamicMultilayerMap
{
	private:
		dynamicTileMap masterMap;
		std::vector<std::vector<tile>* > layers;
		unsigned int totalLayers;
		unsigned int currentMasterMapLayer;

		//Simple bounds check for indices
		bool isValidIndex(unsigned int);
		//Loads a single raw .maplayerN file
		bool loadLayer(const std::string&, std::vector<tile>*);
		//Saves a single layer to file
		bool exportAsLayer(const std::string& filename, std::vector<tile>* map, unsigned int layerIndex);
        int numRows;
        int numCols;
	public:
		dynamicMultilayerMap(int newNumRows, int newNumCols, int numLayers);
		//Important deallocation
		~dynamicMultilayerMap();
		
		/*This loading function first loads the master map file
		 * (indicated by the filename you provide), which is a normal
		 * single-layer map. It then loads N layers, from seperate .maplayerN
		 * files. These layer files only contain raw tile data, which is why the
		 * master map is used to configure them.
		 * For example:
		 * load("myMap.map", 4) will...
		 * myMap.map----Loaded first, provides dimension details and becomes [0] in layers array
		 * myMap.maplayer1----Loaded next, becomes [1] in layers array
		 * myMap.maplayer2----Loaded next, becomes [2] in layers array
		 * myMap.maplayer3----Loaded next, becomes [3] in layers array
		 * */
		bool load(std::string, unsigned int);
		bool save(std::string);
		//Returns a pointer to the masterMap
		dynamicTileMap* getMasterMap();
		//Returns a vector array pointer to the requested layer
		std::vector<tile>* getLayer(unsigned int);
        //Sets the layer at the provided index to the requested layer
        //Note that the layers will be deleted when multilayerMap is
        //destroyed, so make sure to avoid double deletes, and make sure
        //to delete the layers that are already created
        //Index must be within bounds; layer will be ignored otherwise
        //You probably should avoid using this function
        void setLayer(unsigned int, std::vector<tile>*);

		//Sets the master map to a different layer (default layer 0)
		//You would only do this if you are using the masterMap directly
		//(it's not used for rendering)
		void setMasterMapLayer(unsigned int);
		//Set the image to use
		void setImage(sprite*);

		//Returns total layers
		unsigned int getTotalLayers();

		/* render(lowRange, highRange, viewX, viewY, window)
		 * Renders the range of layers provided
		 * If you want to render one layer, just use 0 for the high range
		 * Examples:
		 * render(0, 5)----Will render six total layers from layer 0 to layer 5
		 * render(3, 5)----Will render three total layers (3, 4, 5)
		 * render(4, 0)----Only renders layer 4
		 * render(0, 0)----Only renders layer 0
		 *
		 * Note that render will set the masterMap layer back to what it was
		 * previously set to after rendering
		*/
		void render(unsigned int, unsigned int, int, int, window*);
		
};
#endif
