#if !defined(AFX_MYLOAD_H__69AFD653_7536_4709_A5DA_33C01953FBE0__INCLUDED_)
#define AFX_MYLOAD_H__69AFD653_7536_4709_A5DA_33C01953FBE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyLoad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyLoad dialog

class CMyLoad : public CDialog
{
// Construction
public:
	CMyLoad(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CMyLoad)
	enum { IDD = IDD_MY_LOAD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyLoad)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyLoad)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLOAD_H__69AFD653_7536_4709_A5DA_33C01953FBE0__INCLUDED_)
