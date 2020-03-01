#!/bin/sh

# SFML
echo "Building SFML..."
echo "WARNING: This will run git clean -f on SFML"
cd dependencies/SFML
make clean
git clean -f
cmake .
make -j4

cd ../../

#  Imgui-sfml
# I shouldn't need to do this, but I was having trouble getting IMGUI_USER_CONFIG to work
# Instead of using the define, append the file in
echo "Copying imgui-sfml config to imgui imconfig..."
cd dependencies/imgui
git checkout HEAD -- imconfig.h
cat ../imgui-sfml/imconfig-SFML.h >> imconfig.h
