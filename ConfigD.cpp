// ConfigD.cpp : implementation file
//

#include "stdafx.h"
#include "IFServer.h"
#include "ConfigD.h"
#include "IFServerSubDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NOT_SELECTED -1
#define FAILED		1
#define SUCCESS		2

/////////////////////////////////////////////////////////////////////////////
// CConfigD dialog


CConfigD::CConfigD(CWnd* pParent /*=NULL*/)
	: CIFServerSubDlg(CConfigD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigD)
	//}}AFX_DATA_INIT
	m_ptrMainApp = PTRAPP;
	m_ptrTraffic = new CTrafficControl;
}


void CConfigD::DoDataExchange(CDataExchange* pDX)
{
	CIFServerSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigD)
	DDX_Control(pDX, IDC_IPADDRESS1, m_IpAdd);
	DDX_Control(pDX, IDC_LIST3, m_DenyList);
	DDX_Control(pDX, IDC_LIST2, m_IncludeList);
	DDX_Control(pDX, IDC_LIST_IDSRULES, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigD, CDialog)
	//{{AFX_MSG_MAP(CConfigD)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON1, OnOpenIncludeFile)
	ON_BN_CLICKED(IDC_CHECK5, OnOpenMainRuleFile)
	ON_BN_CLICKED(IDC_BUTTON9, OnStatistics)
	ON_BN_CLICKED(IDC_BUTTON2, OnDeleteDenyFilter)
	ON_BN_CLICKED(IDC_BUTTON8, OnAddClientToDeny)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigD message handlers

BOOL CConfigD::OnInitDialog() 
{
	CIFServerSubDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ptrFM = new CFileManagement;
	m_ptrFM->OpenIDSRulesFile("e:/temp/rules/rules.sample",this);
	m_ptrTraffic->NumberedListRulesOnInterface("eth0");
	RefreshDenyList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigD::OnButton3() 
{
	int timeout = 0;
//	while(m_ptrMainApp->pv.flush_failure != SUCCESS && timeout < 50)
//	{
		m_ptrTraffic->FlushInterface("all");timeout++;
//	}
	
	if (m_ptrMainApp->pv.flush_failure == SUCCESS)
	{
		m_ptrTraffic->ReadFiltersFromFile("e:/temp/denylist/filter.dlt");
		m_ptrTraffic->NumberedListRulesOnInterface("eth0");
		m_DenyList.ResetContent();
		RefreshDenyList();
		m_ptrMainApp->pv.flush_failure == FAILED; 
	}
}

void CConfigD::OnOpenIncludeFile() 
{
	// TODO: Add your control notification handler code here
	CString str;
	
	if(m_IncludeList.GetCurSel() == NOT_SELECTED){
		AfxMessageBox("Select the Include file to Open",MB_OK,0);
		return;
	}
	m_IncludeList.GetText(m_IncludeList.GetCurSel(),str);	
	::ShellExecute(NULL,"open","E:\\Winnt\\NOTEPAD.EXE",(LPCTSTR)str,NULL,SW_SHOWNORMAL);	// 
}

void CConfigD::OnOpenMainRuleFile() 
{
	// TODO: Add your control notification handler code here
	if(m_ptrMainApp->pv.main_rules_file == NULL){
		AfxMessageBox("Main Rules file not specified in Configuration Panel",MB_OK,0);
		return;
	}
		
	::ShellExecute(NULL,"open","E:\\Winnt\\NOTEPAD.EXE",(LPCTSTR)m_ptrMainApp->pv.main_rules_file,NULL,SW_SHOWNORMAL);	// 	
}

void CConfigD::OnStatistics() 
{
	m_ptrTraffic->DetailedStatsOnInterface("eth0");

	AfxMessageBox(m_ptrMainApp->m_DenyStatus, MB_OK, 0); 	
}

void CConfigD::RefreshDenyList()
{
	CString string;
	CString liststring;
	int pos =0, terminator = 1;
	string = m_ptrMainApp->m_DenyList; //m_ptrMainApp->trafficControl->RulesOnInterface;
	if (string.Find("error: no filter yet",0) != -1)
	{
		return;
	}
	while(terminator != -1)
	{
		pos = string.Find("\n",0);
		liststring = string.Left(pos);
		if(pos == -1)
		{
			m_DenyList.AddString(string);
		}else
			m_DenyList.AddString(liststring);
		string = string.Right(string.GetLength()-pos);
		
		string.TrimLeft();
		string.TrimRight();
		terminator = pos;
	}
}

void CConfigD::OnDeleteDenyFilter() 
{
	CString str;
	char *filter;
	filter = new char[256];
	
	if(m_DenyList.GetCurSel() == NOT_SELECTED){
		AfxMessageBox("Select the Filter To Delete",MB_OK,0);
		return;
	}
	if(m_DenyList.GetCurSel() >= 0 && m_DenyList.GetCurSel() < 3)
	{
		AfxMessageBox("Cannot Delete Default Rules",MB_OK,0);
		return;
	}
	m_DenyList.GetText(m_DenyList.GetCurSel(),str);	 	
	str = str.Right(str.GetLength()-7);
	sprintf(filter,"%s",str);
	m_ptrMainApp->Strip(filter);
	m_ptrTraffic->DeleteFilter(filter);
	OnButton3(); //Refresh the Service;
}

void CConfigD::OnAddClientToDeny() 
{
	char *filter;
	filter = new char[STD_BUF];
	CString str;
	m_IpAdd.GetWindowText(str);
	str.TrimLeft();
	if(str == "0.0.0.0")
	{
		AfxMessageBox("Must Enter Ip To Block");
		return;
	}
	m_IpAdd.SetWindowText("");
	sprintf(filter, "block in on eth0 from %s to any\n",str);
	m_ptrTraffic->WriteFilter(filter);
	m_ptrTraffic->AddFilterToList(filter);
	OnButton3();
}
