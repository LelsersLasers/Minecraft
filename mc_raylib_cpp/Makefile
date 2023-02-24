buildAndRun: compile run

CC = g++
RAYLIB_FLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
EXTRA_FLAGS = -Wall -Ofast -std=c++17

OUT_DIR = build

compile:
	mkdir -vp $(OUT_DIR)
	$(CC) -o $(OUT_DIR)/main src/*.cpp $(RAYLIB_FLAGS) $(EXTRA_FLAGS)
	

compileWithProfile:
	mkdir -vp $(OUT_DIR)
	$(CC) -o $(OUT_DIR)/mainWithProfile src/*.cpp $(RAYLIB_FLAGS) $(EXTRA_FLAGS) -pg


run:
	./$(OUT_DIR)/main

runWithProfile:
	./$(OUT_DIR)/mainWithProfile


clean:
	rm -vr $(OUT_DIR)
