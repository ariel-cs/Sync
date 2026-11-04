CC = gcc
CFLAGS = -Wall -Wextra -g -std=gnu11 -pthread
TARGET = sync_car
OBJS = sync_car.o sync_car-runner.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

sync_car.o: sync_car.c sync_car.h
	$(CC) $(CFLAGS) -c $< -o $@

sync_car-runner.o: sync_car-runner.c sync_car.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
