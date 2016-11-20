SDIR = src
ODIR = bin
CC = gcc
CFLAGS = -std=c99
TARGET = zemu

all: $(TARGET)

$(TARGET): $(SDIR)/*.c
	$(CC) -o $(ODIR)/$(TARGET) $(CFLAGS) $^

clean:
	$(RM) $(ODIR)/$(TARGET)
