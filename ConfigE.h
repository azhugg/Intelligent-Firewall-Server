#if !defined(AFX_CONFIGE_H__F8BB5E75_0CCF_462B_BB5D_D9F24433EFA0__INCLUDED_)
#define AFX_CONFIGE_H__F8BB5E75_0CCF_462B_BB5D_D9F24433EFA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigE.h : header file
//

#include "IFServerSubDlg.h"
#include "BalloonHelp.h"	// Added by ClassView
#include "IFServer.h"	// Added by ClassView
#include "FileManagement.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CConfigE dialog

class CConfigE : public CIFServerSubDlg
{
// Construction
public:
	CFileManagement *m_ptrFileMan;
	CIFServerApp *m_ptrMainApp;
	CString m_csLogFile;
	
	CConfigE(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigE)
	enum { IDD = IDD_DIALOG5 };
	CListBox	m_lstAlert;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigE)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigE)
	afx_msg void OnMove(int x, int y);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnOpenDumpFile();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBalloonHelp bhPersistent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGE_H__F8BB5E75_0CCF_462B_BB5D_D9F24433EFA0__INCLUDED_)
