#ifndef MULTILAYERMAP_CPP
#define MULTILAYERMAP_CPP
#include <fstream>
#include <sstream>
#include <iostream>
#include "multilayerMap.hpp"

multilayerMap::multilayerMap()
{
	totalLayers=0;
	currentMasterMapLayer=0;
}
//Important deallocation
multilayerMap::~multilayerMap()
{
	//Make sure layers isn't empty
	if (layers.empty()) return;
	//Reset masterMap's default layer (it deallocates it automatically)
	masterMap.setMap(layers[0]);
	//Deallocate layers 1-N
	std::vector<std::vector<tile*>* >::iterator it=layers.begin();
	//Skip first layer because masterMap will handle it
	++it;
	for (; it!=layers.end(); ++it)
	{
		//Loop through the layer and delete each tile
		for (std::vector<tile*>::iterator del=(*it)->begin(); del!=(*it)->end(); ++del)
		{
			delete (*del);
		}
		//Clear the map vector
		(*it)->clear();
		delete (*it);
	}
	//Clear the layers vector
	layers.clear();
}
bool multilayerMap::isValidIndex(unsigned int index)
{
	if (index<totalLayers) return true;
	else return false;
}
bool multilayerMap::loadLayer(const std::string& filename, std::vector<tile*>* layerVector)
{
	std::ifstream in;
	in.open(filename.c_str(), std::ios::binary);
	if (in.is_open())
	{
		char current;
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
			layerVector->push_back(newTile);
		}
	}
	else return false;
	in.close();
	return true;
}
bool multilayerMap::exportAsLayer(const std::string& filename, std::vector<tile*>* map, unsigned int layerIndex)
{
	std::ofstream out;
	std::ostringstream s;
	s << filename << "layer" << layerIndex;
	out.open(s.str().c_str(), std::ios::binary | std::ios::trunc);
	if (out.is_open())
	{
		for (std::vector<tile*>::iterator it=map->begin(); it!=map->end(); ++it)
		{
			if (!out.good())
			{
				out.close();
				return false;
			};
			out << (*it)->x;
			out << (*it)->y;
		}
	}
	else return false;
	out.close();
	return true;
}
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
bool multilayerMap::load(std::string filename, unsigned int newTotalLayers)
{
	totalLayers=newTotalLayers;
	//Load the master layer, which holds configuration details for all layers as well
	//as layer 0
	if (!masterMap.loadMapFromFile(filename.c_str()))
	{
		//Failed to load map
        //std::cout << "  multilayerMap.cpp.112: Failed to load master map '" << filename << "'\n";
		return false;
	}
	//Add the master map layer to the layers map
	layers.push_back(masterMap.getMap());

	//Load new layers file by file
	//std::string layerFileName=filename;
	for (unsigned int i=1; i<totalLayers; ++i)
	{
		std::ostringstream temp;
		//Set the filename to the correct layer
		temp << filename << "layer" << i;
		//Create the new layer and load it
		std::vector<tile*>* newLayer=new std::vector<tile*>;
		if (!loadLayer(temp.str(), newLayer))
		{
            //std::cout << "  multilayerMap.cpp.130: Failed to load map layer '" << temp.str() << "'\n";
			delete newLayer;
			return false;
		}
		layers.push_back(newLayer);
	}
	return true;
}
//Saves the map and all layers
bool multilayerMap::save(std::string filename)
{
	setMasterMapLayer(0);
	//Save layer 0
	if (!masterMap.dumpMapToFile(filename.c_str())) return false;
	//Export all layers
	for (unsigned int i=1; i<totalLayers; ++i)
	{
		if (!exportAsLayer(filename, layers[i], i)) return false;
	}
	return true;
}
//Returns a pointer to the masterMap
staticTileMap* multilayerMap::getMasterMap()
{
	return &masterMap;
}
//Returns a vector array pointer to the requested layer
std::vector<tile*>* multilayerMap::getLayer(unsigned int index)
{
	if (isValidIndex(index))
	{
		return layers[index];
	}
	else return NULL;
}
void multilayerMap::setLayer(unsigned int index, std::vector<tile*>* newLayer)
{
    if (isValidIndex(index))
	{
		layers[index] = newLayer;
	}
	else return;
}
//Sets the master map to a different layer (default layer 0)
//You would only do this if you are using the masterMap directly
//(it's not used for rendering)
void multilayerMap::setMasterMapLayer(unsigned int index)
{
	if (isValidIndex(index))
	{
		masterMap.setMap(layers[index]);
		currentMasterMapLayer=index;
	}
}

