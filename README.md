# luamodule
A simple example for writing a Lua module in C.

## Building

Create Makefile.local and specify the Lua include directory and library:

```
# Example paths
LUA_INC = -I/usr/include/lua5.3
LUA_LIB = -llua5.3
```

Then just run make to build the module and compile the Lua test script.
