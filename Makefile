CC = g++
SRCS = ./src/*.cpp
INC = ./src/
OPTS = -std=c++17 -Wall -Werror -lpthread

EXEC = bin/nbody
EXEC_DEBUG = bin/nbody_debug

all: clean compile test
debug: compile_debug

compile_debug:
	$(CC) $(SRCS) $(OPTS) -g -I$(INC) -o $(EXEC_DEBUG)

compile:
	$(CC) $(SRCS) -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW $(OPTS) -O3 -I$(INC) -o $(EXEC)

clean:
	rm -f $(EXEC)

test:
	./bin/nbody -i input/nb-10.txt -o test.out -s 10 -t 0.05 -d 123 -V