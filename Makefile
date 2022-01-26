CC=clang++
CFLAGS=-Wall -Wextra -std=c++20 -g -fPIC -Ilib
LDFLAGS=-lncurses

SRCS=\
src/Card.cpp \
src/Creature.cpp \
src/Interface.cpp \
src/Player.cpp \
src/Duel.cpp \
src/Game.cpp \
src/Land.cpp \
src/CardRegistry.cpp

SERVER_SRCS = \
src/Card.cpp \
src/Creature.cpp \
src/GameServer.cpp \
src/Duel.cpp \
src/Player.cpp \
src/Interface.cpp \
src/UserInterface.cpp \
src/Land.cpp  \
src/CardRegistry.cpp \
src/Game.cpp

CLIENT_SRCS = \
src/Card.cpp \
src/Creature.cpp \
src/UserInterface.cpp \
src/Client.cpp \
src/Land.cpp \
src/CardRegistry.cpp

OBJS=$(subst .cpp,.o,$(subst src/,bin/,$(SRCS)))

SERVER_OBJS=$(subst .cpp,.o,$(subst src/,bin/,$(SERVER_SRCS)))

CLIENT_OBJS=$(subst .cpp,.o,$(subst src/,bin/,$(CLIENT_SRCS)))

.PHONY: clean clean_all

all: bin/magic

bin/%.o: src/%.cpp src/%.hpp
	$(CC) $(CFLAGS) -c $< -o $(subst .cpp,.o,$(subst src/,bin/,$<))

bin/magic: $(OBJS) src/main.cpp
	g++ $(CFLAGS) $(OBJS) src/main.cpp -o bin/magic $(LDFLAGS)

server: $(SERVER_OBJS) src/server_main.cpp
	$(CC) $(CFLAGS) $(SERVER_OBJS) src/server_main.cpp -o bin/server $(LDFLAGS)

client: $(CLIENT_OBJS) src/client_main.cpp
	$(CC) $(CFLAGS) $(CLIENT_OBJS) src/client_main.cpp -o bin/client $(LDFLAGS)

clean:
	rm -f bin/*.o
	rm -f bin/magic

clean_all: clean
	rm -f bin/server
	rm -f bin/client
