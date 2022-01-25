CC=clang++
CFLAGS=-Wall -Wextra -std=c++20 -g -fPIC
LDFLAGS=-lncurses

SRCS=\
src/Card.cpp \
src/Creature.cpp \
src/Interface.cpp \
src/Player.cpp \
src/Duel.cpp \
src/Game.cpp

SERVER_SRCS = \
src/Card.cpp \
src/Creature.cpp \
src/GameServer.cpp \
src/Duel.cpp \
src/Player.cpp \
src/Interface.cpp \
src/UserInterface.cpp

OBJS=$(subst .cpp,.o,$(subst src/,bin/,$(SRCS)))

SERVER_OBJS=$(subst .cpp,.o,$(subst src/,bin/,$(SERVER_SRCS)))

.PHONY: clean

all: bin/magic

bin/%.o: src/%.cpp src/%.hpp
	$(CC) $(CFLAGS) -c $< -o $(subst .cpp,.o,$(subst src/,bin/,$<))

bin/magic: $(OBJS) src/main.cpp
	g++ $(CFLAGS) $(OBJS) src/main.cpp -o bin/magic $(LDFLAGS)

server: $(SERVER_OBJS) src/server_main.cpp
	$(CC) $(CFLAGS) $(SERVER_OBJS) src/server_main.cpp -o bin/server $(LDFLAGS) 

server_test: $(SERVER_OBJS) src/server_test.cpp
	$(CC) $(CFLAGS) $(SERVER_OBJS) src/server_test.cpp -o bin/server_test $(LDFLAGS) 

clean:
	rm -f bin/*.o
	rm -f bin/magic
	rm -f bin/server bin/server_test
