#if !defined(AFX_CONFIGA_H__20B0A115_8768_47A1_A789_B2E6FAFF9B4E__INCLUDED_)
#define AFX_CONFIGA_H__20B0A115_8768_47A1_A789_B2E6FAFF9B4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "IFServerSubDlg.h"
#include "IFServer.h"	// Added by ClassView
// ConfigA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigA dialog

class CConfigA : public CIFServerSubDlg
{
// Construction
public:
	CIFServerApp *m_ptrMainApp;
	CConfigA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigA)
	enum { IDD = IDD_DIALOG1 };
	CString	m_Password;
	CString	m_iComboSelection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigA)
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnChangePassword();
	afx_msg void OnKillfocusUserName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGA_H__20B0A115_8768_47A1_A789_B2E6FAFF9B4E__INCLUDED_)
