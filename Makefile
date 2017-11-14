CC := gcc
SRCDIR := src
BUILDDIR := build
BINDIR := bin
SHAREDDIR := bin/lib
TARGET := transent
LIBS := ./bin/lib/libtransent.a #libtransent.so
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -w#-g
LIB := -L./bin/lib -ltransent
INC := -I./include

all: lib client server

$(TARGET): transent.c
	$(CC) $^ -o $(TARGET) $(LIB) $(INC)

$(LIBS): $(OBJECTS)
	if [ ! -d "$(SHAREDDIR)" ];then mkdir -p $(SHAREDDIR); fi;
	ar rcs $@ $(OBJECTS)
	#$(CC) -shared -fpic $^ -o bin/lib/$(LIBS) $(INC)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	if [ ! -d "$(BUILDDIR)" ];then mkdir $(BUILDDIR); fi
	$(CC) $(CFLAGS) -c -o $@ $< $(INC)

#%.o: %.c
	#$(CC) $(CFLAGS) -c -o $@ $< $(INC)

test:
	$(CC) $(CFLAGS) tests/test.c $(INC) $(LIBS) -o test

lib: $(LIBS)

client: client.c
	$(CC) $^ -o $@ $(LIBS) $(INC)

server: server.c
	$(CC) $^ -o $@ $(LIBS) $(INC)

clean:
	$(RM) -r $(BUILDDIR) $(BINDIR) test *.o
	
.PHONY: clean
