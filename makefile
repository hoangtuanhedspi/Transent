CC := gcc
SRCDIR := src
BUILDDIR := build
BINDIR := bin
SHAREDDIR := bin/lib
TARGET := transent
LIBS := libtransent.so
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -w#-g
LIB := -L./bin/lib -ltransent
INC := -I./include

all: $(LIBS) client server

$(TARGET): transent.o
	$(CC) $^ -o $(TARGET) $(LIB) $(INC)

$(LIBS): $(OBJECTS)
	if [ ! -d "$(SHAREDDIR)" ];then mkdir -p $(SHAREDDIR); fi
	$(CC) -shared $^ -o bin/lib/$(LIBS) $(INC)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	if [ ! -d "$(BUILDDIR)" ];then mkdir $(BUILDDIR); fi
	$(CC) $(CFLAGS) -c -o $@ $< $(INC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(INC)

test:
	$(CC) $(CFLAGS) tests/test.c $(INC) $(LIB) -o test

client: client.o
	$(CC) $^ -o $@ $(LIB) $(INC)

server: server.o
	$(CC) $^ -o $@ $(LIB) $(INC)

clean:
	$(RM) -r $(BUILDDIR) $(BINDIR) test *.o
	
.PHONY: clean
