
# not sure what the args after ':' do...

compile: main.cpp cameraController.cpp block.cpp blockType.cpp chunk.cpp world.cpp PerlinNoise.cpp PerlinNoiseUtil.cpp atlas.cpp
	mkdir -p build
	g++ -o build/main main.cpp cameraController.cpp block.cpp blockType.cpp chunk.cpp world.cpp PerlinNoise.cpp PerlinNoiseUtil.cpp atlas.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -Ofast -std=c++17

compileWithProfile: main.cpp cameraController.cpp block.cpp blockType.cpp chunk.cpp world.cpp PerlinNoise.cpp PerlinNoiseUtil.cpp atlas.cpp
	mkdir -p build
	g++ -o build/mainWithProfile main.cpp cameraController.cpp block.cpp blockType.cpp chunk.cpp world.cpp PerlinNoise.cpp PerlinNoiseUtil.cpp atlas.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -Ofast -std=c++17 -pg


run:
	./build/main

runWithProfile:
	./build/mainWithProfile


clean:
	rm -v build/*
