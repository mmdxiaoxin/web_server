CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm

SRCDIR = src
OBJDIR = obj
BINDIR = bin
WEBDIR = web_root

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
EXECUTABLE = $(BINDIR)/web_server

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR)/*.o $(EXECUTABLE) $(BINDIR)/$(WEBDIR) $(BINDIR)/config.txt

run:
	$(EXECUTABLE)

web:
	cp -r $(WEBDIR) config.txt $(BINDIR)/

.PHONY: all clean create_symlinks run web
