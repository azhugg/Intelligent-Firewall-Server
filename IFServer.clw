; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CIFServerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "IFServer.h"

ClassCount=11
Class1=CIFServerApp
Class2=CIFServerDlg

ResourceCount=10
Resource2=IDD_ADAPTER (English (U.K.))
Resource1=IDR_MAINFRAME
Class3=TGest
Resource3=IDD_DIALOG3
Class4=CSelectAdaptor
Resource4=IDD_DIALOG4
Resource5=IDD_IFSERVER_DIALOG
Resource6=IDD_DIALOG1
Resource7=IDD_DIALOG5
Class5=CIFServerSubDlg
Class6=CConfigA
Class7=CConfigB
Class8=CConfigC
Class9=CConfigD
Resource8=IDD_DIALOG2
Class10=CConfigE
Resource9=IDD_DIALOG_SELECT_ADAPTORS
Class11=CAlertScreenDlg
Resource10=IDD_DIALOG6

[CLS:CIFServerApp]
Type=0
HeaderFile=IFServer.h
ImplementationFile=IFServer.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CIFServerApp

[CLS:CIFServerDlg]
Type=0
HeaderFile=IFServerDlg.h
ImplementationFile=IFServerDlg.cpp
Filter=D
LastObject=CIFServerDlg
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_IFSERVER_DIALOG]
Type=1
Class=CIFServerDlg
ControlCount=11
Control1=IDOK,button,1342242827
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON_STARTENGINE,button,1342242816
Control4=IDC_BUTTON_STOPENGINE,button,1073807360
Control5=IDC_BUTTON_SELECTADAPTOR,button,1073807360
Control6=IDC_STATIC_ADAPTOR,static,1073872897
Control7=IDC_PAGE_TREE,SysTreeView32,1350631479
Control8=IDC_PHELP,button,1342242816
Control9=IDC_DLG_FRAME,static,1073741832
Control10=IDC_CAPTION_BAR,static,1342308864
Control11=IDC_STATIC,static,1342177298

[CLS:TGest]
Type=0
HeaderFile=TGest.h
ImplementationFile=TGest.cpp
BaseClass=CDialog
Filter=D
LastObject=IDCANCEL

[CLS:CSelectAdaptor]
Type=0
HeaderFile=SelectAdaptor.h
ImplementationFile=SelectAdaptor.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_ADAPTER
VirtualFilter=dWC

[DLG:IDD_ADAPTER (English (U.K.))]
Type=1
Class=CSelectAdaptor
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631425
Control4=IDC_STATIC,static,1342308352
Control5=IDC_ADAPTER,static,1342308352

[DLG:IDD_DIALOG_SELECT_ADAPTORS]
Type=1
Class=CSelectAdaptor
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631425
Control4=IDC_STATIC,static,1342308352
Control5=IDC_ADAPTER,static,1342308352

[DLG:IDD_DIALOG1]
Type=1
Class=CConfigA
ControlCount=7
Control1=IDC_COMBO1,combobox,1344339969
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT1,edit,1350635696
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1350569990
Control6=IDC_SHOCKWAVEFLASH1,{D27CDB6E-AE6D-11CF-96B8-444553540000},1342242816
Control7=IDC_SHOCKWAVEFLASH2,{D27CDB6E-AE6D-11CF-96B8-444553540000},1342242816

