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
		char *init_file = "myLuaTest.lua";	//lua�ű��ļ���
		char *main_func = "main()";			//lua�ű��ļ��еĺ�����

		if (argc >= 2)
			init_file = argv[1];
		if (argc >= 3)
			main_func = argv[2];
		lua_State *L = engine_open(init_file);		//����lua�ű��ļ�

		while(true)
		{
			Sleep(1000);
			printf("Hello lua!\n");
			engine_eval(L, main_func);				//����������lua�ű��е� "main()" ����
		}

		return 0;
	}
	__finally{}
}