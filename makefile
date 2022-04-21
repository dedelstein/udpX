EXECUTABLES = fancy_client udp_client udp_server
CC = gcc

OBJS = \
	udp_server.o \
	udp_client.o \
	fancy_client.o

CFLAGS = -Wall -g -std=c99

REBUILDABLES = $(OBJS) $(EXECUTABLES)

all: $(OBJS) $(EXECUTABLES)

udp_server:
	$(CC) -o $@ udp_server.o $(CFLAGS)

udp_client:
	$(CC) -o $@ udp_client.o $(CFLAGS)

fancy_client:
	$(CC) -o $@ fancy_client.o $(CFLAGS)

%.o: %.c
	$(CC) -o $@ -c $<

clean:
	rm -f $(REBUILDABLES)
