extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

lua_State *engine_open(char *initFile="myInitTest.lua");
int engine_eval(lua_State *L, char *cmd);