#if !defined(AFX_SHOW_H__EA52064E_4CD6_49CD_A909_9680F7AF9589__INCLUDED_)
#define AFX_SHOW_H__EA52064E_4CD6_49CD_A909_9680F7AF9589__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Show.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShow dialog

class CShow : public CDialog
{
// Construction
public:
	CShow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShow)
	enum { IDD = IDD_DIALOG_SHOW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShow)
	public:
	virtual int DoModal();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShow)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOW_H__EA52064E_4CD6_49CD_A909_9680F7AF9589__INCLUDED_)
