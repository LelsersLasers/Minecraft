
# not sure what the args after ':' do
compile: main.cpp cameraController.cpp Vector3Util.cpp Vector2Util.cpp block.cpp blockType.cpp
	mkdir -p build
	g++ -o build/main main.cpp cameraController.cpp Vector3Util.cpp Vector2Util.cpp block.cpp blockType.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Wall

run:
	./build/main

clean:
	rm -v build/*

