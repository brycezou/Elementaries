
// IDCardCorrectAndRecognize.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CIDCardApp:
// �йش����ʵ�֣������ IDCardCorrectAndRecognize.cpp
//

class CIDCardApp : public CWinApp
{
public:
	CIDCardApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CIDCardApp theApp;