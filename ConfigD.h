#if !defined(AFX_CONFIGD_H__ED0210BE_FA04_4F35_A8DC_839E30D1EAA2__INCLUDED_)
#define AFX_CONFIGD_H__ED0210BE_FA04_4F35_A8DC_839E30D1EAA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigD dialog

#include "IFServerDlg.h"
#include "FileManagement.h"	// Added by ClassView
#include "IFServer.h"	// Added by ClassView
#include "TrafficControl.h"	// Added by ClassView

class CConfigD : public CIFServerSubDlg
{
// Construction
public:
	void RefreshDenyList();
	CTrafficControl *m_ptrTraffic;
	CIFServerApp *m_ptrMainApp;
	CFileManagement *m_ptrFM;
	CConfigD(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigD)
	enum { IDD = IDD_DIALOG4 };
	CIPAddressCtrl	m_IpAdd;
	CListBox	m_DenyList;
	CListBox	m_IncludeList;
	CListBox	m_List;
	CListBox	m_IDSRules;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigD)
		virtual BOOL OnInitDialog();
	afx_msg void OnButton3();
	afx_msg void OnOpenIncludeFile();
	afx_msg void OnOpenMainRuleFile();
	afx_msg void OnStatistics();
	afx_msg void OnDeleteDenyFilter();
	afx_msg void OnAddClientToDeny();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGD_H__ED0210BE_FA04_4F35_A8DC_839E30D1EAA2__INCLUDED_)
