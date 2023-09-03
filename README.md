# Game-of-life
Game of life in sfml

To compile this code on linux (Ubuntu) open terminal and type these comands:
```
sudo apt update
sudo apt install g++ libsfml-dev

cd [path to Game-of-life-main]

g++ -o GameOfLife main.cpp -lsfml-graphics -lsfml-window -lsfml-system
./GameOfLife
```
With CMake:
```
sudo apt update
sudo apt install g++ libsfml-dev

cd [path to Game-of-life-main]

mkdir build
cd build

cmake ..
make
bin/GameOfLife 
```
