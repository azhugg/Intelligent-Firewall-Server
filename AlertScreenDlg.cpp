// AlertScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IFServer.h"
#include "AlertScreenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlertScreenDlg dialog

//VOID CALLBACK BProcessorProc(HWND hwnd,UINT message,UINT idTimer,DWORD dwTime);

CAlertScreenDlg::CAlertScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlertScreenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlertScreenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAlertScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlertScreenDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlertScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CAlertScreenDlg)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlertScreenDlg message handlers

BEGIN_EVENTSINK_MAP(CAlertScreenDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAlertScreenDlg)
	ON_EVENT(CAlertScreenDlg, IDC_SHOCKWAVEFLASH1, 1958 /* OnProgress */, OnOnProgressShockwaveflash1, VTS_I4)
	ON_EVENT(CAlertScreenDlg, IDC_SHOCKWAVEFLASH1, -609 /* OnReadyStateChange */, OnOnReadyStateChangeShockwaveflash1, VTS_I4)
	ON_EVENT(CAlertScreenDlg, IDC_SHOCKWAVEFLASH1, 150 /* FSCommand */, OnFSCommandShockwaveflash1, VTS_BSTR VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAlertScreenDlg::OnOnProgressShockwaveflash1(long percentDone) 
{
	// TODO: Add your control notification handler code here
//	if()
	AfxMessageBox("Percent Done");
}

void CAlertScreenDlg::OnOnReadyStateChangeShockwaveflash1(long newState) 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("Percent Done");	
}

void CAlertScreenDlg::OnFSCommandShockwaveflash1(LPCTSTR command, LPCTSTR args) 
{
	// TODO: Add your control notification handler code here
	
}

void CAlertScreenDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
}

BOOL CAlertScreenDlg::InitInstance()
{
	//nResult = ::SetTimer(NULL,0,5000,(TIMERPROC) BProcessorProc);
	return TRUE;
}

void CAlertScreenDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == 32)
	{
		this->DestroyWindow();
	}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAlertScreenDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == 32)
	{
		this->DestroyWindow();
	}
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}
