// TGest.cpp : implementation file
//

#include "stdafx.h"
#include "IFServer.h"
#include "TGest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TGest dialog


TGest::TGest(CWnd* pParent /*=NULL*/)
	: CDialog(TGest::IDD, pParent)
{
	//{{AFX_DATA_INIT(TGest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void TGest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TGest)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TGest, CDialog)
	//{{AFX_MSG_MAP(TGest)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TGest message handlers
