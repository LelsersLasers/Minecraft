
# not sure what the args after ':' do...
compile: main.cpp cameraController.cpp block.cpp blockType.cpp chunk.cpp world.cpp ColorUtil.cpp PerlinNoise.cpp PerlinNoiseUtil.cpp
	mkdir -p build
	g++ -o build/main main.cpp cameraController.cpp block.cpp blockType.cpp chunk.cpp world.cpp ColorUtil.cpp PerlinNoise.cpp PerlinNoiseUtil.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -O3 -std=c++17

run:
	./build/main

clean:
	rm -v build/*
