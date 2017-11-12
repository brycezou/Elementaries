// FriendInfo.h: interface for the CFriendInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRIENDINFO_H__D59D3B84_87B9_4CCB_B3AB_89207E82D544__INCLUDED_)
#define AFX_FRIENDINFO_H__D59D3B84_87B9_4CCB_B3AB_89207E82D544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFriendInfo  
{
public:
	CFriendInfo();
	CFriendInfo(CString,CString);
	CString m_Name;
	CString m_IP;
	virtual ~CFriendInfo();

};

#endif // !defined(AFX_FRIENDINFO_H__D59D3B84_87B9_4CCB_B3AB_89207E82D544__INCLUDED_)
