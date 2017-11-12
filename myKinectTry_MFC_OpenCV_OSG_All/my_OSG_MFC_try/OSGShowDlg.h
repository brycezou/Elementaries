#pragma once
#include "CoreOSG.h"

// COSGShowDlg �Ի���

class COSGShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COSGShowDlg)

public:
	COSGShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COSGShowDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_OSG };

	CCoreOSG	*m_OSG;
	HANDLE		m_OSGThreadHandle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
