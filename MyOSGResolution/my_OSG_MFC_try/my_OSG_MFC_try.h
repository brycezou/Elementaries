
// my_OSG_MFC_try.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C_OSG_MFC_App:
// �йش����ʵ�֣������ my_OSG_MFC_try.cpp
//

class C_OSG_MFC_App : public CWinApp
{
public:
	C_OSG_MFC_App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C_OSG_MFC_App theApp;