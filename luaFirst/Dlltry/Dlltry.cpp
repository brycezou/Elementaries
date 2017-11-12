extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#ifdef _WIN32
#define LUA_API __declspec(dllexport)
#else
#define LUA_API
#endif


int Add_in_Cpp(lua_State *L)
{
	int argc = lua_gettop(L);
	if (argc != 2)
	{
		luaL_error(L, "Add_in_Cpp takes 2 arguments, %d is given\n", argc);
	}
	double num1 = lua_tonumber(L, 1);
	double num2 = lua_tonumber(L, 2);
	double sum = num1 + num2;
	lua_pushnumber(L, sum);
	return 1;
}

extern "C" int LUA_API luaopen_Dlltry(lua_State *L)
{
	struct luaL_reg driver[] = 
	{
		{"Add_in_Lua", Add_in_Cpp},
		{NULL, NULL}
	};
	luaL_register(L, "Dlltry", driver);
	fprintf(stderr, "Dlltry module is loaded!\n");
	return 1;
}