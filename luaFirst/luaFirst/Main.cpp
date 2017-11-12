#include <windows.h>
#include <stdio.h>
#include "luaFirst.h"

LONG WINAPI MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionPtrs)  
{  
	printf("Unexpected Error!\n");
	exit(-1);
	return EXCEPTION_EXECUTE_HANDLER;   
}   

int main(int argc, char *argv[])
{
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);  
	__try
	{
		char *init_file = "myLuaTest.lua";	//lua脚本文件名
		char *main_func = "main()";			//lua脚本文件中的函数名

		if (argc >= 2)
			init_file = argv[1];
		if (argc >= 3)
			main_func = argv[2];
		lua_State *L = engine_open(init_file);		//加载lua脚本文件

		while(true)
		{
			Sleep(1000);
			printf("Hello lua!\n");
			engine_eval(L, main_func);				//解析并运行lua脚本中的 "main()" 函数
		}

		return 0;
	}
	__finally{}
}