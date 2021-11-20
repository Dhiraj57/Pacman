all: compile link 

compile:
	g++ -I src/include -c main.cpp -c src/Pacman.cpp -c src/Ghost.cpp -c src/GhostManager.cpp -c src/DrawText.cpp -c src/DrawMap.cpp -c src/ConvertSketch.cpp -c src/MapCollision.cpp
link:
	g++ main.o Pacman.o Ghost.o GhostManager.o DrawText.o DrawMap.o ConvertSketch.o MapCollision.o -o Pacman -L src/lib -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio 