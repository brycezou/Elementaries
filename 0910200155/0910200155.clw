; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyLoad
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "0910200155.h"

ClassCount=6
Class1=CMy0910200155App
Class2=CMy0910200155Dlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_MY0910200155_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ATTENTION
Class4=CShow
Class5=CMemoryNode
Class6=CMyLoad
Resource4=IDD_DIALOG_SHOW

[CLS:CMy0910200155App]
Type=0
HeaderFile=0910200155.h
ImplementationFile=0910200155.cpp
Filter=N
LastObject=CMy0910200155App

[CLS:CMy0910200155Dlg]
Type=0
HeaderFile=0910200155Dlg.h
ImplementationFile=0910200155Dlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CMy0910200155Dlg

[CLS:CAboutDlg]
Type=0
HeaderFile=0910200155Dlg.h
ImplementationFile=0910200155Dlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_MY0910200155_DIALOG]
Type=1
Class=CMy0910200155Dlg
ControlCount=12
Control1=IDC_STATIC,static,1342308352
Control2=IDC_LIST_SHAPE,listbox,1352728833
Control3=IDC_STATIC,static,1342308352
Control4=IDC_COMBO_PEN_COLOR,combobox,1344339971
Control5=IDC_STATIC,static,1342308352
Control6=IDC_COMBO_BRUSH_COLOR,combobox,1344339971
Control7=IDC_BUTTON_DRAW,button,1342242817
Control8=IDC_BUTTON_CLEAN,button,1342242817
Control9=IDC_BUTTON_SHOW,button,1342242817
Control10=IDOK,button,1342242817
Control11=IDC_DRAW_PLACE,button,1342210055
Control12=IDC_BUTTON_LOAD,button,1342242817

[DLG:IDD_DIALOG_SHOW]
Type=1
Class=CShow
ControlCount=0

[CLS:CShow]
Type=0
HeaderFile=Show.h
ImplementationFile=Show.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CShow

[CLS:CMemoryNode]
Type=0
HeaderFile=MemoryNode.h
ImplementationFile=MemoryNode.cpp
BaseClass=CDocument
Filter=N
LastObject=CMemoryNode
VirtualFilter=DC

[DLG:IDD_ATTENTION]
Type=1
Class=CAboutDlg
ControlCount=1
Control1=IDOK,button,1342242816

[CLS:CMyLoad]
Type=0
HeaderFile=MyLoad.h
ImplementationFile=MyLoad.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CMyLoad

