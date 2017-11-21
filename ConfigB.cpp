// ConfigB.cpp : implementation file
//

#include "stdafx.h"
#include "IFServer.h"
#include "ConfigB.h"
//#include "IFServerSubDlg.h"
#include "AlertScreenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigB dialog


CConfigB::CConfigB(CWnd* pParent /*=NULL*/)
	: CIFServerSubDlg(CConfigB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigB)
	m_BHValue = _T("1");
	m_cstrFileName = _T("");
	m_csRulesDirectory = _T("");
	m_csRulesFile = _T("");
	//}}AFX_DATA_INIT
	
	stArray[0] =	&m_sIn1;
	stArray[1] =	&m_sIn2;
	stArray[2] =	&m_sIn3;
	stArray[3] =	&m_sIn4;
	stArray[4] =	&m_sIn5;
	stArray[5] =	&m_sIn6;
	stArray[6] =	&m_sIn7;
	stArray[7] =	&m_sIn8;	
	stArray[8] =	&m_sIn9;	
	stArray[9] =	&m_sIn10;
	stArray[10] =	&m_sIn11;
	stArray[11] =	&m_sIn12;
	stArray[12] =	&m_sIn13;
	stArray[13] =	&m_sIn14;
	stArray[14] =	&m_sIn15;
	startf = NULL;
	m_ptrMainApp = PTRAPP;

}


void CConfigB::DoDataExchange(CDataExchange* pDX)
{
	CIFServerSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigB)
	DDX_Control(pDX, IDC_STATIC_DENY_LIST, m_DenyList);
	DDX_Control(pDX, IDC_SPIN1, m_BHSen);
	DDX_Control(pDX, IDC_STATIC_INDI_9, m_sIn9);
	DDX_Control(pDX, IDC_STATIC_INDI_8, m_sIn8);
	DDX_Control(pDX, IDC_STATIC_INDI_7, m_sIn7);
	DDX_Control(pDX, IDC_STATIC_INDI_6, m_sIn6);
	DDX_Control(pDX, IDC_STATIC_INDI_5, m_sIn5);
	DDX_Control(pDX, IDC_STATIC_INDI_4, m_sIn4);
	DDX_Control(pDX, IDC_STATIC_INDI_3, m_sIn3);
	DDX_Control(pDX, IDC_STATIC_INDI_2, m_sIn2);
	DDX_Control(pDX, IDC_STATIC_INDI_15, m_sIn15);
	DDX_Control(pDX, IDC_STATIC_INDI_14, m_sIn14);
	DDX_Control(pDX, IDC_STATIC_INDI_13, m_sIn13);
	DDX_Control(pDX, IDC_STATIC_INDI_12, m_sIn12);
	DDX_Control(pDX, IDC_STATIC_INDI_10, m_sIn10);
	DDX_Control(pDX, IDC_STATIC_INDI_11, m_sIn11);
	DDX_Control(pDX, IDC_STATIC_INDI_1, m_sIn1);
	DDX_Text(pDX, IDC_EDIT1, m_BHValue);
	DDX_Text(pDX, IDC_STATIC_DIRECTORY, m_cstrFileName);
	DDX_Text(pDX, IDC_STATIC_RULES_DIRECTORY, m_csRulesDirectory);
	DDX_Text(pDX, IDC_STATIC_RULE_FILE, m_csRulesFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigB, CIFServerSubDlg)
	//{{AFX_MSG_MAP(CConfigB)
		ON_WM_SHOWWINDOW()
	ON_EN_UPDATE(IDC_EDIT1, OnUpdateEdit1)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, OnBrowse)
	ON_BN_CLICKED(IDC_BUTTON4, OnRulesDirectory)
	ON_BN_CLICKED(IDC_BUTTON5, OnMainRuleFile)
	ON_BN_CLICKED(IDC_BUTTON6, OnOpenDenyListFile)
	ON_BN_CLICKED(IDC_BUTTON3, Oncheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CConfigB message handlers
void CConfigB::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CIFServerSubDlg::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

BOOL CConfigB::OnInitDialog() 
{
	CIFServerSubDlg::OnInitDialog();
	char *ptrstr;
	ptrstr = new char[2];
	
	// TODO: Add extra initialization here
	m_BHSen.SetRange(1,15);
	SetIndicators(m_ptrMainApp->pv.critical_sensitivity-1); // Set From Configuration file
	sprintf(ptrstr,"%d",m_ptrMainApp->pv.critical_sensitivity);
	m_BHValue = ptrstr;
	m_csRulesDirectory = m_ptrMainApp->pv.rules_directory;
	m_csRulesFile = m_ptrMainApp->pv.main_rules_file;
	m_cstrFileName = m_ptrMainApp->pv.log_dir;
	m_DenyList.SetWindowText(m_ptrMainApp->pv.deny_list_file);
	UpdateData(FALSE);
	startf = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigB::SetIndicators(int value)
{	
	NullIndicator();
	for (int i= 0 ; i <= value ; i++)
		stArray[i]->ShowWindow(1);	
}

void CConfigB::NullIndicator()
{
	for (int i= 0 ; i <= 14 ; i++)
		stArray[i]->ShowWindow(0);	
}

void CConfigB::OnUpdateEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CIFServerSubDlg::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
}

void CConfigB::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CIFServerSubDlg::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	int val;
	
	if(startf)
	{
		UpdateData(TRUE);
		val = atoi(m_BHValue);
		SetIndicators(val-1);
		m_ptrMainApp->pv.critical_sensitivity = val;
	}
}

