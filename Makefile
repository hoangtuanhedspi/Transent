CC := gcc
SRCDIR := src
BUILDDIR := build
BINDIR := bin
SHAREDDIR := bin/lib
TARGET := transent
LIBS := ./bin/lib/libtransent.a
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -w
LIB := -L./bin/lib -ltransent
INC := -I./include

all: lib client server

$(TARGET): transent.c
	$(CC) $^ -o $(TARGET) $(LIB) $(INC)

$(LIBS): $(OBJECTS)
	if [ ! -d "$(SHAREDDIR)" ];then mkdir -p $(SHAREDDIR); fi;
	ar rcs $@ $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	if [ ! -d "$(BUILDDIR)" ];then mkdir $(BUILDDIR); fi
	$(CC) $(CFLAGS) -c -o $@ $< $(INC)

test:
	$(CC) $(CFLAGS) tests/test.c $(INC) $(LIB) -o test

lib: $(LIBS)

client: client.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) $(INC)

server: server.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) $(INC)

clean:
	$(RM) -r $(BUILDDIR) $(BINDIR) test *.o client server
	
.PHONY: clean
