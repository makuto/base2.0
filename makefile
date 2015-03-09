FLAGS=g++ -c -Os -Wall
ARCHIVE_NAME=lib/base.a

all: eptParser.o math.o collision.o timer.o profiler.o graphics.o input.o tileMap.o multilayerMap.o tileCamera.o animation.o sound.o
	@echo -------------------Building archive...
	ar rvs $(ARCHIVE_NAME) eptParser.o math.o collision.o graphics.o input.o tileMap.o multilayerMap.o tileCamera.o animation.o timer.o profiler.o sound.o
eptParser.o: ept/eptParser.hpp ept/eptParser.cpp
	$(FLAGS) "ept/eptParser.hpp"
	$(FLAGS) "ept/eptParser.cpp"
math.o: math/math.hpp math/math.cpp
	$(FLAGS) "math/math.hpp"
	$(FLAGS) "math/math.cpp"
collision.o: collision/collision.hpp collision/collision.cpp
	$(FLAGS) "collision/collision.hpp"
	$(FLAGS) "collision/collision.cpp"
timer.o: timer/timer.hpp timer/timer.cpp
	$(FLAGS) "timer/timer.hpp"
	$(FLAGS) "timer/timer.cpp"
profiler.o: profiler/profiler.hpp profiler/profiler.cpp
	$(FLAGS) "profiler/profiler.hpp"
	$(FLAGS) "profiler/profiler.cpp"
graphics.o: graphics/graphics.hpp graphics/graphics.cpp
	$(FLAGS) "graphics/graphics.hpp"
	$(FLAGS) "graphics/graphics.cpp"
input.o: input/input.hpp input/input.cpp
	$(FLAGS) "input/input.hpp"
	$(FLAGS) "input/input.cpp"
tileMap.o: tileMap/tileMap.hpp tileMap/tileMap.cpp
	$(FLAGS) "tileMap/tileMap.hpp"
	$(FLAGS) "tileMap/tileMap.cpp"
multilayerMap.o: tileMap/multilayerMap.hpp tileMap/multilayerMap.cpp
	$(FLAGS) "tileMap/multilayerMap.hpp"
	$(FLAGS) "tileMap/multilayerMap.cpp"
tileCamera.o: tileMap/tileCamera.hpp tileMap/tileCamera.cpp
	$(FLAGS) "tileMap/tileCamera.hpp"
	$(FLAGS) "tileMap/tileCamera.cpp"
animation.o: animation/animation.hpp animation/animation.cpp
	$(FLAGS) "animation/animation.hpp"
	$(FLAGS) "animation/animation.cpp"
sound.o: sound/sound.hpp sound/sound.cpp
	$(FLAGS) "sound/sound.hpp"
	$(FLAGS) "sound/sound.cpp"
tools: tileEditor.o multilayerMapEditor.o tiledImporter.o
	@echo -------------------Building tileEditor...
	g++ -Wall -o "tools/tileEditor" "tileEditor.o" $(ARCHIVE_NAME) -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system
	@echo -------------------Building multilayerMapEditor...
	g++ -Wall -o "tools/multilayerMapEditor" "multilayerMapEditor.o" $(ARCHIVE_NAME) -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system
	@echo -------------------Building tiledImporter...
	g++ -Wall -o "tools/tiledImporter" "pugixml.o" "tiledImporter.o" $(ARCHIVE_NAME) -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system
tileEditor.o: tileMap/tileEditor.cpp
	$(FLAGS) "tileMap/tileEditor.cpp"
multilayerMapEditor.o:
	$(FLAGS) "tileMap/multilayerMapEditor.cpp"
tiledImporter.o: tiledImporter/tiledImporter.cpp pugixml.o
	$(FLAGS) "tiledImporter/tiledImporter.cpp"
pugixml.o: tiledImporter/pugixml/pugiconfig.hpp tiledImporter/pugixml/pugixml.hpp tiledImporter/pugixml/pugixml.cpp
	$(FLAGS) "tiledImporter/pugixml/pugiconfig.hpp"
	$(FLAGS) "tiledImporter/pugixml/pugixml.hpp"
	$(FLAGS) "tiledImporter/pugixml/pugixml.cpp"
