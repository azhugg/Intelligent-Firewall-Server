// ConfigurationA.cpp : implementation file
//

#include "stdafx.h"
#include "IFServer.h"
#include "ConfigurationA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigurationA dialog


CConfigurationA::CConfigurationA(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigurationA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigurationA)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConfigurationA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigurationA)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigurationA, CDialog)
	//{{AFX_MSG_MAP(CConfigurationA)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigurationA message handlers
