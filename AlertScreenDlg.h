#if !defined(AFX_ALERTSCREENDLG_H__E7FA4110_F114_4E27_BAB2_21106B960BBC__INCLUDED_)
#define AFX_ALERTSCREENDLG_H__E7FA4110_F114_4E27_BAB2_21106B960BBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlertScreenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlertScreenDlg dialog

class CAlertScreenDlg : public CDialog
{
// Construction
public:
	UINT nResult;
	BOOL InitInstance();
	CAlertScreenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlertScreenDlg)
	enum { IDD = IDD_DIALOG6 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlertScreenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlertScreenDlg)
	afx_msg void OnOnProgressShockwaveflash1(long percentDone);
	afx_msg void OnOnReadyStateChangeShockwaveflash1(long newState);
	afx_msg void OnFSCommandShockwaveflash1(LPCTSTR command, LPCTSTR args);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALERTSCREENDLG_H__E7FA4110_F114_4E27_BAB2_21106B960BBC__INCLUDED_)
