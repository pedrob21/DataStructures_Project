ifeq ($(OS),Windows_NT)
    RM = cmd /C "del /Q"
    EXEC = manga_manager.exe
else
    RM = rm -f
    EXEC = manga_manager
endif

CC = gcc
CFLAGS = -Wall -g
OBJS = main.o manga.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

main.o: main.c manga.h
	$(CC) $(CFLAGS) -c main.c

manga.o: manga.c manga.h
	$(CC) $(CFLAGS) -c manga.c

clean:
	@if [ -f main.o ]; then rm main.o; fi
	@if [ -f manga.o ]; then rm manga.o; fi
	@if [ -f $(EXEC) ]; then rm $(EXEC); fi

run: $(EXEC)
	./$(EXEC)
