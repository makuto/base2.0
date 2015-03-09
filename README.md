# base2.0
A simple game library and abstraction layer for SFML 2.2

------------------
Base
------------------

Setup
-----
You need SFML 2.0 installed if you have base2.0, or SFML 1.6 if you have base.

It's then really easy from there; just 
make
make tools
sudo make install
sudo make installTools

If you want to clean up:
sudo make clean
sudo make cleanTools

Installing the library will result in the following:
	/usr/include/base2.0 will contain all of the header files. To use them:
	#include <base2.0/module/module.hpp>

	/usr/lib will contain libbase2.0.a or libbase.a . Use these when linking like so:
	g++ -o "myProj" myProj.o -lbase2.0 [or -lbase] -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

	The Tile Map Editor (tileEditor) binary will be added to /usr/bin
	"tle.desktop" will be added to /usr/share/applications, which adds the Tile Map Editor to the Ubuntu Dash

Cleaning will undo all of these changes.
 
Basic Use (installed)
------
#include <base[or base2.0]/module/module.hpp>

To link:
g++ -o "myProj" myProj.o -lbase [or -lbase2.0] -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

Basic Use (not installed)
------
#include "/path/to/base/module/module.hpp"

To link:
g++ -o "myProj" myProj.o "/path/to/base/lib/base.a" -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

About
------
Base/Base2.0 are my game libraries and SFML abstraction layers. I've built it with the goal of a simple interface and maximum multimedia library independence. You are free to use Base in any way you want (but don't sell it).

Questions? Email me: macoymadson@gmail.com

Check out my website: http://augames.f11.us/
