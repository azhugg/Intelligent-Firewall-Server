// RASnifferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RASniffer.h"
#include "RASnifferDlg.h"
#include "PacketStatusDlg.h"
//#include "pcap.h"
#include "libcap.h"
#include "log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRASnifferDlg dialog

CRASnifferDlg::CRASnifferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRASnifferDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRASnifferDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRASnifferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRASnifferDlg)
	DDX_Control(pDX, IDC_BUTTON_PKTCOUNT, m_PktCount);
	DDX_Control(pDX, IDC_BUTTON_STOPENGINE, m_StopEngine);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartEngine);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRASnifferDlg, CDialog)
	//{{AFX_MSG_MAP(CRASnifferDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON_ARRAY, OnButtonArray)
	ON_BN_CLICKED(IDC_BUTTON_STOPENGINE, OnButtonStopengine)
	ON_BN_CLICKED(IDC_BUTTON_PKTCOUNT, OnButtonPktcount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRASnifferDlg message handlers

BOOL CRASnifferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_StopEngine.EnableWindow(FALSE);
	m_PktCount.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRASnifferDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRASnifferDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRASnifferDlg::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	CRASnifferApp* ptrMainApp;
    ptrMainApp =PTRAPP;
	ptrMainApp->StartSniff();
	m_PktCount.EnableWindow(TRUE);
	m_StopEngine.EnableWindow(TRUE);
	m_StartEngine.EnableWindow(FALSE);
}

void CRASnifferDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	/*char *devicev;
	char erro[256];

	devicev = pcap_lookupdev(erro);
//	MessageBox("Check","Check",MB_OK);
	CLibCap *objlib;

	objlib->SetAdapt(devicev);
	CLog *objLog;
	objLog->CreatePidFile(devicev);*/
	UpdateData(FALSE);
}

void CRASnifferDlg::OnButtonArray() 
{
	// TODO: Add your control notification handler code here
//	u_long swap[12]= {0x00000000,0x80000000,0xC0000000,0xE0000000,0xF0000000,0xF8000000,0xFC000000,0xFE000000,0xFF000000,0xFF800000,0xFFC00000,0xFFE00000};
		return;
}
void CRASnifferDlg::OnButtonStopengine() 
{
	// TODO: Add your control notification handler code here
	CRASnifferApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	m_StopEngine.EnableWindow(FALSE);
	m_StartEngine.EnableWindow(FALSE);
	m_PktCount.EnableWindow(FALSE);
	ptrMainApp->CleanExit(SIGQUIT);
}

void CRASnifferDlg::OnButtonPktcount() 
{
	// TODO: Add your control notification handler code here
	CPacketStatusDlg m_dlg;
	int nResponse = m_dlg.DoModal();
/*	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}*/
}
