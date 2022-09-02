#CC = gcc
CFLAGS = -Wall -luci
OBJS = hello.o

all: hello

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

hello: $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -f hello *.o
