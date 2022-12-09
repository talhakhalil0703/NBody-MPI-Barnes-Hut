CC = mpicxx
SRCS = ./src/*.cpp
INC = ./src/
OPTS =  -lglfw3 -lGL -lX11 -lpthread -lXrandr -ldl -lGLEW -std=c++17 -DNDEBUG

EXEC = nbody
EXEC_DEBUG = nbody_debug

all: clean compile test
debug: compile_debug

compile_debug:
	$(CC) $(SRCS) $(OPTS) -g -I$(INC) -o $(EXEC_DEBUG)

compile:
	$(CC) $(SRCS) $(OPTS) -o3 -I$(INC) -o $(EXEC)

clean:
	rm -f $(EXEC)

test:
	./nbody -i input/nb-10.txt -o test.out -s 1000 -t 0.5 -d 0.005
v:
	./nbody -i input/nb-10.txt -o test.out -s 1000 -t 0.5 -d 0.005 -V