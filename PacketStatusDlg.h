#if !defined(AFX_PACKETSTATUSDLG_H__1FF43965_B283_4FBC_B76F_89C1A8106E2A__INCLUDED_)
#define AFX_PACKETSTATUSDLG_H__1FF43965_B283_4FBC_B76F_89C1A8106E2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketStatusDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPacketStatusDlg dialog

class CPacketStatusDlg : public CDialog
{
// Construction
public:
	BOOL OnInitDialog();
	UINT uResult;
	void RefreshPacketCount();
	CPacketStatusDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPacketStatusDlg)
	enum { IDD = IDD_DIALOG_PKTSTATUS };
	CString	m_ArpCount;
	CString	m_FragCount;
	CString	m_IcmpCount;
	CString	m_IPV6Count;
	CString	m_IPXCount;
	CString	m_OtherCount;
	CString	m_TcpCount;
	CString	m_UdpCount;
	CString	m_Temp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPacketStatusDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPacketStatusDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETSTATUSDLG_H__1FF43965_B283_4FBC_B76F_89C1A8106E2A__INCLUDED_)
