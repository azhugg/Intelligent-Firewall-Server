#if !defined(AFX_CONFIGB_H__C00461DF_5588_4296_AD56_BD4AEE57948E__INCLUDED_)
#define AFX_CONFIGB_H__C00461DF_5588_4296_AD56_BD4AEE57948E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigB.h : header file
//

#include "IFServerDlg.h"
#include "IFServer.h"	// Added by ClassView
/////////////////////////////////////////////////////////////////////////////
// CConfigB dialog

class CConfigB : public CIFServerSubDlg
{
// Construction
public:
	CIFServerApp *m_ptrMainApp;
	int startf;
	void NullIndicator();
	CStatic *stArray[15];
	void SetIndicators(int value);
	CConfigB(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigB)
	enum { IDD = IDD_DIALOG2 };
	CStatic	m_DenyList;
	CSpinButtonCtrl	m_BHSen;
	CStatic	m_sIn9;
	CStatic	m_sIn8;
	CStatic	m_sIn7;
	CStatic	m_sIn6;
	CStatic	m_sIn5;
	CStatic	m_sIn4;
	CStatic	m_sIn3;
	CStatic	m_sIn2;
	CStatic	m_sIn15;
	CStatic	m_sIn14;
	CStatic	m_sIn13;
	CStatic	m_sIn12;
	CStatic	m_sIn10;
	CStatic	m_sIn11;
	CStatic	m_sIn1;
	CStatic	m_ind9;
	CStatic	m_ind1;
	CSpinButtonCtrl	m_BHSens;
	CString	m_BHValue;
	CString	m_cstrFileName;
	CString	m_csRulesDirectory;
	CString	m_csRulesFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigB)
		afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
		virtual BOOL OnInitDialog();
	afx_msg void OnUpdateEdit1();
	afx_msg void OnChangeEdit1();
	afx_msg void OnBrowse();
	afx_msg void OnRulesDirectory();
	afx_msg void OnMainRuleFile();
	afx_msg void OnOpenDenyListFile();
	afx_msg void Oncheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGB_H__C00461DF_5588_4296_AD56_BD4AEE57948E__INCLUDED_)
