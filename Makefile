build/mosh : mosh/* build
	g++ -o build/mosh mosh/*

build :
	mkdir build
