CC := gcc
SRCDIR := src
BUILDDIR := build
BINDIR := bin
SHAREDDIR := bin/lib
SHAREDINC := bin/include/transent
TARGET := transent
LIBS := ./bin/lib/libtransent.a
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -w
LIB := -L./bin/lib -ltransent -lpthread -lrt
INC := -I./bin/include

all: lib client server

$(TARGET): transent.c
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LIB) $(INC)

$(LIBS): checkdir $(OBJECTS)
	ar rcs $@ $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CFLAGS) -c -o $@ $< $(INC)

test: lib
	$(CC) $(CFLAGS) tests/test.c $(LIB) $(INC) -o test

sys: lib
	$(CC) $(CFLAGS) tests/system.c $(LIB) $(INC) -o stest

checkdir:
	if [ ! -d "$(BUILDDIR)" ];then mkdir $(BUILDDIR); fi
	if [ ! -d "$(SHAREDDIR)" ];then mkdir -p $(SHAREDDIR); fi;
	if [ ! -d "$(SHAREDINC)" ];then mkdir -p $(SHAREDINC) && cp -R include/*.h $(SHAREDINC); fi;

lib: $(LIBS)

client: client.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) $(INC)

server: server.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) $(INC)

clean:
	$(RM) -r $(BUILDDIR) $(BINDIR) test stest *.o client server
	
.PHONY: clean