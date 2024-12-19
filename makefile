all:
	g++ -I libs/ -L libs/SDL2 -o build/main src/main.cpp -lmingw32 -lSDL2main -lSDL2
	./build/main
