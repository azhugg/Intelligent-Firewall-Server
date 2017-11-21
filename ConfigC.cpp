// ConfigC.cpp : implementation file
//

#include "stdafx.h"
#include "IFServer.h"
#include "ConfigC.h"
#include "IFServerSubDlg.h"
#include "SelectAdaptor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigC dialog


CConfigC::CConfigC(CWnd* pParent /*=NULL*/)
	: CIFServerSubDlg(CConfigC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigC)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ptrMainApp = PTRAPP;
	m_Mode = 2;
}


void CConfigC::DoDataExchange(CDataExchange* pDX)
{
	CIFServerSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigC)
	DDX_Control(pDX, IDC_STATIC_ADAPTER_NAME, m_stAdapName);
		DDX_Control(pDX, IDC_STATIC_ENGINE, m_btEngineMode);
		DDX_Control(pDX, IDC_RADIO_SNIFFER, m_btMode);
		DDX_Control(pDX, IDC_BUTTON_SELECT_ADAPTER, m_btSelAdap);
		DDX_Control(pDX, IDC_RADIO_START_ENGINE, m_StartEngine);
		DDX_Control(pDX, IDC_RADIO_STOP_ENGINE, m_StopEngine);
		DDX_Control(pDX, IDC_STATIC_PROGRESS_STATUS, m_stStatus);
		DDX_Control(pDX, IDC_STATIC_SNIFFER, m_stSniffer);
		DDX_Control(pDX, IDC_STATIC_IDS, m_stIDS);
		DDX_Control(pDX, IDC_STATIC_FIREWALL, m_stFirewall);
		DDX_Control(pDX, IDC_RADIO_IDS_ONLY, m_btIDS);
		DDX_Control(pDX, IDC_RADIO_FIREWALL, m_btFirewall);
		DDX_Control(pDX, IDC_PROGRESS1, m_prog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigC, CDialog)
	//{{AFX_MSG_MAP(CConfigC)
		ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO_SNIFFER, OnRadioSniffer)
	ON_BN_CLICKED(IDC_RADIO_IDS_ONLY, OnRadioIdsOnly)
	ON_BN_CLICKED(IDC_RADIO_FIREWALL, OnRadioFirewall)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ADAPTER, OnButtonSelectAdapter)
	ON_BN_CLICKED(IDC_RADIO_START_ENGINE, OnRadioStartEngine)
	ON_BN_CLICKED(IDC_RADIO_STOP_ENGINE, OnRadioStopEngine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigC message handlers

void CConfigC::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CIFServerSubDlg::OnShowWindow(bShow, nStatus);
	
//	m_prog.SetPos(rand() % 100);
}

