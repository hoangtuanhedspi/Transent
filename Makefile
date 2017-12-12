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
<<<<<<< HEAD
CFLAGS :=  
=======
CFLAGS := -w --std=c99
>>>>>>> edb6f8ebb9df585da1ee83175a2f14fbcc0a5b5d
LIB := -L./bin/lib -ltransent
INC := -I./bin/include

all: lib client server

$(TARGET): transent.c
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LIB) $(INC)

$(LIBS): $(OBJECTS)
	if [ ! -d "$(SHAREDDIR)" ];then mkdir -p $(SHAREDDIR); fi;
<<<<<<< HEAD
	if [ ! -d "$(SHAREDINC)" ];then mkdir -p $(SHAREDINC); fi;
	cp -R include/ $(SHAREDINC)
=======
	if [ ! -d "$(SHAREDINC)" ];then mkdir -p $(SHAREDINC) && cp -R include/*.h $(SHAREDINC); fi;
>>>>>>> edb6f8ebb9df585da1ee83175a2f14fbcc0a5b5d
	ar rcs $@ $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	if [ ! -d "$(BUILDDIR)" ];then mkdir $(BUILDDIR); fi
	$(CC) $(CFLAGS) -c -o $@ $< $(INC)

test: lib
	$(CC) $(CFLAGS) tests/test.c $(LIB) $(INC) -o test

lib: $(LIBS)

client: client.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) $(INC)

server: server.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) $(INC)

clean:
	$(RM) -r $(BUILDDIR) $(BINDIR) test *.o client server
	
.PHONY: clean
