build/mosh : mosh/* build
	g++ -std=c++17 -lstdc++fs -o build/mosh mosh/*

build :
	mkdir build
