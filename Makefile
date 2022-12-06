CC = mpicxx
SRCS = ./src/*.cpp
INC = ./src/
OPTS =  -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW -std=c++17 -Wall -lpthread

EXEC = bin/nbody
EXEC_DEBUG = bin/nbody_debug

all: clean compile test
debug: compile_debug

compile_debug:
	$(CC) $(SRCS) $(OPTS) -g -I$(INC) -o $(EXEC_DEBUG)

compile:
	$(CC) $(SRCS) $(OPTS) -O3 -I$(INC) -o $(EXEC)

clean:
	rm -f $(EXEC)

test:
	mpirun -np 12 ./bin/nbody -i input/nb-10.txt -o test.out -s 1000 -t 0.5 -d 0.005