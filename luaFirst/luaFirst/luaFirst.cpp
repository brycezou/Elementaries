#include <iostream>
#include "luaFirst.h"
using namespace std;

lua_State *engine_open(char *initFile)
{
	lua_State *L = lua_open();
	luaL_openlibs(L);

	if (luaL_dofile(L, initFile) != 0)
	{
		const char* errmsg = lua_tostring(L, -1);
		cerr<<errmsg<<endl;
		lua_pop(L,1);
	}
	return L;
}

int engine_eval(lua_State *L, char *cmd)
{
	int retval = luaL_dostring(L, cmd);
	if (retval != 0)
	{
		const char* errmsg = lua_tostring(L, -1);
		cerr<<errmsg<<endl;
		lua_pop(L,1);
	}
	return retval;
}