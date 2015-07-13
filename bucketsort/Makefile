CC    = g++ --std=c++0x
FLAGS = -O3 -Wall -Wno-unused-result

SRC   = bucketsort.cpp main.cpp
EXEC  = bin/bucketsort

all: make_dirs $(EXEC)

clean:
	rm -f -r bin

make_dirs:
	mkdir -p bin

$(EXEC): $(SRC)
	$(CC) $(FLAGS) $^ -o $@ -pthread

run:
	./$(EXEC) input/medium.in input/medium.out

