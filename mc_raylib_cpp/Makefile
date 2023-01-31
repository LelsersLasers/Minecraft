
# not sure what the args after ':' do
compile: main.cpp cameraController.cpp Vector3Util.cpp Vector2Util.cpp block.cpp blockType.cpp chunk.cpp world.cpp ColorUtil.cpp noise1234.c
	mkdir -p build
	g++ -o build/main main.cpp cameraController.cpp Vector3Util.cpp Vector2Util.cpp block.cpp blockType.cpp chunk.cpp world.cpp ColorUtil.cpp noise1234.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall -std=c++17

run:
	./build/main

clean:
	rm -v build/*

