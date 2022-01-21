CC=clang++
CFLAGS=-Wall -Wextra -std=c++20 -g
LDFLAGS=-lncurses

SRCS=\
src/Card.cpp \
src/Creature.cpp \
src/Interface.cpp \
src/Player.cpp \
src/Game.cpp

OBJS=$(subst .cpp,.o,$(subst src/,bin/,$(SRCS)))

.PHONY: clean rapport

all: bin/magic

bin/%.o: src/%.cpp src/%.hpp
	$(CC) $(CFLAGS) -c $< -o $(subst .cpp,.o,$(subst src/,bin/,$<))

bin/magic: $(OBJS) src/main.cpp
	g++ $(CFLAGS) $(OBJS) src/main.cpp -o bin/magic $(LDFLAGS)

docs/rapport/rapport.pdf: docs/rapport/rapport.md
	cd docs/rapport && pandoc -V papersize:a4 -V geometry:margin=2cm rapport.md -o rapport.pdf
	
rapport: docs/rapport/rapport.pdf

clean:
	rm -f bin/*.o
	rm -f bin/magic