BOOL CConfigC::OnInitDialog() 
{
	CIFServerSubDlg::OnInitDialog();
	CFont m_font;
//	m_stStatus.SetFont(
	if (m_font.GetSafeHandle()==NULL)
		{
			m_font.CreateFont(12,0, 0, 1, 
							  0,0, 0, 0, ANSI_CHARSET,
							  OUT_DEFAULT_PRECIS,
							  CLIP_DEFAULT_PRECIS,
							  DEFAULT_QUALITY,
							  FF_MODERN,
							  "Century Gothic");
		}

//		if (m_font.GetSafeHandle()!=NULL)
//		pOldFont = dc.SelectObject(&m_font);
//	m_stStatus.SetFont(&m_font,TRUE);


	m_prog.SetRange(0, 100);
	m_prog.ShowWindow(FALSE);
	m_StartEngine.EnableWindow(FALSE);
	m_StopEngine.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigC::OnRadioSniffer() 
{
	// TODO: Add your control notification handler code here
	
	m_ptrMainApp->pv.mode = MD_SNIFFER;
	m_stSniffer.ShowWindow(1);
	m_stFirewall.ShowWindow(0);
	m_stIDS.ShowWindow(0);
	m_Mode = 0;
}

void CConfigC::OnRadioIdsOnly() 
{
	// TODO: Add your control notification handler code here
	m_ptrMainApp->pv.mode = MD_IDS_ONLY;
	m_stSniffer.ShowWindow(0);
	m_stFirewall.ShowWindow(0);
	m_stIDS.ShowWindow(1);
	m_Mode = 1;
}

void CConfigC::OnRadioFirewall() 
{
	// TODO: Add your control notification handler code here
	m_ptrMainApp->pv.mode = MD_FIREWALL;
	m_stSniffer.ShowWindow(0);
	m_stFirewall.ShowWindow(1);
	m_stIDS.ShowWindow(0);
	m_Mode = 2;
}

void CConfigC::OnButtonSelectAdapter() 
{
	// TODO: Add your control notification handler code here
	CSelectAdaptor dlg;
	
	//show the selectadapter dialog
	dlg.m_Adapter=Adapter;
	if (dlg.DoModal()==IDOK) Adapter=dlg.m_Adapter;
	
	//write the name of the adapter in the title bar of the program's window
//	CWnd *pater=this->GetParent();
	CWnd *ptrDlg = this;
	CString titlestr;
	CString tmpstr;

	tmpstr=Adapter.Right(100);
	if(tmpstr.GetLength()==Adapter.GetLength())
	{
		if (!Adapter.GetLength())
		{
			return;
		}
		titlestr.Format("IFServer-%s",Adapter);
		m_stAdapName.SetWindowText(titlestr);
		m_StartEngine.EnableWindow(TRUE);
	}
	else
		titlestr.Format("IFServer-...%s",tmpstr);
	
//	m_SelAdaptor.SetWindowText(titlestr);
	
}

void CConfigC::OnRadioStartEngine() 
{
	m_btSelAdap.EnableWindow(FALSE);
	if(m_Mode == 2)
	{
		m_btFirewall.SetCheck(1);
		m_stSniffer.ShowWindow(0);
		m_stFirewall.ShowWindow(1);
		m_stIDS.ShowWindow(0);
	}
	m_ptrMainApp->StartSniff();
	m_StartEngine.EnableWindow(0);
	m_prog.ShowWindow(TRUE);
	m_prog.SetRange(0,100);
	long loop =0,i=0;
	while(i != 101)
	{
		m_prog.SetPos(i);
		i++;
		loop = 0;
		if(m_Mode != 0)
		{
			for(int j = 0;j<= 100;j++)
			{
				if(i == 27 || i == 77 || i ==39 || i == 50 || i ==36 || i == 100 && (m_Mode == 2 || m_Mode == 1))
				{
					while(loop != 50000000)
					{
						loop++;
					}
				}
				else
				{
					while(loop != 5000000)
					{
						loop++;
					}
				}
			}
		}
		else if(i != 100)
		{
			while(loop != 5000000)
					{
						loop++;
					}
			if (i > 45)
				i = 100;
		}
				
		if (i < 25)
		{
			m_stStatus.SetWindowText(m_ptrMainApp->pv.datalink);
		}
		else if (i < 35 && i > 25 && (m_Mode == 2 || m_Mode == 1))
		{
    		m_stStatus.SetWindowText("Initializing Decode Engine");
		}
		else if (i < 55 && i > 35 && (m_Mode == 2 || m_Mode == 1))
		{
			m_stStatus.SetWindowText("Initializing Input Plugins");
		}
		else if (i < 75 && i > 55 && (m_Mode == 2 || m_Mode == 1))
		{
			m_stStatus.SetWindowText("Initializing Output Plugins");
		}
		else if (i < 85 && i > 75 && (m_Mode == 2 || m_Mode == 1))
		{
			m_stStatus.SetWindowText("Initializing Rules Engine");
		}
		else if (i < 95 && i > 85 && m_Mode == 2)
		{
			m_stStatus.SetWindowText("Activating Blocking Mechanism");
		}
		else if (i == 100)
		{
			m_stStatus.SetWindowText("Engine Started");
		}
		else
			continue;

		if(i == 100)
				{
					loop = 0;
					while(loop != 200000000)
					{
						loop++;
					}
					if(m_Mode == 0)
						i = 101;
				}
	
	}
	m_prog.ShowWindow(FALSE);
	m_stStatus.SetWindowText("");
	m_btMode.EnableWindow(FALSE);
	m_btIDS.EnableWindow(FALSE);
	m_btFirewall.EnableWindow(FALSE);
	m_StopEngine.EnableWindow(TRUE);
}

void CConfigC::OnRadioStopEngine() 
{
	m_ptrMainApp->StopEngine();	
	m_btSelAdap.EnableWindow(TRUE);
	m_StartEngine.EnableWindow(TRUE);
	m_StartEngine.SetCheck(0);
	m_btMode.EnableWindow(TRUE);
	m_btIDS.EnableWindow(TRUE);
	m_btFirewall.EnableWindow(TRUE);
	m_stStatus.SetWindowText("Engine Stopped");
	m_StopEngine.EnableWindow(FALSE);


}
