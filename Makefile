SDIR = src
ODIR = bin
CC = gcc
CFLAGS = -std=c99
TARGET = zemu

all: $(TARGET)

$(TARGET): $(SDIR)/zemu.c $(SDIR)/loop.c $(SDIR)/utils.c
	$(CC) -o $(ODIR)/$(TARGET) $(CFLAGS) $^

clean:
	$(RM) $(ODIR)/$(TARGET)
