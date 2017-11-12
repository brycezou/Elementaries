// FriendInfo.cpp: implementation of the CFriendInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CESDC_Frame.h"
#include "FriendInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFriendInfo::CFriendInfo()
{

}

CFriendInfo::~CFriendInfo()
{

}

CFriendInfo::CFriendInfo(CString name, CString ip)
{
	this->m_Name=name;
	this->m_IP=ip;
}