clean:
	rm *.o
	rm $(ARCHIVE_NAME)
	#rm *.gch
	rm graphics/*.gch
	rm input/*.gch
	rm timer/*.gch
	rm profiler/*.gch
	rm collision/*.gch
	rm tileMap/*.gch
	rm sound/*.gch
	rm ept/*.gch
	rm animation/*.gch
	@echo -------------------If base is installed, removing it... [root priveleges]
	rm /usr/lib/libbase2.0.a
	rm -r /usr/include/base2.0
cleanTools:
	rm multilayerMapEditor.o
	rm tileEditor.o
	@echo -------------------Removing tools [error if not created with make tools]
	rm tools/tileEditor
	rm /usr/bin/tileEditor
	rm tools/multilayerMapEditor
	rm /usr/bin/multilayerMapEditor
	rm tools/tiledImporter
	rm /usr/bin/tiledImporter
install:
	@echo -------------------Installing /lib/base.a to /usr/lib/libbase2.0.a...
	cp lib/base.a /usr/lib/libbase2.0.a
	@echo -------------------Preparing /usr/include/base2.0...
	mkdir /usr/include/base2.0
	mkdir /usr/include/base2.0/ept 
	mkdir /usr/include/base2.0/math
	mkdir /usr/include/base2.0/collision
	mkdir /usr/include/base2.0/timer
	mkdir /usr/include/base2.0/profiler
	mkdir /usr/include/base2.0/graphics
	mkdir /usr/include/base2.0/input
	mkdir /usr/include/base2.0/tileMap
	mkdir /usr/include/base2.0/sound
	mkdir /usr/include/base2.0/animation
	@echo -------------------Installing header files to /usr/include/base2.0...
	cp math/math.hpp /usr/include/base2.0/math/math.hpp
	cp ept/eptParser.hpp /usr/include/base2.0/ept/eptParser.hpp
	cp collision/collision.hpp /usr/include/base2.0/collision/collision.hpp
	cp timer/timer.hpp /usr/include/base2.0/timer/timer.hpp
	cp profiler/profiler.hpp /usr/include/base2.0/profiler/profiler.hpp
	cp graphics/graphics.hpp /usr/include/base2.0/graphics/graphics.hpp
	cp input/input.hpp /usr/include/base2.0/input/input.hpp
	cp tileMap/tileMap.hpp /usr/include/base2.0/tileMap/tileMap.hpp
	cp tileMap/multilayerMap.hpp /usr/include/base2.0/tileMap/multilayerMap.hpp
	cp tileMap/tileCamera.hpp /usr/include/base2.0/tileMap/tileCamera.hpp
	cp sound/sound.hpp /usr/include/base2.0/sound/sound.hpp
	cp animation/animation.hpp /usr/include/base2.0/animation/animation.hpp
update:
	@echo -------------------Installing /lib/base.a to /usr/lib/libbase2.0.a...
	cp lib/base.a /usr/lib/libbase2.0.a
	@echo -------------------Installing header files to /usr/include/base2.0...
	cp math/math.hpp /usr/include/base2.0/math/math.hpp
	cp ept/eptParser.hpp /usr/include/base2.0/ept/eptParser.hpp
	cp collision/collision.hpp /usr/include/base2.0/collision/collision.hpp
	cp timer/timer.hpp /usr/include/base2.0/timer/timer.hpp
	cp profiler/profiler.hpp /usr/include/base2.0/profiler/profiler.hpp
	cp graphics/graphics.hpp /usr/include/base2.0/graphics/graphics.hpp
	cp input/input.hpp /usr/include/base2.0/input/input.hpp
	cp tileMap/tileMap.hpp /usr/include/base2.0/tileMap/tileMap.hpp
	cp tileMap/multilayerMap.hpp /usr/include/base2.0/tileMap/multilayerMap.hpp
	cp tileMap/tileCamera.hpp /usr/include/base2.0/tileMap/tileCamera.hpp
	cp sound/sound.hpp /usr/include/base2.0/sound/sound.hpp
	cp animation/animation.hpp /usr/include/base2.0/animation/animation.hpp
installTools:
	@echo -------------------Installing Tile Map Editor [/usr/bin] and .desktop shortcut [Ubuntu only]
	cp tools/tileEditor /usr/bin/tileEditor
	cp tools/tle.desktop /usr/share/applications/tle.desktop
	cp tools/multilayerMapEditor /usr/bin/multilayerMapEditor
	cp tools/mlme.desktop /usr/share/applications/mlme.desktop
	cp tools/tiledImporter /usr/bin/tiledImporter
