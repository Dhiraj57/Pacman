all: compile link 

compile:
	g++ -I src/include -c main.cpp -c src/Pacman.cpp -c src/Ghost.cpp -c src/GhostManager.cpp -c src/DrawText.cpp -c src/DrawMap.cpp -c src/ConvertSketch.cpp -c src/MapCollision.cpp
link:
	g++ main.o -o main -L src/lib -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio 