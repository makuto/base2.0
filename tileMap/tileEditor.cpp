#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../input/input.hpp"
#include "../timer/timer.hpp"
#include "../collision/collision.hpp"
#include "tileMap.hpp"
//To link: g++ -Wall -o "%e" "%f" "../../resources/base2.0/lib/base.a" -lsfml-graphics -lsfml-window -lsfml-system
//Tile editor now included in base2.0. Use that one more than this one

using namespace std;

const float CLICK_PAUSE=0.2;
const float MOUSE_TOLERANCE=1;

sprite imgMap;
window win (1024, 648, "Tile Map Editor");
struct selection
{
	unsigned int x;
	unsigned int y;
};
bool exportAsLayer(const std::string& filename, std::vector<tile*>* map)
{
	ofstream out;
	ostringstream s;
	unsigned int layerIndex=1;
	cout << "Layer index: ";
	cin >> layerIndex;
	s << filename << "layer" << layerIndex;
	out.open(s.str().c_str(), ios::binary | ios::trunc);
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
int main()
{
	string input;
	string mapFileName;
	bool newMap=true;
	float inputX=1;
	float inputY=1;
	//For the image map
	unsigned int rows=0;
	unsigned int columns=0;
    win.setBackgroundColor(125, 125, 125, 255);
    win.update();
    inputManager in;
    in.setWindow(win.getBase());
    /////////////////////////////////////////////
    cout << "Tile Map Editor v.1\n-------------------------\n";
    staticTileMap mapManager;
    /*mapFileName="headerMap.map";
    if (!mapManager.loadMapFromFile(mapFileName.c_str())) return 0;*/
    
    //Load saved map or configure mapManager for new map
    while (1)
    {
		cout << "Specify map file (or input N for a new map): ";
		cin >> input;
		if (input=="N")
		{
			cout << "	Specify the new file's name: ";
			cin >> input;
			ofstream out;
			out.open(input.c_str(), ios::trunc);
			if (out.good())
			{
				out.close();
				newMap=true;
				mapFileName=input;
				cout << "	Map width base (<256): ";
				int widthA, widthB; 
				cin >> widthA;
				cout << "	Map width multiple (<256): ";
				cin >> widthB;
				cout << "	Map height base (<256): ";
				int heightA, heightB;
				cin >> heightA;
				cout << "	Map height multiple (<256): ";
				cin >> heightB;
				mapManager.setMapSize((unsigned char)widthA,(unsigned char)widthB,(unsigned char)heightA,(unsigned char)heightB);
				cout << "	Tile width: ";
				cin >> inputX;
				cout << "	Tile height: ";
				cin >> inputY;
				mapManager.setTileSize(inputX, inputY);
				cout << "	Required columns (X): ";
				cin >> inputX;
				cout << "	Required rows (Y): ";
				cin >> inputY;
				mapManager.setImageSize(inputX, inputY);
				cout << "	Map format (in numbers): ";
				cin >> inputX;
				cout << "		Setting format...";
				mapManager.setFormat((unsigned char) inputX);
				cout << "done!\n";
				cout << "Filling with boatloads of random tiles...\n";
				mapManager.randomize();
				cout << "done!\n";
				cout << "New map file successfully created!\n";
				break;
			}
			else
			{
				cout << "		There was an error creating the new file, please try again\n";
				continue;
			}
		}
		if (input.length())
		{
			if (mapManager.loadMapFromFile(input.c_str())) 
			{
				cout << "Map successfully loaded!\n";
				newMap=false;
				mapFileName=input;
				break;
			}
			else cout << "	There was a problem opening the file. Please try again.\n";
		}
	}

	//Unimportant debug output
	cout << "Tile size: " << mapManager.getTileWidth() << " , " << mapManager.getTileHeight() << "\n";
	cout << "Map size: " << mapManager.getMapCols() << " , " << mapManager.getMapRows() << "\n";
	
	///Allow the user to select the image map they will be using (remove when integrated into map file)
    while (1)
    {
		cout << "Please enter the image map location: ";
	    cin >> input;
	    cout << "	Loading image map...\n";
	    if (!imgMap.load(input.c_str()))
	    {
			cout << "	Unable to load " << input << ", please try again.\n";
		}
	    else 
	    {
			if (imgMap.getWidth()<mapManager.getImageCols() * mapManager.getTileWidth()
			|| imgMap.getHeight()<mapManager.getImageRows() * mapManager.getTileHeight())
			{
				cout << "WARNING: Map requires more tiles than this image map offers!\n";
			}
			cout << "	Image successfully loaded!\n";
			break;
		}
	}

	//Set up the image map and tell the map manager
    imgMap.setSmooth(false);
    /////////////////////////////////////////////
    //tileMap mapManager;
    mapManager.setImage(&imgMap);

    //Resize the view to show the max amount of tiles for this window size
	mapManager.setViewSize((win.getWidth()/mapManager.getTileWidth()) + 1, (win.getHeight()/mapManager.getTileHeight()) + 1);

	cout << "Press SPACE to use console input. h=Help\n";
    /////////////////////////////////////////////
    float viewX=0;
    float viewY=0;
    float frames=0;
    //float time=0;
    timer currTime;
    currTime.start();
    float speed=5;

    rows=mapManager.getImageRows();
    columns=mapManager.getImageCols();
    //The tile in the image map that is currently in use
    tile currentSelection;
    currentSelection.x=currentSelection.y=0;
    tile* selected=NULL;
    
	//Special editing tools variables 
    //Not actually tiles, but they hold two unsigned chars which is what I need
    selection selectionA;
    selection selectionB;
    selectionA.x=selectionA.y=selectionB.x=selectionB.y=0;
    
    //Text output
    text outputText;
    if (!outputText.loadFont("editorFont.ttf"))
    {
		std::cout << "WARNING: editorFont.ttf couldn't be loaded. No window text will display.\n";
	}
    outputText.setText("Tile Map Editor v.1 by Macoy Madson");
    outputText.setPosition(0,0);
    outputText.setSize(15);
    outputText.setColor(0,0,0,255);
    
    //Click timer
    timer click;
    click.start();
    
    //For the tile select image
    bool isShowing=false;
    float selectX=0;
    float selectY=0;
    sprite* selectImg=mapManager.getImage();
    aabb selectBound(0,0,selectImg->getWidth(), selectImg->getHeight());
    aabb mouseBound(0,0, MOUSE_TOLERANCE, MOUSE_TOLERANCE);
    
    //A small dot that aligns with the grid cell nearest the mouse
    /*sprite cursor;
    cursor.load("data/cursor.png");
    cursor.setOrigin(3, 3);*/
    
    ////////////////////////////////////////////////////////////////////
    ///////////////////Main loop////////////////////////////////////////
    while (!win.shouldClose())
    {
		//Quit on escape
        if (in.isPressed(sf::Keyboard::Escape))
        {
			break;
		}
		
		///////CLI Activation//////////////////////////////
		if (in.isPressed(sf::Keyboard::Space))
		{
			cout << "Command: ";
			cin >> input;
			if (input=="h")
			{
				cout << "h=Help\n"
				<<"q=Quit\n"
				<<"s=Save\n"
				<<"wipe=Wipe the map\n"
				<<"chImg=Change the image map\n"
				<<"keys=Controls\n"
				<<"sel=Pick a tile in the image map (row, column)\n"
				<<"swap=The tile you input will be swapped with the selected tile\n";
			}
			if (input=="q")
			{
				cout << "Save changes before quitting? (y/n): ";
				cin >> input;
				if (input=="y") break;
				else return 1;
			}
			if (input=="s")
			{		
				cout << "Saving map...\n";
			    if (!mapManager.dumpMapToFile(mapFileName.c_str())) cout << "Failed to save map!\n";
			    else cout << "Successfully saved map!\n";
			}
			if (input=="wipe")
			{
				cout << "Are you SURE you want to wipe the map? (y/n): ";
				cin >> input;
				if (input=="y")
				{
					cout << "	Wiping map to "<<(int)currentSelection.x<<" , "<<(int)currentSelection.y<<"...";
					std::vector<tile*> *map=mapManager.getMap();
					for (std::vector<tile*>::iterator i=map->begin(); i!=map->end(); ++i)
					{
						(*i)->x=currentSelection.x;
						(*i)->y=currentSelection.y;
					}
					cout << "	Done wiping\n";
				}
			}
			if (input=="chImg")
			{
				while (1)
			    {
					cout << "Please enter the image map location: ";
				    cin >> input;
				    cout << "	Loading image map...\n";
				    if (!imgMap.load(input.c_str()))
				    {
						cout << "	Unable to load " << input << ". Sorry, but I need to crash now...\n";
						return 1;
					}
				    else 
				    {
						if (imgMap.getWidth()<mapManager.getImageCols() * mapManager.getTileWidth()
						|| imgMap.getHeight()<mapManager.getImageRows() * mapManager.getTileHeight())
						{
							cout << "WARNING: Map requires more tiles than this image map offers!\n";
						}
						cout << "	Image successfully loaded!\n";
						break;
					}
				}
			}
			if (input=="keys")
			{
				cout << "WASD: Move view\n"
					<< "Left mouse button: Change tile near mouse\n"
					<< "Right mouse button: Use/Select the tile near mouse\n"
					<< "Ctrl + RMB: View tile details\n"
					<< "E: View image map to select tile\n"
					<< "Ctrl + S: Immediately save the map\n"
					<< "Ctrl + L: Save the map as a .maplayer\n"
					<< "1 & 2: Select position\n"
					<< "B: Box fill\n"
					<< "Delete: Mouse will erase tiles if clicked\n"
					<< "[REMOVED] L: Lock/pause editing. Press U to resume\n";
			}
			if (input=="sel")
			{
				cout << "X: ";
				cin >> inputX;
				cout << "Y: ";
				cin >> inputY;
				if (inputX > mapManager.getImageCols() || inputX<0 || inputY>mapManager.getImageRows() || inputY<0) cout << "Error: Tile out of range. Selection aborted...\n";
				else
				{
					currentSelection.x=(unsigned char) inputX;
					currentSelection.y=(unsigned char) inputY;
				}
			}
			if (input=="swap")
			{
				cout << "Tile to swap to selection:\n	X: ";
				cin >> inputX;
				cout << "	Y: ";
				cin >> inputY;
				if (inputX > rows || inputX<0 || inputY>columns || inputY<0) cout << "Error: Tile out of range. Selection aborted...\n";
				else
				{
					cout << "Swapping "<<inputX << " , " << inputY << " to "
					<<(int)currentSelection.x<<" , "<<(int)currentSelection.y<<"...";
					std::vector<tile*> *map=mapManager.getMap();
					for (std::vector<tile*>::iterator i=map->begin(); i!=map->end(); ++i)
					{
						if ((*i)->x==(unsigned int) inputX && (*i)->y==(unsigned int) inputY)
						{
							(*i)->x=currentSelection.x;
							(*i)->y=currentSelection.y;
						}
					}
					cout << "done!\n";
				}
			}
		}
		//////////////Camera movement//////////////////////
		++frames;
		if (in.isPressed(sf::Keyboard::D))
		{
			viewX+=speed;
		}
		if (in.isPressed(sf::Keyboard::S))
		{
			viewY+=speed;
		}
		if (in.isPressed(sf::Keyboard::A))
		{
			viewX-=speed;
		}
		if (in.isPressed(sf::Keyboard::W))
		{
			viewY-=speed;
		}

		////////////Select null tile/////////////////
		if (in.isPressed(sf::Keyboard::Delete) && click.getTime()>=CLICK_PAUSE)
		{
			click.start();
			currentSelection.x=NO_DISPLAY;
			currentSelection.y=NO_DISPLAY;
			cout << "Selected is " << (int)currentSelection.x << " , " << (int)currentSelection.y << "\n";
		}

        ////////////Set tile to current selection////
        if (in.isMousePressed(sf::Mouse::Left)) 
        {	
			selected=mapManager.pickTile((int) viewX/mapManager.getTileWidth(), (int) viewY/mapManager.getTileHeight(), in.getMouseX(), in.getMouseY());
	        if (selected !=NULL)
	        {
				selected->x=currentSelection.x;
				selected->y=currentSelection.y;
			}
		}
		///////////Selection/////////////////////////
		if (in.isMousePressed(sf::Mouse::Right) && click.getTime()>=CLICK_PAUSE)
		{
			click.start();
			selected=mapManager.pickTile((int) viewX/mapManager.getTileWidth(), (int) viewY/mapManager.getTileHeight(), in.getMouseX(), in.getMouseY());
	        if (selected !=NULL)
	        {
				//Debug output text
				std::stringstream ss;
				ss << "Selected is a " << (int)selected->x << " , " << (int)selected->y << " at " << mapManager.pointToMapX((int) viewX/mapManager.getTileWidth(), in.getMouseX()) <<" , "<<mapManager.pointToMapY((int) viewY/mapManager.getTileHeight(), in.getMouseY());
				outputText.setText(ss.str());

				//Information output
				if (in.isPressed(sf::Keyboard::LControl)||in.isPressed(sf::Keyboard::RControl))
				{
					cout << "Tile is a " << (int)selected->x << " , " << (int)selected->y 
					<< " located at "<<mapManager.pointToMapX((int) viewX/mapManager.getTileWidth(), in.getMouseX())<<" , "<<mapManager.pointToMapY((int) viewY/mapManager.getTileHeight(), in.getMouseY())<< "\n";
					//outputText.setText("Selected is " + s
				}
				else
				{
					currentSelection.x=selected->x;
					currentSelection.y=selected->y;
					cout << "Selected is " << (int)currentSelection.x << " , " << (int)currentSelection.y << "\n";
				}
			}
			//The order is important because if the selector is over the
			//map you will still be able to select things
			if (isShowing)
			{
				mouseBound.setPosition(in.getMouseX(), in.getMouseY());
				if (isColliding(&selectBound, &mouseBound))
				{
					float tempX=mouseBound.getX()-selectBound.getX();
					float tempY=mouseBound.getY()-selectBound.getY();
					if (tempX>0 && tempY>0 && tempX<=selectBound.getWidth() && tempY<=selectBound.getHeight())
					{
						tempX=(int) tempX / (int)mapManager.getTileWidth();
						tempY=(int) tempY / (int)mapManager.getTileHeight();
						cout << "Temp X and Y: " << tempX << " , " << tempY << "\n";
						if (tempX > mapManager.getImageCols() || tempX<0 || tempY>mapManager.getImageRows() || tempY<0) 
							cout << "Error: Tile out of range. Selection aborted...\n";
						else
						{
							currentSelection.x=(unsigned int) tempX;
							currentSelection.y=(unsigned int) tempY;
							cout << "Selected is " << (int)currentSelection.x << " , " << (int)currentSelection.y << "\n";
						}
						
					}
				}
			}
		}

		//Set positions for advanced editing
		if (in.isPressed(sf::Keyboard::Num1) && click.getTime()>=CLICK_PAUSE)
		{
			click.start();
			selectionA.x=mapManager.pointToMapX((int) viewX/mapManager.getTileWidth(), in.getMouseX());
			selectionA.y=mapManager.pointToMapY((int) viewY/mapManager.getTileHeight(), in.getMouseY());
			//Max out the selections if needed
			if (selectionA.x>mapManager.getMapCols()) selectionA.x=mapManager.getMapCols();
			if (selectionA.y>mapManager.getMapRows()) selectionA.y=mapManager.getMapRows();
			cout << "Position A is " << (int) selectionA.x << " , " <<(int) selectionA.y << "\n"; 
		}
		if (in.isPressed(sf::Keyboard::Num2) && click.getTime()>=CLICK_PAUSE)
		{
			click.start();
			selectionB.x=mapManager.pointToMapX((int) viewX/mapManager.getTileWidth(), in.getMouseX());
			selectionB.y=mapManager.pointToMapY((int) viewY/mapManager.getTileHeight(), in.getMouseY());
			//Max out if needed
			if (selectionB.x>mapManager.getMapCols()) selectionB.x=mapManager.getMapCols();
			if (selectionB.y>mapManager.getMapRows()) selectionB.y=mapManager.getMapRows();
			cout << "Position B is " << (int) selectionB.x << " , " <<(int) selectionB.y << "\n"; 
		}
		//Box fill
		if (in.isPressed(sf::Keyboard::B) && click.getTime()>=CLICK_PAUSE)
		{
			click.start();
			cout << "Box filling...";
			for (unsigned int i=selectionA.y; i<=selectionB.y; ++i)
			{
				for (unsigned int n=selectionA.x; n<=selectionB.x; ++n)
				{
					selected=mapManager.getTileAt(i, n);
					if (selected !=NULL)
					{
						selected->x=currentSelection.x;
						selected->y=currentSelection.y;
					}
				}
			}
			cout << "done\n";
		}

		//Save the tilemap CTRL S
		if (in.isPressed(sf::Keyboard::S) && (in.isPressed(sf::Keyboard::RControl) || in.isPressed(sf::Keyboard::LControl)) 
		&& click.getTime()>CLICK_PAUSE)
		{
			click.start();
			cout << "Saving map...\n";
		    if (!mapManager.dumpMapToFile(mapFileName.c_str())) cout << "Failed to save map!\n";
		    else cout << "Successfully saved map!\n";
		}

		//Save the tile map as a layer (require CLI input) CTRL L
		if (in.isPressed(sf::Keyboard::L) && (in.isPressed(sf::Keyboard::RControl) || in.isPressed(sf::Keyboard::LControl)) 
		&& click.getTime()>CLICK_PAUSE)
		{
			click.start();
			std::cout << "Dumping map as layer...\n";
			exportAsLayer(mapFileName, mapManager.getMap());
			std::cout << "Done\n";
		}
        //The select tile set up
		if (in.isPressed(sf::Keyboard::E) && click.getTime()>CLICK_PAUSE)
		{
			click.start();
			if (!isShowing)
			{
				isShowing=true;
				//For big maps, putting it at the mouse cursor
				//is annoying. Instead, we'll put it in the corner
				//selectX=in.getMouseX();
				//selectY=in.getMouseY();
				selectX=0;
				selectY=0;
				selectBound.setPosition(selectX, selectY);
			}
			else isShowing=false;
		}
        
        //Locking mechanism
		if (/*in.isPressed(sf::Keyboard::L) || */!win.isFocused())
		{
			cout << "Editor locked. Press Space to command\n";
			while(/*!in.isPressed(sf::Keyboard::U) ||*/ !win.isFocused())
			{
				if (win.shouldClose()) break;
				if (win.isFocused()) break;
				if (in.isPressed(sf::Keyboard::Space)) break;
			}
			click.start();
			cout << "Editor unlocked\n";
		}

		//////////////////////////////////////
		//////////Rendering///////////////////
		//////////////////////////////////////
		mapManager.setViewOffset(-(int)viewX % (int)mapManager.getTileWidth(), -(int) viewY % (int)mapManager.getTileHeight());
		mapManager.drawMap((int) viewX/mapManager.getTileWidth(),(int) viewY/mapManager.getTileHeight(),&win);
		//Draw the selection map
		if (isShowing)
		{
			selectImg->clearSubRect();
			selectImg->setPosition(selectX, selectY);
			win.draw(selectImg);
		}
		//Draw the cursor
		/*if (click.getTime()>CLICK_PAUSE)
		{
			
		}*/
		/*//Nasty. But hey, it works, doesn't it? No, it doesn't
		float cursorX=(in.getMouseX()+(mapManager.getViewOffsetX())-((int)in.getMouseX()%(int)mapManager.getTileWidth()) + (mapManager.getTileWidth()/2));
		float cursorY=(in.getMouseY()+(mapManager.getViewOffsetY()) -((int)in.getMouseY()%(int)mapManager.getTileHeight()) + (mapManager.getTileHeight()/2));
		cursor.setPosition(cursorX, cursorY);
		win.draw(&cursor);*/
		win.draw(&outputText);
        win.update();
    }
    //time=currTime.getTime();
    cout << "Saving map...\n";
    if (!mapManager.dumpMapToFile(mapFileName.c_str())) cout << "Failed to save map\n";
    else cout << "Successfully saved map!\n";
    /*cout << "\n|Total time: " << time << " seconds. \n";
    cout << "|Avg frame time: " << 1/(time/frames) << " fps\n";*/
	return 1;
}
