CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -O3 -pipe

SRCS = init.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = init
INSTALL_DIR = /usr/local/bin

.PHONY: all clean install

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

install: $(EXEC)
	cp $(EXEC) $(INSTALL_DIR)

clean:
	rm -f $(OBJS) $(EXEC)