[DLG:IDD_DIALOG2]
Type=1
Class=CConfigB
ControlCount=70
Control1=IDC_EDIT1,edit,1350639744
Control2=IDC_SPIN1,msctls_updown32,1342177462
Control3=IDC_EDIT2,edit,1082204288
Control4=IDC_SPIN2,msctls_updown32,1073741878
Control5=IDC_EDIT3,edit,1082204288
Control6=IDC_SPIN3,msctls_updown32,1073741878
Control7=IDC_EDIT4,edit,1082204288
Control8=IDC_SPIN4,msctls_updown32,1073741878
Control9=IDC_EDIT5,edit,1082204288
Control10=IDC_SPIN5,msctls_updown32,1073741878
Control11=IDC_EDIT6,edit,1082204288
Control12=IDC_SPIN6,msctls_updown32,1073741878
Control13=IDC_EDIT7,edit,1082204288
Control14=IDC_SPIN7,msctls_updown32,1073741878
Control15=IDC_EDIT8,edit,1082204288
Control16=IDC_SPIN8,msctls_updown32,1073741878
Control17=IDC_EDIT9,edit,1082204288
Control18=IDC_SPIN9,msctls_updown32,1073741878
Control19=IDC_EDIT10,edit,1082204288
Control20=IDC_SPIN10,msctls_updown32,1073741878
Control21=IDC_EDIT11,edit,1082204288
Control22=IDC_SPIN11,msctls_updown32,1073741878
Control23=IDC_EDIT12,edit,1082204288
Control24=IDC_SPIN12,msctls_updown32,1073741878
Control25=IDC_EDIT13,edit,1082204288
Control26=IDC_SPIN13,msctls_updown32,1073741878
Control27=IDC_EDIT14,edit,1082204288
Control28=IDC_SPIN14,msctls_updown32,1073741878
Control29=IDC_EDIT15,edit,1082204288
Control30=IDC_SPIN15,msctls_updown32,1073741878
Control31=IDC_EDIT16,edit,1082204288
Control32=IDC_SPIN16,msctls_updown32,1073741878
Control33=IDC_EDIT17,edit,1082204288
Control34=IDC_SPIN17,msctls_updown32,1073741878
Control35=IDC_EDIT18,edit,1082204288
Control36=IDC_SPIN18,msctls_updown32,1073741878
Control37=IDC_EDIT19,edit,1082204288
Control38=IDC_SPIN19,msctls_updown32,1073741878
Control39=IDC_EDIT20,edit,1082204288
Control40=IDC_SPIN20,msctls_updown32,1073741878
Control41=IDC_STATIC,button,1342177287
Control42=IDC_STATIC_INDI_1,static,1073741838
Control43=IDC_STATIC_INDI_2,static,1073741838
Control44=IDC_STATIC_INDI_3,static,1073741838
Control45=IDC_STATIC_INDI_4,static,1073741838
Control46=IDC_STATIC_INDI_5,static,1073741838
Control47=IDC_STATIC_INDI_6,static,1073741838
Control48=IDC_STATIC_INDI_7,static,1073741838
Control49=IDC_STATIC_INDI_8,static,1073741838
Control50=IDC_STATIC_INDI_9,static,1073741838
Control51=IDC_STATIC_INDI_10,static,1073741838
Control52=IDC_STATIC_INDI_11,static,1073741838
Control53=IDC_STATIC_INDI_12,static,1073741838
Control54=IDC_STATIC_INDI_13,static,1073741838
Control55=IDC_STATIC_INDI_14,static,1073741838
Control56=IDC_STATIC_INDI_15,static,1073741838
Control57=IDC_STATIC_DIR,button,1342177287
Control58=IDC_STATIC_DIRECTORY,static,1342312460
Control59=IDC_BUTTON1,button,1342275584
Control60=IDC_STATIC_RULES_DIR,button,1342177287
Control61=IDC_STATIC_RULES_DIRECTORY,static,1342312460
Control62=IDC_BUTTON4,button,1342275584
Control63=IDC_STATIC_RULES_FILE,button,1342177287
Control64=IDC_STATIC_RULE_FILE,static,1342312460
Control65=IDC_BUTTON5,button,1342275584
Control66=IDC_STATIC,static,1342181394
Control67=IDC_STATIC_DENY_LIST_FILE,button,1342177287
Control68=IDC_STATIC_DENY_LIST,static,1342312460
Control69=IDC_BUTTON6,button,1342275584
Control70=IDC_BUTTON3,button,1073807360

