#if !defined(AFX_MASTERORSLAVE_H__8D93C70B_26CA_486D_8D72_B49F884A8992__INCLUDED_)
#define AFX_MASTERORSLAVE_H__8D93C70B_26CA_486D_8D72_B49F884A8992__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MasterOrSlave.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMasterOrSlave dialog

class CMasterOrSlave : public CDialog
{
// Construction
public:
	CMasterOrSlave(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMasterOrSlave)
	enum { IDD = IDD_DIALOG_MEETING_SELECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMasterOrSlave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMasterOrSlave)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MASTERORSLAVE_H__8D93C70B_26CA_486D_8D72_B49F884A8992__INCLUDED_)
