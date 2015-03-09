#ifndef TILEDIMPORTER_CPP
#define TILEDIMPORTER_CPP
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <base2.0/graphics/graphics.hpp>
#include <base2.0/input/input.hpp>
#include <base2.0/timer/timer.hpp>
#include <base2.0/tileMap/tileMap.hpp>
#include <base2.0/tileMap/tileCamera.hpp>
#include <base2.0/tileMap/multilayerMap.hpp>
#include <base2.0/ept/eptParser.hpp>
#include "/home/macoy/Downloads/pugixml-1.4/src/pugixml.hpp"

const unsigned char MAP_FORMAT = 1;
//Takes a pugi::xml_node at the data level (map.layer.data), the tileMap
//(used only to get the map metadata like image size and map size), and
//the vector you want to populate with the CSV data. tileArray should already
//be sized appropriately (use resize(tileMap.getMapCols() * tileMap.getMapRows())
//or staticTileMap randomize)
void csvToTileVector(pugi::xml_node* layerData, staticTileMap& tileMap, std::vector<tile*>& tileArray)
{
    
    //Parse CSV into vector
    std::string csvDataString = layerData->child_value();
    std::vector<int> csvData;
    for (unsigned int i = 0; i < csvDataString.length(); i++)
    {
        switch (csvDataString[i])
        {
            case ' ':
                break;
            case ',':
                break;
            case '\n':
                break;
            default:
                std::string singleValue;
                for (unsigned int n = i; n < csvDataString.length(); n++)
                {
                    if (csvDataString[n] != ',' && csvDataString[n] != '\n')
                    {
                        singleValue += csvDataString[n];
                    }
                    else
                    {
                        i = n;
                        break;
                    }
                }
                csvData.push_back(attrToInt(singleValue));
                break;
        }
    }
    if (csvData.size() != tileMap.getMapCols() * tileMap.getMapRows())
    {
        std::cout << "WARNING: CSV data did not meet the required map length!\n";
    }
    //Parse the ints from CSV into tile objects
    int index = 0;
    for (std::vector<tile*>::iterator it = tileArray.begin(); it != tileArray.end(); it++)
    {
        int currentTileValue = csvData[index];
        if (currentTileValue==0) //NULL tile
        {
            (*it)->x = 255;
            (*it)->y = 255;
        }
        else
        {
            //Convert the single value to a x, y tile
            int row = currentTileValue / tileMap.getImageCols();
            int mod = currentTileValue % tileMap.getImageCols();
            int col = (currentTileValue-1) - (row * tileMap.getImageCols());
            if (mod==0) //Edges must be handled in a different way (TODO: why?) 
            {
                col = tileMap.getImageCols() -1;
                row--;
            }
            (*it)->x = col;
            (*it)->y = row;
        }
        index++;
    }
}
//Export a .maplayer
bool exportAsLayer(const std::string& filename, std::vector<tile*>* map, unsigned int layerIndex)
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
int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: tiledImporter [File to import] [Export filename] [optional args]\n"
        << "Example: tiledImporter ~/maps/map1.tmx ~/maps/converted/map1.map\n"
        << "\n Optional arguments: \n   -p  Open a window to preview the map\n\n"
        << "Export from Tiled (if tiledImporter installed): \n  gnome-terminal -e \"tiledImporter %mapfile %mapfile.map\"\n"
        <<"Note that the .tmx in the second arg will be trimmed for better naming\n\n"
        << "(Requires at least 2 arguments, " << argc-1 <<" provided)\n";
        return -1;
    }
    char* fileToImport = argv[1];
    char* exportFileNameArg = argv[2];
    std::string options = "";
    //User provided an optional argument
    if (argc==4)
    {
        options = argv[3];
    }
    //Trim .tmx from exportFileName if it is there (so you don't have .tmx.map)
    std::string newExportFileName(exportFileNameArg);
    std::size_t tmxPos = newExportFileName.find(".tmx");
    std::string exportFileName;
    if (tmxPos != std::string::npos)
    {
        std::string first = newExportFileName.substr(0, tmxPos);
        std::string last = newExportFileName.substr(tmxPos + 4, newExportFileName.length());
        std::ostringstream newFileName;
        newFileName << first << last;
        exportFileName = newFileName.str();
    }
    else
    {
        exportFileName = exportFileNameArg;
    }
    
    //Load the XML file with Tiled CSV data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileToImport);
    std::cout << "XML parsed, Result: " << result.description() << "\n";

    //Get tile map attributes from the XML
    std::string tileMapImageFilename = doc.child("map").child("tileset").child("image").attribute("source").value();
    int tileMapImageWidth = doc.child("map").child("tileset").child("image").attribute("width").as_int();
    int tileMapImageHeight = doc.child("map").child("tileset").child("image").attribute("height").as_int();
    int tileMapTileWidth = doc.child("map").child("tileset").attribute("tilewidth").as_int();
    int tileMapTileHeight = doc.child("map").child("tileset").attribute("tileheight").as_int();
    int tileMapWidth = doc.child("map").attribute("width").as_int();
    int tileMapHeight = doc.child("map").attribute("height").as_int();
    int tileMapWidth1 = 1;
    int tileMapHeight1 = 1;

    //Get properties specifying width, width1, height, and height1 because
    //it's too hard to find these with just a width and height (only applies
    //to maps larger than 255)
    if (tileMapWidth > 255 || tileMapHeight > 255)
    {
        std::cout << "WARNING: Map is larger than binary allowance;\n" <<
        "you must have properties width, width1, height, height1 specified!";
        tileMapWidth = doc.child("map").child("properties").find_child_by_attribute("property", "name", "width").attribute("value").as_int();
        tileMapWidth1 = doc.child("map").child("properties").find_child_by_attribute("property", "name", "width1").attribute("value").as_int();
        tileMapHeight = doc.child("map").child("properties").find_child_by_attribute("property", "name", "height").attribute("value").as_int();
        tileMapHeight1 = doc.child("map").child("properties").find_child_by_attribute("property", "name", "height1").attribute("value").as_int();
    }

    //Load the tile set
    //sprite tileMapImage;
    //if (!tileMapImage.load(tileMapImageFilename.c_str())) return -1;
    
    //Prepare a static map with the XML attributes
    staticTileMap testMap;
    //testMap.setImage(&tileMapImage);
    testMap.setImageSize(tileMapImageWidth / tileMapTileWidth, tileMapImageHeight / tileMapTileHeight);
    testMap.setTileSize(tileMapTileWidth, tileMapTileHeight);
    testMap.setMapSize(tileMapWidth, tileMapWidth1, tileMapHeight, tileMapHeight1);
    //Makes the testMap array the right size (prepares it for csvToTileVector)
    testMap.randomize();
    std::vector<tile*> mapData = *testMap.getMap();
    pugi::xml_node layerData = doc.child("map").child("layer").child("data");
    //Parses CSV for master layer
    csvToTileVector(&layerData, testMap, mapData);
    //Set map format
    //TODO: Actually make this work?
    testMap.setFormat(MAP_FORMAT);
    
    //Dump master map layer (layer 0)
    std::cout << "  Dump map success: " << testMap.dumpMapToFile(exportFileName.c_str()) << "\n";
    
    //Parse other layers (if any)
    int numLayers = 0;
    for (pugi::xml_node layerNode = doc.child("map").child("layer").next_sibling("layer"); layerNode; layerNode = layerNode.next_sibling("layer"))
    {
        ++numLayers;
        pugi::xml_node currentLayerData = layerNode.child("data");
        std::cout << "  Parsing layer '" << layerNode.attribute("name").value() << "'...\n"; 
        //Parses CSV for layer
        csvToTileVector(&currentLayerData, testMap, mapData);
        exportAsLayer(exportFileName, &mapData, numLayers);
    }
    std::cout << "Layers parsed: " << numLayers << "\n";
    
    //Confirm successful export by loading the exported map back into a multilayerMap
    multilayerMap finalMap;
    if (!finalMap.load(exportFileName, numLayers + 1))
    {
        std::cout << "Multilayer map failed to load! There might have been a problem exporting...\n";
        std::cout << "Export file name: " << exportFileName << "\n";
        return -1;
    }
    std::cout << "Successfully exported map '" << exportFileName <<"'\n";

    //Debug preview (if user specified option (-p))
    if (options=="-p")
    {
        std::cout << "Previewing map...\n";
        window win(1024, 600, "TiledImporter Preview (ESC or Space to close)");
        timer frameTime;
        frameTime.start();
        float viewSpeed = 100;
        float viewX = 0;
        float viewY = 0;
        inputManager in(&win);
        tileCamera cam;
        cam.setMap(finalMap.getMasterMap());
        cam.setViewSize(win.getWidth(), win.getHeight());

        //Load the tile set
        sprite tileMapImage;
        if (!tileMapImage.load(tileMapImageFilename.c_str())) return -1;
        std::cout << "Loaded tileset '" << tileMapImageFilename << "' successfully\n";
        //Prepare map
        finalMap.setImage(&tileMapImage);
        //Debug preview of exported map
        while (!win.shouldClose() && !in.isPressed(inputCode::Space) && !in.isPressed(inputCode::Escape))
        {
            //Move view
            if (in.isPressed(inputCode::Up))
            {
                viewY += -viewSpeed * frameTime.getTime();
            }
            if (in.isPressed(inputCode::Down))
            {
                viewY += viewSpeed * frameTime.getTime();
            }
            if (in.isPressed(inputCode::Left))
            {
                viewX += -viewSpeed * frameTime.getTime();
            }
            if (in.isPressed(inputCode::Right))
            {
                viewX += viewSpeed * frameTime.getTime();
            }
            cam.setPosition(viewX, viewY);
            finalMap.render(0, numLayers, cam.getX(), cam.getY(), &win);
            frameTime.start();
            win.update();
        }
    }
    return 1;
}
#endif
