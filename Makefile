CPP_FLAGS=-O2 -Wall -Werror -std=c++11

all: bin/main

bin/main: bin/utils.o bin/Tree.o bin/Buffer.o bin/BitReader.o bin/BitWriter.o bin/main.o
	clang++ -o bin/main $(CPP_FLAGS) bin/Tree.o bin/Buffer.o bin/BitReader.o bin/BitWriter.o bin/utils.o bin/main.o

bin/main.o: src/main.cpp src/Tree.h src/Buffer.h src/BitReader.h src/BitWriter.h src/utils.h
	clang++ -o bin/main.o -c $(CPP_FLAGS) src/main.cpp

bin/Tree.o: src/Tree.cpp src/Tree.h src/utils.h
	clang++ -o bin/Tree.o -c $(CPP_FLAGS) src/Tree.cpp

bin/Buffer.o: src/Buffer.cpp src/Buffer.h src/utils.h
	clang++ -o bin/Buffer.o -c $(CPP_FLAGS) src/Buffer.cpp

bin/BitReader.o: src/BitReader.cpp src/BitReader.h src/utils.h src/Buffer.h
	clang++ -o bin/BitReader.o -c $(CPP_FLAGS) src/BitReader.cpp

bin/BitWriter.o: src/BitWriter.cpp src/BitWriter.h src/utils.h src/Buffer.h
	clang++ -o bin/BitWriter.o -c $(CPP_FLAGS) src/BitWriter.cpp

bin/utils.o: src/utils.cpp src/utils.h
	clang++ -o bin/utils.o -c $(CPP_FLAGS) src/utils.cpp

.PHONY: clean

clean:
	rm -rf bin/*.o bin/main
