CC=clang++
CFLAGS=-Wall -Wextra -g
LDFLAGS=

SRCS=src/Card.cpp
OBJS=$(subst .cpp,.o,$(subst src/,bin/,$(SRCS)))

.PHONY: clean

all: bin/magic

bin/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $(subst .cpp,.o,$(subst src/,bin/,$<))

bin/magic: $(OBJS)
	g++ $(OBJS) src/main.cpp -o bin/magic

clean:
	rm -f bin/*.o
	rm -f bin/magic