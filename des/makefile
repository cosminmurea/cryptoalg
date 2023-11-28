SOURCE = des.c
TARGET = des.out
DEPS = ../utils.c
CC = gcc
CFLAGS = -g -Wall

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(SOURCE) $(DEPS)
	$(CC) $(CFLAGS) $(SOURCE) $(DEPS) -o $(TARGET)

.PHONY: clean

clean:
	rm $(TARGET)