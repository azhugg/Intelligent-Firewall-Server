// IFServerSubDlg.cpp: implementation of the CIFServerSubDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "IFServerSubDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CIFServerSubDlg, CDialog)

CIFServerSubDlg::CIFServerSubDlg()
{
	ASSERT(0);
}

CIFServerSubDlg::CIFServerSubDlg(UINT nID, CWnd *pParent /*=NULL*/)
: CDialog(nID)
{
   m_id = nID;
}


CIFServerSubDlg::~CIFServerSubDlg()
{

}

//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CIFServerSubDlg, CDialog)
	//{{AFX_MSG_MAP(CIFServerSubDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

void CIFServerSubDlg::OnOK()
{
   EndDialog(IDOK);
}
//////////////////////////////////////////////////////////////////////

void CIFServerSubDlg::OnCancel()
{
   EndDialog(IDCANCEL);
}

//////////////////////////////////////////////////////////////////////

BOOL CIFServerSubDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		TCHAR szClass[10];
		CWnd* pWndFocus = GetFocus();
		if (((pWndFocus = GetFocus()) != NULL) &&
			IsChild(pWndFocus) &&
			(pWndFocus->GetStyle() & ES_WANTRETURN) &&
			GetClassName(pWndFocus->m_hWnd, szClass, 10) &&
			(lstrcmpi(szClass, _T("EDIT")) == 0))
		{
			pWndFocus->SendMessage(WM_CHAR, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}

		return FALSE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////
