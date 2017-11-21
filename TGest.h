#if !defined(AFX_TGEST_H__219CFD51_348B_4EB3_B6E4_3BE29EEBE362__INCLUDED_)
#define AFX_TGEST_H__219CFD51_348B_4EB3_B6E4_3BE29EEBE362__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TGest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TGest dialog

class TGest : public CDialog
{
// Construction
public:
	TGest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TGest)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TGest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TGest)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TGEST_H__219CFD51_348B_4EB3_B6E4_3BE29EEBE362__INCLUDED_)
