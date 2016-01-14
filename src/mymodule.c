#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <string.h>

/**
 * Maximum object name size.
 */
#define MOD_NAME_SIZE 32

/**
 * Lua metatable name to use.
 */
static const char MOD_MT_NAME[] = "mymodule";

/**
 * A struct containing the module state.
 */
struct mod_state
{
	char name[MOD_NAME_SIZE];
};

/**
 * Creates a new module instance. Memory is managed by Lua.
 *
 * @param s Lua state
 * @return Number of objects pushed.
 */
static int mod_new(lua_State *s)
{
	/* Get name (first argument) */
	const char* name = luaL_checkstring(s, 1);

	/* Create new state. If allocation fails, Lua will raise an error. */
	struct mod_state *ms = (struct mod_state*)lua_newuserdata(s,
		sizeof(struct mod_state));
	/* Clear all data */
	memset(ms, 0, sizeof(struct mod_state));

	/* Copy name */
	strncpy(ms->name, name, MOD_NAME_SIZE);
	ms->name[MOD_NAME_SIZE - 1] = '\0';

	/* Set metatable */
	luaL_getmetatable(s, MOD_MT_NAME);
	if (lua_istable(s, -1))
	{
		lua_setmetatable(s, -2);
	}
	else
	{
		luaL_error(s, "Metatable not found: %s", MOD_MT_NAME);
	}

	return 1;
}

/**
 * Gets a module state from the Lua stack.
 * If the object at the given index is no Lua userdata or the wrong metatable
 * is used, Lua will raise an error.
 *
 * @param s Lua state
 * @param idx Stack index
 * @return Pointer to module state.
 */
static inline struct mod_state* mod_checkptr(lua_State *s, int idx)
{
	struct mod_state *p =
		(struct mod_state*)luaL_checkudata(s, idx, MOD_MT_NAME);
	luaL_argcheck(s, p != NULL, 1, "module instance expected");

	return p;
}

/*
 * Gets the name from the module state.
 *
 * @param s Lua state
 * @return Number of objects pushed.
 */
static int mod_getName(lua_State *s)
{
	struct mod_state *ms = mod_checkptr(s, 1);
	lua_pushstring(s, ms->name);

	return 1;
}


/**
 * Handler for calls to __gc. Useful to perform some cleanup tasks like closing
 * open file handles or freeing memory.
 *
 * @param s Lua state
 * @return Number of objects pushed.
 */
static int mod_gc(lua_State *s)
{
	/* TODO Perform cleanup tasks */
	/* struct mod_state *ms = mod_checkptr(s); */

	return 0;
}

/* Module functions */
static luaL_Reg mod_funcs[] =
{
	{"getName", mod_getName},
	{"__gc", mod_gc},
	{"__tostring", mod_getName},
	{NULL, NULL}
};

/**
 * Opens the Lua module.
 *
 * @remark The name used here must be equal to what is used for require() in
 * Lua, e.g. require("mymodule") expects the function luaopen_mymodule() to
 * be present.
 * @param s Lua state
 * @return Number of objects pushed.
 */
int luaopen_mymodule(lua_State *s)
{
	/* Create module metatable */
	luaL_newmetatable(s, MOD_MT_NAME);
	lua_pushliteral(s, "__index");
	lua_pushvalue(s, -2);
	lua_rawset(s, -3);
	luaL_setfuncs(s, mod_funcs, 0);
	lua_pop(s, 1);

	/* Static module object */
	lua_newtable(s);
	lua_pushliteral(s, "new");
	lua_pushcfunction(s, mod_new);
	lua_rawset(s, -3);

	return 1;
}
