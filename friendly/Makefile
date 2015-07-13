CC    = g++
FLAGS = -O3 -Wall -Wno-unused-result

SRC   = friendly.cpp
EXEC  = bin/friendly

all: make_dirs $(EXEC)

clean:
	rm -f -r bin

make_dirs:
	mkdir -p bin

$(EXEC): $(SRC)
	$(CC) $(FLAGS) $^ -o $@ -pthread

run:
	./$(EXEC) < input/small.in

