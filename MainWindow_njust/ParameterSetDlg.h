#if !defined(AFX_PARAMETERSETDLG_H__0BA1221E_0D67_4C3F_BFA8_93A07871512F__INCLUDED_)
#define AFX_PARAMETERSETDLG_H__0BA1221E_0D67_4C3F_BFA8_93A07871512F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParameterSetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParameterSetDlg dialog
#include "afxinet.h"
#include "afxtempl.h"
class CParameterSetDlg : public CDialog
{
// Construction
public:
    CInternetSession* m_pInetSession;
	CRect rtMain;
	void OnPaintPicture();
	HBITMAP hBitmap;
	CParameterSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParameterSetDlg)
	enum { IDD = IDD_DIALOG_PARA_SET };
	CStatic	m_Bitmap;
	CListBox	m_seedList;
	CSliderCtrl	m_sR;
	CSliderCtrl	m_sG;
	CSliderCtrl	m_sB;
	int		m_blockNum;
	int		m_blockSize;
	int		m_correctNum;
	int		m_lab;
	int		m_editG;
	int		m_editR;
	double	m_scr2Height_1;
	double	m_scr2Height_2;
	double	m_scr2Height_3;
	double	m_similarity;
	int		m_editB;
	int		m_deltTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParameterSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParameterSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditR();
	afx_msg void OnChangeEditG();
	afx_msg void OnChangeEditB();
	afx_msg void OnButtonSendPara();
	afx_msg void OnButtonGraps();
	afx_msg	void OnMyUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChangeEditBlockSize();
	afx_msg void OnChangeEditBlockNum();
	afx_msg void OnChangeEditScreen2height1();
	afx_msg void OnChangeEditLab();
	afx_msg void OnChangeEditCorrectNum();
	afx_msg void OnChangeEditScreen2height2();
	afx_msg void OnChangeEditSimilarity();
	afx_msg void OnChangeEditScreen2height3();
	afx_msg void OnChangeEditTime();
	afx_msg void OnButtonDeleteAll();
	afx_msg void OnPaint();
	afx_msg void OnShowOnlyPicture();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OnCaptureFrame();
	void OnInitial();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMETERSETDLG_H__0BA1221E_0D67_4C3F_BFA8_93A07871512F__INCLUDED_)
