// RASnifferDlg.h : header file
//

#if !defined(AFX_RASNIFFERDLG_H__EA147228_5735_11D6_86D4_006008A601F7__INCLUDED_)
#define AFX_RASNIFFERDLG_H__EA147228_5735_11D6_86D4_006008A601F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRASnifferDlg dialog

class CRASnifferDlg : public CDialog
{
// Construction
public:
	CRASnifferDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRASnifferDlg)
	enum { IDD = IDD_RASNIFFER_DIALOG };
	CButton	m_PktCount;
	CButton	m_StopEngine;
	CButton	m_StartEngine;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRASnifferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRASnifferDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnButton1();
	afx_msg void OnButtonArray();
	afx_msg void OnButtonStopengine();
	afx_msg void OnButtonPktcount();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RASNIFFERDLG_H__EA147228_5735_11D6_86D4_006008A601F7__INCLUDED_)