void multilayerMap::setImage(sprite* newImage)
{
	masterMap.setImage(newImage);
}
unsigned int multilayerMap::getTotalLayers()
{
	return totalLayers;
}
/*Renders the range of layers provided
 * If you want to render one layer, just use 0 for the high range
 * Examples:
 * render(0, 5)----Will render six total layers from layer 0 to layer 5
 * render(3, 5)----Will render three total layers (3, 4, 5)
 * render(4, 0)----Only renders layer 4
 * render(0, 0)----Only renders layer 0
*/
void multilayerMap::render(unsigned int lowerIndex, unsigned int upperIndex, int x, int y, window* win)
{
	//They only want to render one layer
	if (!upperIndex && isValidIndex(lowerIndex))
	{
		masterMap.setMap(layers[lowerIndex]);
		masterMap.drawMap(x, y, win);
	}
	else
	{
		//Bounds check
		if (isValidIndex(lowerIndex) && isValidIndex(upperIndex))
		{
			for (unsigned int i=lowerIndex; i<=upperIndex; ++i)
			{
				masterMap.setMap(layers[i]);
				masterMap.drawMap(x, y, win);
			}
		}
	}
	//Set the map back to the selected layer
	setMasterMapLayer(currentMasterMapLayer);
}


dynamicMultilayerMap::dynamicMultilayerMap(int newNumRows, int newNumCols, int numLayers)
{
	totalLayers=0;
	currentMasterMapLayer=0;
    numRows = newNumRows;
    numCols = newNumCols;
    masterMap.setMapSize(numRows, numCols);
    totalLayers = numLayers;
    layers.resize(totalLayers);
}
//Important deallocation
dynamicMultilayerMap::~dynamicMultilayerMap()
{
	//Make sure layers isn't empty
	if (layers.empty()) return;
	//Reset masterMap's default layer (it deallocates it automatically)
	masterMap.setMap(layers[0]);
	//Deallocate layers 1-N
	/*std::vector<std::vector<tile>* >::iterator it=layers.begin();
	//Skip first layer because masterMap will handle it
	++it;
	for (it; it!=layers.end(); ++it)
	{
		//Loop through the layer and delete each tile
		for (std::vector<tile>::iterator del=(*it)->begin(); del!=(*it)->end(); ++del)
		{
			delete (*del);
		}
		//Clear the map vector
		(*it)->clear();
		delete (*it);
	}
	//Clear the layers vector
	layers.clear();*/
}
bool dynamicMultilayerMap::isValidIndex(unsigned int index)
{
	if (index<totalLayers) return true;
	else return false;
}
bool dynamicMultilayerMap::loadLayer(const std::string& filename, std::vector<tile>* layerVector)
{
	/*std::ifstream in;
	in.open(filename.c_str(), std::ios::binary);
	if (in.is_open())
	{
		char current;
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
			layerVector->push_back(newTile);
		}
	}
	else return false;
	in.close();
	return true;*/
    return false;
}
bool dynamicMultilayerMap::exportAsLayer(const std::string& filename, std::vector<tile>* map, unsigned int layerIndex)
{
	std::ofstream out;
	std::ostringstream s;
	s << filename << "layer" << layerIndex;
	out.open(s.str().c_str(), std::ios::binary | std::ios::trunc);
	if (out.is_open())
	{
		for (std::vector<tile>::iterator it=map->begin(); it!=map->end(); ++it)
		{
			if (!out.good())
			{
				out.close();
				return false;
			};
			out << (*it).x;
			out << (*it).y;
		}
	}
	else return false;
	out.close();
	return true;
}
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
bool dynamicMultilayerMap::load(std::string filename, unsigned int newTotalLayers)
{
	/*totalLayers=newTotalLayers;
	//Load the master layer, which holds configuration details for all layers as well
	//as layer 0
	if (!masterMap.loadMapFromFile(filename.c_str()))
	{
		//Failed to load map
        //std::cout << "  dynamicMultilayerMap.cpp.112: Failed to load master map '" << filename << "'\n";
		return false;
	}
	//Add the master map layer to the layers map
	layers.push_back(masterMap.getMap());

	//Load new layers file by file
	//std::string layerFileName=filename;
	for (unsigned int i=1; i<totalLayers; ++i)
	{
		std::ostringstream temp;
		//Set the filename to the correct layer
		temp << filename << "layer" << i;
		//Create the new layer and load it
		std::vector<tile>* newLayer=new std::vector<tile>;
		if (!loadLayer(temp.str(), newLayer))
		{
            //std::cout << "  dynamicMultilayerMap.cpp.130: Failed to load map layer '" << temp.str() << "'\n";
			delete newLayer;
			return false;
		}
		layers.push_back(newLayer);
	}
	return true;*/
    return false;
}
//Saves the map and all layers
bool dynamicMultilayerMap::save(std::string filename)
{
	/*setMasterMapLayer(0);
	//Save layer 0
	if (!masterMap.dumpMapToFile(filename.c_str())) return false;
	//Export all layers
	for (unsigned int i=1; i<totalLayers; ++i)
	{
		if (!exportAsLayer(filename, layers[i], i)) return false;
	}
	return true;*/
    return false;
}
//Returns a pointer to the masterMap
dynamicTileMap* dynamicMultilayerMap::getMasterMap()
{
	return &masterMap;
}
//Returns a vector array pointer to the requested layer
std::vector<tile>* dynamicMultilayerMap::getLayer(unsigned int index)
{
	if (isValidIndex(index))
	{
		return layers[index];
	}
	else return NULL;
}
void dynamicMultilayerMap::setLayer(unsigned int index, std::vector<tile>* newLayer)
{
    if (isValidIndex(index))
	{
		layers[index] = newLayer;
	}
	else
    {
        return;
    }
}
//Sets the master map to a different layer (default layer 0)
//You would only do this if you are using the masterMap directly
//(it's not used for rendering)
void dynamicMultilayerMap::setMasterMapLayer(unsigned int index)
{
	if (isValidIndex(index))
	{
		masterMap.setMap(layers[index]);
		currentMasterMapLayer=index;
	}
}

