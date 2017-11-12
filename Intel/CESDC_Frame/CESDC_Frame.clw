; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTongBuDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CESDC_Frame.h"

ClassCount=8
Class1=CCESDC_FrameApp
Class2=CCESDC_FrameDlg
Class3=CAboutDlg

ResourceCount=8
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG_SEND_FILE
Class4=CEditFriendInfoDlg
Resource4=IDD_DIALOG_TONG_BU
Resource5=IDD_DIALOG_MEETING_SELECT
Class5=CTongBuDlg
Class6=CMasterOrSlave
Resource6=IDD_CESDC_FRAME_DIALOG
Class7=CTransFileDlg
Resource7=IDD_DIALOG_ADD_FRIEND
Class8=CMyAddr
Resource8=IDD_DIALOG_MY_IP

[CLS:CCESDC_FrameApp]
Type=0
HeaderFile=CESDC_Frame.h
ImplementationFile=CESDC_Frame.cpp
Filter=N
LastObject=CCESDC_FrameApp

[CLS:CCESDC_FrameDlg]
Type=0
HeaderFile=CESDC_FrameDlg.h
ImplementationFile=CESDC_FrameDlg.cpp
Filter=W
LastObject=CCESDC_FrameDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=CESDC_FrameDlg.h
ImplementationFile=CESDC_FrameDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CESDC_FRAME_DIALOG]
Type=1
Class=CCESDC_FrameDlg
ControlCount=12
Control1=IDC_LIST_FRIEND_NAME,SysListView32,1350631425
Control2=IDC_BUTTON_ADD_FRIEND,button,1342242816
Control3=IDC_BUTTON_DELETE_FRIEND,button,1342242816
Control4=IDC_BUTTON_MEETING,button,1342242816
Control5=IDC_BUTTON_SEND_FILE,button,1342242816
Control6=IDC_LIST_MEMBER_NAME,SysListView32,1350631425
Control7=IDC_BUTTON_ADD_MEMBER,button,1342242816
Control8=IDC_BUTTON_DELETE_MEMBER,button,1342242816
Control9=IDC_BUTTON_START_MEETING,button,1342242816
Control10=IDC_BUTTON_STOP_MEETING,button,1342242816
Control11=IDC_FRAME,static,1073741828
Control12=IDC_BUTTON_MY_ID,button,1342242816

[DLG:IDD_DIALOG_ADD_FRIEND]
Type=1
Class=CEditFriendInfoDlg
ControlCount=6
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_NAME,edit,1350631552
Control6=IDC_EDIT_IP,edit,1350631552

[CLS:CEditFriendInfoDlg]
Type=0
HeaderFile=EditFriendInfoDlg.h
ImplementationFile=EditFriendInfoDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT_NAME

[DLG:IDD_DIALOG_MEETING_SELECT]
Type=1
Class=CMasterOrSlave
ControlCount=2
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816

[DLG:IDD_DIALOG_TONG_BU]
Type=1
Class=CTongBuDlg
ControlCount=1
Control1=IDC_PICTURE,static,1342177294

[CLS:CTongBuDlg]
Type=0
HeaderFile=TongBuDlg.h
ImplementationFile=TongBuDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_PICTURE
VirtualFilter=dWC

[CLS:CMasterOrSlave]
Type=0
HeaderFile=MasterOrSlave.h
ImplementationFile=MasterOrSlave.cpp
BaseClass=CDialog
Filter=D
LastObject=CMasterOrSlave
VirtualFilter=dWC

[DLG:IDD_DIALOG_SEND_FILE]
Type=1
Class=CTransFileDlg
ControlCount=4
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_FILE_PATH,SysListView32,1350631426
Control4=IDC_BUTTON_CHOOSE_FILE,button,1342242816

[CLS:CTransFileDlg]
Type=0
HeaderFile=TransFileDlg.h
ImplementationFile=TransFileDlg.cpp
BaseClass=CDialog
Filter=W
VirtualFilter=dWC
LastObject=CTransFileDlg

[DLG:IDD_DIALOG_MY_IP]
Type=1
Class=CMyAddr
ControlCount=2
Control1=IDOK,button,1342242816
Control2=IDC_EDIT_MY_IPADDR,edit,1484851328

[CLS:CMyAddr]
Type=0
HeaderFile=MyAddr.h
ImplementationFile=MyAddr.cpp
BaseClass=CDialog
Filter=D
LastObject=CMyAddr
VirtualFilter=dWC

