# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -Isrc/include

# Linker flags
LDFLAGS = -Lsrc/lib -lmingw32 -lSDL2main -lSDL2

# Targets
TARGET = snake

# Object files
OBJS = src/linkedlist.o src/queue.o src/snake.o

# Default rule to create the executable
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to create object files
src/%.o: src/%.c src/include/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)