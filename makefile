all: game.o main.cpp
	g++ game.o main.cpp -o projet
game.o: game.cpp
	g++ -c game.cpp
