СС = clang
CFLAGS = -Wall -Wextra -g
TARGET = client

OBJS = main.o bencode.o free_mem.o printer.o

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	$(MAKE) clean_files

clean_files:
	rm -f $(OBJS)

main.o: main.c bencode.h
	$(CC) $(CFLAGS) -c main.c -o main.o

bencode.o: bencode.c bencode.h
	$(CC) $(CFLAGS) -c bencode.c -o bencode.o

printer.o: printer.c bencode.h
	$(CC) $(CFLAGS) -c printer.c -o printer.o

free_mem.o: free_mem.c bencode.h
	$(CC) $(CFLAGS) -c free_mem.c -o free_mem.o

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean clean_files
