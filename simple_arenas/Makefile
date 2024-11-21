# Define the source files
SOURCES = arena.c
OBJECTS = $(SOURCES:.c=.o)

# Define the compiler flags
CFLAGS = -Wall -g3

# The default target is to build the program
all: build

# Build the program
build:
	gcc $(CFLAGS) $(SOURCES) -o target
	chmod +x target

# Run the test suite on the built program
test: build
	./target
