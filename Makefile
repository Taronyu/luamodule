# Set LUA_INC to Lua include directory and LUA_LIB to Lua libraries:
#
# LUA_INC = -I/usr/include/lua5.3
# LUA_LIB = -llua5.3
#
include Makefile.local

# Compiler and linker settings
CC = gcc
CFLAGS = -c -Wall -fpic $(LUA_INC)
LD = gcc
LDFLAGS = -shared
LIBS = $(LUA_LIB)
# Lua compiler
LUAC = luac

# Debug flags
ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g
else
	CFLAGS += -O2 -DNDEBUG
endif

# Input and output files
BIN = build/mymodule.so
SRC = src/mymodule.c
OBJ = $(SRC:src/%.c=build/%.o)
TEST_SRC = src/test.lua
TEST_BIN = $(TEST_SRC:src/%.lua=build/%.luac)

.PHONY: all clean

all: $(BIN) $(TEST_BIN)

$(BIN): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -o $@ $<

TEST_BIN: $(TEST_SRC)

build/%.luac: src/%.lua | build
	$(LUAC) -o $@ $<

build:
	mkdir $@

clean:
	rm -rf build/
