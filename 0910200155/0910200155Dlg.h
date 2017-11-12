// 0910200155Dlg.h : header file
//

#if !defined(AFX_0910200155DLG_H__B725260A_97D5_4D14_BB82_C84AD25FD2F5__INCLUDED_)
#define AFX_0910200155DLG_H__B725260A_97D5_4D14_BB82_C84AD25FD2F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMy0910200155Dlg dialog
#include "MemoryNode.h"
class CMy0910200155Dlg : public CDialog
{
// Construction
public:
	void OnFileRead();
	void OnFileWrite();
	CPtrArray pta;  
	CMemoryNode *pmN;
	CMemoryNode *pmn;
	COLORREF m_CurrentBrushColor;
	COLORREF m_CurrentPenColor;
	CMy0910200155Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMy0910200155Dlg)
	enum { IDD = IDD_MY0910200155_DIALOG };
	CButton	m_drawplace;
	CListBox	m_Shape;
	CComboBox	m_BrushColor;
	CComboBox	m_PenColor;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy0910200155Dlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMy0910200155Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelendokComboPenColor();
	afx_msg void OnSelendokComboBrushColor();
	afx_msg void OnButtonDraw();
	afx_msg void OnButtonShow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonClean();
	afx_msg void OnButtonLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_0910200155DLG_H__B725260A_97D5_4D14_BB82_C84AD25FD2F5__INCLUDED_)