void CConfigB::OnBrowse() 
{
	/*	CFileDialog dlg( TRUE,_T("AVI"),_T("*.AVI"),
					 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
					 _T("Animation (*.AVI)|*.AVI|"));

	if( dlg.DoModal()==IDOK )
	{
		m_cstrFileName = dlg.GetPathName();
		UpdateData(FALSE);
	}	*/
	m_cstrFileName = m_ptrMainApp->BrowseForFolder(this->m_hWnd,"Log Directory",0);
	UpdateData(FALSE);
	sprintf(m_ptrMainApp->pv.log_dir,"%s",m_cstrFileName);
}

void CConfigB::OnRulesDirectory() 
{
	// TODO: Add your control notification handler code here
//	CIFServerApp *ptrMainApp;
//	m_ptrMainApp = PTRAPP;
	m_csRulesDirectory = m_ptrMainApp->BrowseForFolder(this->m_hWnd,"Rules Directory",0);
	UpdateData(FALSE);	
	sprintf(m_ptrMainApp->pv.rules_directory,"%s",m_csRulesDirectory);
}

void CConfigB::OnMainRuleFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg( TRUE,_T("rul"),_T("*.rul"),
					 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
					 _T("IDS Rules (*.rul)|*.rul|"));

	if( dlg.DoModal()==IDOK )
	{
		m_csRulesFile = dlg.GetPathName();
		sprintf(m_ptrMainApp->pv.main_rules_file,"%s", m_csRulesFile);
		UpdateData(FALSE);
	}
}

void CConfigB::OnOpenDenyListFile() 
{
	// TODO: Add your control notification handler code here
	CString string;
	CFileDialog dlg( TRUE,_T("dlt"),_T("*.dlt"),
					 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
					 _T("Deny List (*.dlt)|*.dlt|"));

	if( dlg.DoModal()==IDOK )
	{
		m_DenyList.SetWindowText(dlg.GetPathName());
		m_DenyList.GetWindowText(string);
		sprintf(m_ptrMainApp->pv.deny_list_file,"%s", string);
		UpdateData(FALSE);
	}
}

void CConfigB::Oncheck() 
{
	// TODO: Add your control notification handler code here
	CAlertScreenDlg ptrscreen;
	ptrscreen.DoModal();
}
