#pragma once
#include "CoreOSG.h"

// COSGShowDlg 对话框

class COSGShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COSGShowDlg)

public:
	COSGShowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COSGShowDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_OSG };

	CCoreOSG	*m_OSG;
	HANDLE		m_OSGThreadHandle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
