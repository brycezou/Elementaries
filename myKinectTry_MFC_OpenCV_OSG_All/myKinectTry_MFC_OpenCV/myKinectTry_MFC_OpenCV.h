
// myKinectTry_MFC_OpenCV.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMyKinectTryApp:
// �йش����ʵ�֣������ myKinectTry_MFC_OpenCV.cpp
//

class CMyKinectTryApp : public CWinApp
{
public:
	CMyKinectTryApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMyKinectTryApp theApp;