; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CParameterSetDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mainwindow_njust.h"
LastPage=0

ClassCount=4
Class1=CMainWindow_njustApp
Class2=CMainWindow_njustDlg
Class3=CParameterSetDlg

ResourceCount=3
Resource1=IDD_DIALOG_PARA_SET
Resource2=IDD_MAINWINDOW_NJUST_DIALOG
Class4=CCatchShow
Resource3=IDD_DIALOG_SHOW

[CLS:CMainWindow_njustApp]
Type=0
BaseClass=CWinApp
HeaderFile=MainWindow_njust.h
ImplementationFile=MainWindow_njust.cpp
LastObject=CMainWindow_njustApp
Filter=N
VirtualFilter=AC

[CLS:CMainWindow_njustDlg]
Type=0
BaseClass=CDialog
HeaderFile=MainWindow_njustDlg.h
ImplementationFile=MainWindow_njustDlg.cpp
LastObject=CMainWindow_njustDlg
Filter=D
VirtualFilter=dWC

[CLS:CParameterSetDlg]
Type=0
BaseClass=CDialog
HeaderFile=ParameterSetDlg.h
ImplementationFile=ParameterSetDlg.cpp
LastObject=CParameterSetDlg
Filter=D
VirtualFilter=dWC

[DLG:IDD_MAINWINDOW_NJUST_DIALOG]
Type=1
Class=CMainWindow_njustDlg
ControlCount=15
Control1=IDC_EDIT_ADDRESS,edit,1350631552
Control2=IDC_EDIT_PORT,edit,1350631552
Control3=IDC_BUTTON_CONNECT,button,1342242816
Control4=IDC_EDIT_SYSTEM_STATE,edit,1342244992
Control5=IDC_BUTTON_PARA,button,1342242816
Control6=IDC_BUTTON_INSTRUCT,button,1342242816
Control7=IDC_BUTTON_STOP,button,1342242816
Control8=IDC_BUTTON_GRASP,button,1342242816
Control9=IDC_LIST_INSTRCTION,SysListView32,1350631681
Control10=IDC_BUTTON_REFRESH,button,1342242816
Control11=IDC_BUTTON_DELETE,button,1342242816
Control12=IDC_BUTTON_BACK,button,1342242816
Control13=IDC_FRAME,static,1073741828
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_PARA_SET]
Type=1
Class=CParameterSetDlg
ControlCount=38
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_BLOCK_SIZE,edit,1350631552
Control8=IDC_EDIT_BLOCK_NUM,edit,1350631552
Control9=IDC_EDIT_SCREEN2HEIGHT_1,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDIT_LAB,edit,1350631552
Control14=IDC_EDIT_CORRECT_NUM,edit,1350631552
Control15=IDC_EDIT_SCREEN2HEIGHT_2,edit,1350631552
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_EDIT_R,edit,1350631552
Control19=IDC_STATIC,static,1342308352
Control20=IDC_EDIT_G,edit,1350631552
Control21=IDC_STATIC,static,1342308352
Control22=IDC_EDIT_B,edit,1350631552
Control23=IDC_SLIDER_R,msctls_trackbar32,1342242840
Control24=IDC_SLIDER_G,msctls_trackbar32,1342242840
Control25=IDC_SLIDER_B,msctls_trackbar32,1342242840
Control26=IDC_STATIC,static,1342308352
Control27=IDC_EDIT_SIMILARITY,edit,1350631552
Control28=IDC_STATIC,static,1342308352
Control29=IDC_EDIT_SCREEN2HEIGHT_3,edit,1350631552
Control30=IDC_STATIC,static,1342308352
Control31=IDC_LIST_SEED,listbox,1352728833
Control32=IDC_BITMAP,static,1342177294
Control33=IDC_STATIC,static,1342308352
Control34=IDC_EDIT_TIME,edit,1350631552
Control35=IDC_FRAME_TWO,static,1342177287
Control36=IDC_BUTTON_SEND_PARA,button,1342242816
Control37=IDC_BUTTON_GRAPS,button,1342242816
Control38=IDC_BUTTON_DELETE_ALL,button,1342242816

[DLG:IDD_DIALOG_SHOW]
Type=1
Class=CCatchShow
ControlCount=1
Control1=IDC_PICTURE,static,1342177294

[CLS:CCatchShow]
Type=0
HeaderFile=CatchShow.h
ImplementationFile=CatchShow.cpp
BaseClass=CDialog
Filter=D
LastObject=CCatchShow
VirtualFilter=dWC