[DLG:IDD_DIALOG3]
Type=1
Class=CConfigC
ControlCount=16
Control1=IDC_PROGRESS1,msctls_progress32,1350565888
Control2=IDC_STATIC_PROGRESS_STATUS,static,1342308364
Control3=IDC_STATIC_ENGINE,button,1342177287
Control4=IDC_RADIO_SNIFFER,button,1342312457
Control5=IDC_RADIO_IDS_ONLY,button,1342181385
Control6=IDC_RADIO_FIREWALL,button,1342181385
Control7=IDC_STATIC_SNIFFER,static,1073741838
Control8=IDC_STATIC_IDS,static,1073741838
Control9=IDC_STATIC_FIREWALL,static,1073741838
Control10=IDC_STATIC_ENGINE_CONTROL,button,1342308359
Control11=IDC_RADIO_START_ENGINE,button,1342312457
Control12=IDC_RADIO_STOP_ENGINE,button,1342181385
Control13=IDC_STATIC_SELECT_ADAPTER,button,1342308359
Control14=IDC_BUTTON_SELECT_ADAPTER,button,1342242816
Control15=IDC_STATIC_ADAPTER_NAME,static,1342312449
Control16=IDC_STATIC,button,1342177287

[DLG:IDD_DIALOG4]
Type=1
Class=CConfigD
ControlCount=18
Control1=IDC_STATIC,static,1350569989
Control2=IDC_SCROLLBAR1,scrollbar,1073741829
Control3=IDC_STATIC,static,1082134532
Control4=IDC_CHECK3,button,1073811459
Control5=IDC_SCROLLBAR5,scrollbar,1073741829
Control6=IDC_IPADDRESS1,SysIPAddress32,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,button,1342177287
Control9=IDC_LIST_IDSRULES,listbox,1353777409
Control10=IDC_CHECK5,button,1342246915
Control11=IDC_STATIC_IDS,button,1342177287
Control12=IDC_LIST2,listbox,1352728835
Control13=IDC_BUTTON1,button,1342275584
Control14=IDC_LIST3,listbox,1352728833
Control15=IDC_BUTTON3,button,1342242816
Control16=IDC_BUTTON2,button,1342242816
Control17=IDC_BUTTON8,button,1342275584
Control18=IDC_BUTTON9,button,1342275584

[CLS:CIFServerSubDlg]
Type=0
HeaderFile=IFServerSubDlg.h
ImplementationFile=IFServerSubDlg.cpp
BaseClass=CDialog
LastObject=CIFServerSubDlg

[CLS:CConfigA]
Type=0
HeaderFile=ConfigA.h
ImplementationFile=ConfigA.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMBO1
VirtualFilter=dWC

[CLS:CConfigB]
Type=0
HeaderFile=ConfigB.h
ImplementationFile=ConfigB.cpp
BaseClass=CIFServerSubDlg
Filter=D
LastObject=CConfigB
VirtualFilter=dWC

[CLS:CConfigC]
Type=0
HeaderFile=ConfigC.h
ImplementationFile=ConfigC.cpp
BaseClass=CDialog
Filter=D
LastObject=CConfigC
VirtualFilter=dWC

[CLS:CConfigD]
Type=0
HeaderFile=ConfigD.h
ImplementationFile=ConfigD.cpp
BaseClass=CDialog
Filter=D
LastObject=CConfigD
VirtualFilter=dWC

[DLG:IDD_DIALOG5]
Type=1
Class=CConfigE
ControlCount=6
Control1=IDC_LIST_ALERTS,listbox,1352728833
Control2=IDC_STATIC_ALERTSTAT,button,1342177287
Control3=IDC_STATIC_DUMPSTAT,button,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_BUTTON1,button,1342242816
Control6=IDC_BUTTON2,button,1342279424

[CLS:CConfigE]
Type=0
HeaderFile=ConfigE.h
ImplementationFile=ConfigE.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CConfigE

[DLG:IDD_DIALOG6]
Type=1
Class=CAlertScreenDlg
ControlCount=1
Control1=IDC_SHOCKWAVEFLASH1,{D27CDB6E-AE6D-11CF-96B8-444553540000},1342177280

[CLS:CAlertScreenDlg]
Type=0
HeaderFile=AlertScreenDlg.h
ImplementationFile=AlertScreenDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CAlertScreenDlg

