CC = g++
FLAGS = -std=c++0x
EXEC=a.out
all:
	$(CC) $(FLAGS) main.cpp -o $(EXEC)
clean:
	rm -f a.out
