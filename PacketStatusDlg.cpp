// PacketStatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IFServer.h"
#include "PacketStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

	
	


/////////////////////////////////////////////////////////////////////////////
// CPacketStatusDlg dialog
CPacketStatusDlg ptrPkt;
VOID CALLBACK MyTimerProc(HWND hwnd,UINT message,UINT idTimer,DWORD dwTime);

CPacketStatusDlg::CPacketStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPacketStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPacketStatusDlg)
	m_ArpCount = _T("");
	m_FragCount = _T("");
	m_IcmpCount = _T("");
	m_IPV6Count = _T("");
	m_IPXCount = _T("");
	m_OtherCount = _T("");
	m_TcpCount = _T("");
	m_UdpCount = _T("");
	m_Temp = _T("");
	//}}AFX_DATA_INIT
	m_Temp = "A";
}


void CPacketStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPacketStatusDlg)
	DDX_Text(pDX, IDC_STATIC_ARPCOUNT, m_ArpCount);
	DDX_Text(pDX, IDC_STATIC_FRAGSCOUNT, m_FragCount);
	DDX_Text(pDX, IDC_STATIC_ICMPCOUNT, m_IcmpCount);
	DDX_Text(pDX, IDC_STATIC_IPV6COUNT, m_IPV6Count);
	DDX_Text(pDX, IDC_STATIC_IPXCOUNT, m_IPXCount);
	DDX_Text(pDX, IDC_STATIC_OTHERCOUNT, m_OtherCount);
	DDX_Text(pDX, IDC_STATIC_TCPCOUNT, m_TcpCount);
	DDX_Text(pDX, IDC_STATIC_UDPCOUNT, m_UdpCount);
	DDX_Text(pDX, IDC_EDIT_TEMP, m_Temp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPacketStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CPacketStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketStatusDlg message handlers

void CPacketStatusDlg::RefreshPacketCount()
{	
//	UpdateData(TRUE);
//	m_ArpCount.Insert(0,(LPCTSTR)ptrMainApp->pc.arp);
	m_Temp.Insert(0 , "B");//ptrMainApp->pc.frags;
//	m_FragCount = "12";//ptrMainApp->pc.frags;
//	m_IcmpCount = ptrMainApp->pc.icmp;
//	m_IPV6Count = ptrMainApp->pc.ipv6;
//	m_IPXCount = ptrMainApp->pc.ipx;
//	m_TcpCount = ptrMainApp->pc.tcp;
//	m_OtherCount = ptrMainApp->pc.other;
//	m_UdpCount = ptrMainApp->pc.udp;
//	UpdateData(FALSE);
 	CWnd::UpdateData(FALSE);
	//UpdateData(FALSE);
}



VOID CALLBACK MyTimerProc(HWND hwnd,UINT message,UINT idTimer,DWORD dwTime)
{
	AfxMessageBox("Gotcha",MB_OK,0);
	CPacketStatusDlg objDlg;
	objDlg.RefreshPacketCount();
}

BOOL CPacketStatusDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	::KillTimer(this->m_hWnd,0);
	return CDialog::DestroyWindow();
}

void CPacketStatusDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(FALSE);
//	CDialog::OnOK();
}

BOOL CPacketStatusDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	RefreshPacketCount();

	uResult = ::SetTimer(this->m_hWnd,0,6000,(TIMERPROC) MyTimerProc); 
	
	return 0;
}
