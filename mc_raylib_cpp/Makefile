
# not sure what the args after ':' do
compile: main.cpp cameraController.cpp
	mkdir -p build
	g++ -o build/main main.cpp cameraController.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

run:
	./build/main

clean:
	rm -v build/*

