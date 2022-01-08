CC=clang++
CFLAGS=-Wall -Wextra -g
LDFLAGS=

SRCS=Card.cpp
OBJS=$(subst .cpp, .o, $(SRCS))

.PHONY: clean

all: bin/magic

bin/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $(subst "/src", "/bin", $<)

bin/magic: bin/Card.o
	g++ src/main.cpp -o bin/magic

clean:
	rm -f bin/*.o
	rm -f bin/magic