void dynamicMultilayerMap::setImage(sprite* newImage)
{
	masterMap.setImage(newImage);
}
unsigned int dynamicMultilayerMap::getTotalLayers()
{
	return totalLayers;
}
/*Renders the range of layers provided
 * If you want to render one layer, just use 0 for the high range
 * Examples:
 * render(0, 5)----Will render six total layers from layer 0 to layer 5
 * render(3, 5)----Will render three total layers (3, 4, 5)
 * render(4, 0)----Only renders layer 4
 * render(0, 0)----Only renders layer 0
*/
void dynamicMultilayerMap::render(unsigned int lowerIndex, unsigned int upperIndex, int x, int y, window* win)
{
	//They only want to render one layer
	if (!upperIndex && isValidIndex(lowerIndex))
	{
		masterMap.setMap(layers[lowerIndex]);
		masterMap.drawMap(x, y, win);
	}
	else
	{
		//Bounds check
		if (isValidIndex(lowerIndex) && isValidIndex(upperIndex))
		{
			for (unsigned int i=lowerIndex; i<=upperIndex; ++i)
			{
				masterMap.setMap(layers[i]);
				masterMap.drawMap(x, y, win);
			}
		}
	}
	//Set the map back to the selected layer
	setMasterMapLayer(currentMasterMapLayer);
}
#endif
