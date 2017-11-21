// ConfigE.cpp : implementation file
//

#include "stdafx.h"
#include "IFServer.h"
#include "ConfigE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigE dialog


CConfigE::CConfigE(CWnd* pParent /*=NULL*/)
	: CIFServerSubDlg(CConfigE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ptrMainApp = PTRAPP;
	m_ptrFileMan = new CFileManagement;
}


void CConfigE::DoDataExchange(CDataExchange* pDX)
{
	CIFServerSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigE)
	DDX_Control(pDX, IDC_LIST_ALERTS, m_lstAlert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigE, CDialog)
	//{{AFX_MSG_MAP(CConfigE)
	ON_WM_MOVE()
	ON_WM_HELPINFO()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON2, OnOpenDumpFile)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigE message handlers

void CConfigE::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	CRect rect;
    GetClientRect(&rect);
    ClientToScreen(&rect);
    bhPersistent.SetAnchorPoint(rect.BottomRight());
}

BOOL CConfigE::PreTranslateMessage(MSG* pMsg) 
{
	if ( NULL != pMsg->hwnd && ::GetDlgCtrlID(pMsg->hwnd) == IDC_LIST_ALERTS && pMsg->message == WM_RBUTTONDOWN)
   {      
      CBalloonHelp::LaunchBalloon("To View this keep holding the mouse ", "\tThis is the List of the Alert Generated of Last 100 Attempts\n\n\t\"Click Clear to Clean the Log\"\n", CPoint(pMsg->pt), IDI_WARNING, CBalloonHelp::unCLOSE_ON_RBUTTON_UP|CBalloonHelp::unDISABLE_FADE, this, "", 0);
      return TRUE;
   }
   else
   {
      return CDialog::PreTranslateMessage(pMsg);
   }
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CConfigE::OnHelpInfo(HELPINFO* pHelpInfo) 
{
   CString strTitle;
   CString strMsg;
   CRect rect;
   if ( NULL != pHelpInfo->hItemHandle && HELPINFO_WINDOW == pHelpInfo->iContextType )
      ::GetWindowRect((HWND)pHelpInfo->hItemHandle, &rect);

   switch (pHelpInfo->iCtrlId)
   {
   case IDC_LIST_ALERTS:
      strTitle = "Calculate";
      strMsg = "Click this button to perform the division.";
      break;
   /*case IDC_EDIT1:
      strTitle = "Dividend";
      strMsg = "Enter the number to be divided here.";
      break;
   case IDC_EDIT2:
      strTitle = "Divisor";
      strMsg = "Enter the number to divide by here.";
      break;
   case IDC_LBL_LCLICK:
      strTitle = "Left-button dependant balloon test area";
      strMsg = "Press and hold the left mouse button over this area.";
      break;
   case IDC_LBL_RCLICK:
      strTitle = "Right-button dependant balloon test area";
      strMsg = "Press and hold the right mouse button over this area.";
      break;
   case IDC_LBL_CLICK:
      strTitle = "Feedback balloon launcher area";
      strMsg = "Double-click this area.";
      break;
   case IDC_LBL_RESULT:
      strTitle = "Quotient";
      strMsg = "The result of the division will appear here.";
      break;
   case IDOK:
      strTitle = "Close";
      strMsg = "Click this button to close the program.";
      break;*/
   case 100:
	   break;
   default:
      return TRUE;
   }

   CBalloonHelp::LaunchBalloon(strTitle, strMsg, rect.CenterPoint(), IDI_INFORMATION, CBalloonHelp::unCLOSE_ON_MOUSE_MOVE);

   return TRUE;
}


void CConfigE::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CPoint pnt;
	::GetCursorPos(&pnt);
	CBalloonHelp::LaunchBalloon("Well...", "\tWhat do you think?  Is this good?  Is this useful?  Does it need a lot of work?  Or is it a complete waste of time...  Let me know your thoughts!\n\n\tClick this balloon to send me some feedback...", pnt, IDI_QUESTION, CBalloonHelp::unSHOW_CLOSE_BUTTON|CBalloonHelp::unSHOW_INNER_SHADOW|CBalloonHelp::unSHOW_TOPMOST, this, "mailto:shognine@yahoo.com", 0);

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CConfigE::OnOpenDumpFile() 
{
	// TODO: Add your control notification handler code here

	CFileDialog dlg( TRUE,_T("log"),_T("*.log"),
					 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
					 _T("Dump Log (*.log)|*.log|"));

	if( dlg.DoModal()==IDOK )
	{
		m_csLogFile = dlg.GetPathName();
		sprintf(m_ptrMainApp->pv.main_rules_file,"%s", m_csLogFile);
		UpdateData(FALSE);
	}
	::ShellExecute(NULL,"open","E:\\Winnt\\NOTEPAD.EXE",(LPCTSTR)m_csLogFile,NULL,SW_SHOWNORMAL);
	
}

void CConfigE::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(bShow)	
	{
		m_ptrFileMan->LoadAlertFile(0,this);
	}
}
