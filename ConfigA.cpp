// ConfigA.cpp : implementation file
//

#include "stdafx.h"
#include "IFServer.h"
#include "ConfigA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigA dialog


CConfigA::CConfigA(CWnd* pParent /*=NULL*/)
	: CIFServerSubDlg(CConfigA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigA)
	m_Password = _T("");
	m_iComboSelection = _T("");
	//}}AFX_DATA_INIT
	m_ptrMainApp = PTRAPP;
	m_ptrMainApp->pv.checkpass = new char[30];
}


void CConfigA::DoDataExchange(CDataExchange* pDX)
{
	CIFServerSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigA)
		DDX_Text(pDX, IDC_EDIT1, m_Password);
		DDX_CBString(pDX, IDC_COMBO1, m_iComboSelection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigA, CDialog)
	//{{AFX_MSG_MAP(CConfigA)
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_EDIT1, OnChangePassword)
	ON_CBN_KILLFOCUS(IDC_COMBO1, OnKillfocusUserName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CConfigA::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
//	AfxMessageBox("Destroyed",MB_OK,0);
}

void CConfigA::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
//	AfxMessageBox("Kill Focus",MB_OK,0);
}

void CConfigA::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
//	if(!bShow && !nStatus)
//		AfxMessageBox("Lost Window Focus",MB_OK,0);	
}

void CConfigA::OnChangePassword() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	sprintf(m_ptrMainApp->pv.user_name,"%s", m_iComboSelection);
	sprintf(m_ptrMainApp->pv.checkpass,"%s", m_Password);
}

void CConfigA::OnKillfocusUserName() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	sprintf(m_ptrMainApp->pv.user_name,"%s", m_iComboSelection);
	sprintf(m_ptrMainApp->pv.checkpass,"%s", m_Password);
}
