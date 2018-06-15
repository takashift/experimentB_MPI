CC=mpicc
CFLAGS=-Wall 
TARGETS=hello_world

all: $(TARGETS)

hello_world: hello_world.c
	$(CC) hello_world.c -o hello_world

clean:
	$(RM) $(TARGETS)
