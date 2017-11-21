#if !defined(AFX_CONFIGC_H__010308FB_DF57_4A3E_8AE9_F6320291067D__INCLUDED_)
#define AFX_CONFIGC_H__010308FB_DF57_4A3E_8AE9_F6320291067D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigC.h : header file
//
#include "IFServerDlg.h"
#include "IFServer.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CConfigC dialog

class CConfigC : public CIFServerSubDlg
{
// Construction
public:
	int m_Mode;
	CIFServerApp *m_ptrMainApp;
	CConfigC(CWnd* pParent = NULL);   // standard constructor
	CString Adapter;

// Dialog Data
	//{{AFX_DATA(CConfigC)
	enum { IDD = IDD_DIALOG3 };
	CStatic	m_stAdapName;
		CButton	m_btEngineMode;
		CButton	m_btMode;
		CButton	m_btSelAdap;
		CButton	m_StartEngine;
		CButton	m_StopEngine;
		CStatic	m_stStatus;
		CStatic	m_stSniffer;
		CStatic	m_stIDS;
		CStatic	m_stFirewall;
		CStatic	m_SelAdaptor;
		CButton m_btIDS;
		CButton m_btFirewall;
		CProgressCtrl	m_prog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigC)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioSniffer();
	afx_msg void OnRadioIdsOnly();
	afx_msg void OnRadioFirewall();
	afx_msg void OnButtonSelectAdapter();
	afx_msg void OnRadioStartEngine();
	afx_msg void OnRadioStopEngine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGC_H__010308FB_DF57_4A3E_8AE9_F6320291067D__INCLUDED_)
